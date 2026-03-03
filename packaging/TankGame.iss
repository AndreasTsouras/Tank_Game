#ifndef AppVersion
  #define AppVersion "0.1.0"
#endif

#ifndef SourceDir
  #define SourceDir "dist\TankGame"
#endif

#ifndef OutputDir
  #define OutputDir "dist"
#endif

[Setup]
AppId={{4E3DF10D-8978-4EF4-A596-9EB53CB6D219}
AppName=Tank Game
AppVersion={#AppVersion}
AppPublisher=Andreas Tsouras
DefaultDirName={autopf}\Tank Game
DefaultGroupName=Tank Game
OutputDir={#OutputDir}
OutputBaseFilename=TankGameSetup-{#AppVersion}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\TankGame.exe

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "{#SourceDir}\*"; DestDir: "{app}"; Flags: recursesubdirs createallsubdirs ignoreversion

[Icons]
Name: "{autoprograms}\Tank Game"; Filename: "{app}\TankGame.exe"
Name: "{autodesktop}\Tank Game"; Filename: "{app}\TankGame.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\TankGame.exe"; Description: "{cm:LaunchProgram,Tank Game}"; Flags: nowait postinstall skipifsilent
