# Tank Game (OpenGL, C++, GLUT)

This project is set up for **Windows + MSYS2 MinGW64** and supports:
- one-command reopen/build/run for development
- one-command packaging to a distributable app folder + ZIP
- optional Windows installer (`Setup.exe`) via Inno Setup
- automated GitHub Releases on version tags

## 1) Install MSYS2

Install from:
- https://www.msys2.org/

Default path used by scripts:
- `C:\msys64`

## 2) First-time MSYS2 update (exact commands)

Open **MSYS2 MSYS** terminal:

```bash
pacman -Syu --noconfirm
```

If prompted, close and reopen **MSYS2 MSYS**, then run again:

```bash
pacman -Syu --noconfirm
```

## 3) Dependency install (exact command)

Open **MSYS2 MINGW64** terminal:

```bash
pacman -S --needed --noconfirm \
  mingw-w64-x86_64-toolchain \
  mingw-w64-x86_64-cmake \
  mingw-w64-x86_64-ninja \
  mingw-w64-x86_64-glew \
  mingw-w64-x86_64-freeglut \
  mingw-w64-x86_64-glm
```

## 4) One-command reopen/build/run

From repo root in PowerShell:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\reopen-msys2.ps1
```

This builds and runs the game using MSYS2/MinGW64.
The script auto-detects common MSYS2 install paths. If needed, override with:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\reopen-msys2.ps1 -Msys2Root "D:\msys64"
```

## 5) One-command package for redistribution

From repo root in PowerShell:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\package-release.ps1 -Version 1.0.0
```

Outputs:
- `dist\TankGame\` (redistributable app folder)
- `dist\TankGame-win64-1.0.0.zip`
- `dist\TankGameSetup-1.0.0.exe` (if Inno Setup is installed)

If you only want ZIP (no installer):

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\package-release.ps1 -Version 1.0.0 -SkipInstaller
```

If your MSYS2 is in a custom folder:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\package-release.ps1 -Version 1.0.0 -Msys2Root "D:\msys64"
```

## 6) Inno Setup (installer tool)

Install Inno Setup from:
- https://jrsoftware.org/isinfo.php

Installer template in repo:
- `packaging/TankGame.iss`

## 7) GitHub Release automation

Workflow file:
- `.github/workflows/release.yml`

Trigger a release by pushing a version tag:

```powershell
git tag v1.0.0
git push origin v1.0.0
```

The workflow builds:
- ZIP artifact
- Setup installer artifact

and publishes both to the GitHub Release for that tag.
