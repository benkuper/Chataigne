; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=Chataigne
AppVersion=1.3.11
AppPublisher=Ben Kuper
AppPublisherURL=http://benjamin.kuperberg.fr/chataigne
DefaultDirName={pf}\Chataigne
DefaultGroupName=Chataigne
UninstallDisplayIcon={app}\Chataigne.exe
UninstallDisplayName=Chataigne
Compression=lzma2
SolidCompression=yes
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

[Files]
Source: "Binaries/App/Chataigne.exe"; DestDir: "{app}"
Source: "Binaries/App/*.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\Chataigne"; Filename: "{app}\Chataigne.exe"

[Run]
Filename: "{app}\Chataigne.exe"; Description: "{cm:LaunchProgram,Chataigne.exe}"; Flags: nowait postinstall skipifsilent
