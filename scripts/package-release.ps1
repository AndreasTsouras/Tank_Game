param(
    [string]$Version = "0.1.0",
    [string]$Msys2Root = "C:\msys64",
    [switch]$SkipInstaller
)

$ErrorActionPreference = "Stop"

function Resolve-Msys2BashPath {
    param([string]$PreferredRoot)

    $roots = @(
        $PreferredRoot,
        "C:\msys64",
        "D:\msys64",
        (Join-Path $env:LOCALAPPDATA "msys64"),
        (Join-Path $env:USERPROFILE "msys64"),
        (Join-Path ${env:ProgramFiles} "msys64"),
        (Join-Path ${env:ProgramFiles(x86)} "msys64")
    ) | Where-Object { $_ -and $_.Trim().Length -gt 0 } | Select-Object -Unique

    foreach ($root in $roots) {
        $candidate = Join-Path $root "usr\bin\bash.exe"
        if (Test-Path $candidate) {
            return $candidate
        }
    }

    return $null
}

$bash = Resolve-Msys2BashPath -PreferredRoot $Msys2Root
if (-not $bash) {
    throw "MSYS2 bash not found. Checked common paths plus -Msys2Root '$Msys2Root'. Pass -Msys2Root with your MSYS2 install folder (for example D:\msys64)."
}

$repoRoot = Split-Path -Parent $PSScriptRoot
$repoRootUnix = & $bash -lc "cygpath -u '$repoRoot'"
$installDirWin = Join-Path $repoRoot "dist\TankGame"
$distRootWin = Join-Path $repoRoot "dist"
$zipPathWin = Join-Path $distRootWin "TankGame-win64-$Version.zip"

if (Test-Path $installDirWin) {
    Remove-Item -Recurse -Force $installDirWin
}

$installDirUnix = "$repoRootUnix/dist/TankGame"

$buildAndStage = @"
set -euo pipefail
export PATH=/mingw64/bin:/usr/bin:`$PATH
pacman -S --needed --noconfirm \
  mingw-w64-x86_64-toolchain \
  mingw-w64-x86_64-cmake \
  mingw-w64-x86_64-ninja \
  mingw-w64-x86_64-glew \
  mingw-w64-x86_64-freeglut \
  mingw-w64-x86_64-glm
cd '$repoRootUnix/TankGame'
cmake -S . -B build-release -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX='$installDirUnix'
cmake --build build-release -j
cmake --install build-release
exe='$installDirUnix/TankGame.exe'
deps=`$(ldd "`$exe" | awk '/=> \\/mingw64\\/bin\\// {print `$3}' | sort -u)
for dep in `$deps; do
  cp -f "`$dep" '$installDirUnix'/
done
"@

& $bash -lc $buildAndStage

if (Test-Path $zipPathWin) {
    Remove-Item -Force $zipPathWin
}

if (!(Test-Path $distRootWin)) {
    New-Item -ItemType Directory -Path $distRootWin | Out-Null
}

Compress-Archive -Path "$installDirWin\*" -DestinationPath $zipPathWin -Force
Write-Host "Created ZIP: $zipPathWin"

if ($SkipInstaller) {
    Write-Host "Installer build skipped."
    exit 0
}

$innoCandidates = @(
    (Join-Path ${env:ProgramFiles(x86)} "Inno Setup 6\ISCC.exe"),
    (Join-Path ${env:ProgramFiles} "Inno Setup 6\ISCC.exe")
)

$iscc = $innoCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not $iscc) {
    Write-Warning "Inno Setup compiler (ISCC.exe) not found. ZIP was still created."
    exit 0
}

$issFile = Join-Path $repoRoot "packaging\TankGame.iss"
if (!(Test-Path $issFile)) {
    throw "Missing Inno Setup script: $issFile"
}

& $iscc "/DAppVersion=$Version" "/DSourceDir=$installDirWin" "/DOutputDir=$distRootWin" $issFile
Write-Host "Installer built in: $distRootWin"
