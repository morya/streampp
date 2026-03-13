; InnoSetup script for Screen Streamer installer

#define MyAppName "Screen Streamer"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "ScreenStreamer"
#define MyAppURL "https://github.com/yourusername/screen-streamer"
#define MyAppExeName "ScreenStreamer.exe"

[Setup]
AppId={{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile=..\..\LICENSE
OutputDir=..\Output
OutputBaseFilename=ScreenStreamer_Setup
SetupIconFile=..\..\resources\icons\app.ico
Compression=lzma2/ultra
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chinese"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 6.1

[Files]
; Main executable
Source: "..\..\build\release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion

; Qt DLLs
Source: "..\..\build\release\*.dll"; DestDir: "{app}"; Flags: ignoreversion

; Qt plugins
Source: "..\..\build\release\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\..\build\release\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\..\build\release\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\..\build\release\multimedia\*"; DestDir: "{app}\multimedia"; Flags: ignoreversion recursesubdirs createallsubdirs

; FFmpeg DLLs (adjust source path as needed)
Source: "C:\ffmpeg\bin\avcodec-*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\ffmpeg\bin\avformat-*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\ffmpeg\bin\avutil-*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\ffmpeg\bin\swscale-*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\ffmpeg\bin\avdevice-*.dll"; DestDir: "{app}"; Flags: ignoreversion

; SRT DLLs if needed
Source: "C:\ffmpeg\bin\srt.dll"; DestDir: "{app}"; Flags: ignoreversion

; Documentation
Source: "..\..\README.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\LICENSE"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: filesandordirs; Name: "{localappdata}\ScreenStreamer"

[Code]
function InitializeSetup(): Boolean;
begin
  Result := True;
  
  // Check for required dependencies
  if not FileExists(ExpandConstant('{sys}\msvcp140.dll')) then
  begin
    MsgBox('Microsoft Visual C++ 2015-2022 Redistributable is required.' + #13#10 +
           'Please install it from: https://aka.ms/vs/17/release/vc_redist.x64.exe',
           mbError, MB_OK);
    Result := False;
  end;
end;