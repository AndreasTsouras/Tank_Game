param(
    [string]$Version = "0.1.0",
    [string]$Msys2Root = "C:\msys64",
    [switch]$SkipInstaller
)

$ErrorActionPreference = "Stop"

$bash = Join-Path $Msys2Root "usr\bin\bash.exe"
if (!(Test-Path $bash)) {
    throw "MSYS2 bash not found at '$bash'. Install MSYS2 first (https://www.msys2.org/)."
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
