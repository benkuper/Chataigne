; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

#define ApplicationName 'Chataigne'
#define ApplicationVersion GetFileVersion('Binaries/CI/App/Chataigne.exe')

[Setup]
AppName={#ApplicationName}
AppVersion={#ApplicationVersion}
AppPublisher=Ben Kuper
AppPublisherURL=http://benjamin.kuperberg.fr/chataigne
DefaultDirName={pf}\{#ApplicationName}
DefaultGroupName=ApplicationName
UninstallDisplayIcon={app}\{#ApplicationName}.exe
UninstallDisplayName={#ApplicationName}
Compression=lzma2
SolidCompression=yes
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
OutputDir=/
OutputBaseFilename={#ApplicationName}-win-x64-bleedingedge
SetupIconFile=setup.ico

[Files]
Source: "Binaries/CI/App/{#ApplicationName}.exe"; DestDir: "{app}"
Source: "Binaries/CI/App/*.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\{#ApplicationName}"; Filename: "{app}\{#ApplicationName}.exe"

[Run]
Filename: "{app}\{#ApplicationName}.exe"; Description: "{cm:LaunchProgram,{#ApplicationName}.exe}"; Flags: nowait postinstall skipifsilent
