param(
    [string]$Msys2Root = "C:\msys64"
)

$ErrorActionPreference = "Stop"

$bash = Join-Path $Msys2Root "usr\bin\bash.exe"
if (!(Test-Path $bash)) {
    throw "MSYS2 bash not found at '$bash'. Install MSYS2 first (https://www.msys2.org/)."
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
