param(
    [string]$Msys2Root = "C:\msys64"
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
        if (!(Test-Path $root)) {
            continue
        }
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

$cmd = @"
export PATH=/mingw64/bin:/usr/bin:`$PATH
pacman -S --needed --noconfirm \
  mingw-w64-x86_64-toolchain \
  mingw-w64-x86_64-cmake \
  mingw-w64-x86_64-ninja \
  mingw-w64-x86_64-glew \
  mingw-w64-x86_64-freeglut \
  mingw-w64-x86_64-glm
cd '$repoRootUnix/TankGame'
cmake -S . -B build -G Ninja
cmake --build build -j
./build/TankGame.exe
"@

& $bash -lc $cmd
