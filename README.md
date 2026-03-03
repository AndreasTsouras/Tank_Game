# Tank Game (OpenGL, C++, GLUT)

This project builds and runs with **MSYS2 MinGW64** on Windows.

## 1) Install MSYS2

Install MSYS2 from:
- https://www.msys2.org/

Default install path used below:
- `C:\msys64`

## 2) First-time MSYS2 update (exact commands)

Open **MSYS2 MSYS** terminal and run:

```bash
pacman -Syu --noconfirm
```

Close terminal if it asks, reopen **MSYS2 MSYS**, then run again:

```bash
pacman -Syu --noconfirm
```

## 3) Install build/runtime dependencies (exact command)

Open **MSYS2 MINGW64** terminal and run:

```bash
pacman -S --needed --noconfirm \
  mingw-w64-x86_64-toolchain \
  mingw-w64-x86_64-cmake \
  mingw-w64-x86_64-ninja \
  mingw-w64-x86_64-glew \
  mingw-w64-x86_64-freeglut \
  mingw-w64-x86_64-glm
```

## 4) One-command reopen/build/run from Windows PowerShell

From repo root:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\reopen-msys2.ps1
```

What it does:
- Ensures required `mingw-w64-x86_64-*` packages are installed (`--needed`).
- Configures CMake in `TankGame/build`.
- Builds the game.
- Runs `TankGame.exe`.

## Optional: manual build in MINGW64 shell

```bash
cd /c/Users/tsour/Tank_Game_02/Tank_Game/TankGame
cmake -S . -B build -G Ninja
cmake --build build -j
./build/TankGame.exe
```
