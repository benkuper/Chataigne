; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

#define ApplicationName 'Chataigne'
#define ApplicationVersion GetStringFileInfo('Binaries/Win7CI/App/Chataigne.exe',"ProductVersion")

[Setup]
AppName={#ApplicationName}
AppId={#ApplicationName}
AppVersion={#ApplicationVersion}
AppPublisher=Ben Kuper
AppPublisherURL=http://benjamin.kuperberg.fr/chataigne
DefaultDirName={pf}\{#ApplicationName}
DefaultGroupName={#ApplicationName}
UninstallDisplayIcon={app}\{#ApplicationName}.exe
UninstallDisplayName={#ApplicationName}
Compression=lzma2
SolidCompression=yes
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
OutputDir=/
OutputBaseFilename={#ApplicationName}-win-x64-bleedingedge
SetupIconFile=setup.ico

[Messages]
SetupWindowTitle={#ApplicationName} {#ApplicationVersion} Setup

[Files]
Source: "Binaries/CI/App/{#ApplicationName}.exe"; DestDir: "{app}" ; Flags: ignoreversion
Source: "Binaries/CI/App/*.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#ApplicationName}"; Filename: "{app}\{#ApplicationName}.exe"

[Run]
Filename: "{app}\{#ApplicationName}.exe"; Description: "{cm:LaunchProgram,{#ApplicationName}.exe}"; Flags: nowait postinstall skipifsilent

[Code]
function GetUninstallString(): String;
var
  sUnInstPath: String;
  sUnInstallString: String;
begin
  sUnInstPath := ExpandConstant('Software\Microsoft\Windows\CurrentVersion\Uninstall\{#emit SetupSetting("AppId")}_is1');
  sUnInstallString := '';
  if not RegQueryStringValue(HKLM, sUnInstPath, 'UninstallString', sUnInstallString) then
    RegQueryStringValue(HKCU, sUnInstPath, 'UninstallString', sUnInstallString);
  Result := sUnInstallString;
end;


/////////////////////////////////////////////////////////////////////
function IsUpgrade(): Boolean;
begin
  Result := (GetUninstallString() <> '');
end;


/////////////////////////////////////////////////////////////////////
function UnInstallOldVersion(): Integer;
var
  sUnInstallString: String;
  iResultCode: Integer;
begin
// Return Values:
// 1 - uninstall string is empty
// 2 - error executing the UnInstallString
// 3 - successfully executed the UnInstallString

  // default return value
  Result := 0;

  // get the uninstall string of the old app
  sUnInstallString := GetUninstallString();
  if sUnInstallString <> '' then begin
    sUnInstallString := RemoveQuotes(sUnInstallString);
    if Exec(sUnInstallString, '/SILENT /NORESTART /SUPPRESSMSGBOXES','', SW_HIDE, ewWaitUntilTerminated, iResultCode) then
      Result := 3
    else
      Result := 2;
  end else
    Result := 1;
end;

/////////////////////////////////////////////////////////////////////
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep=ssInstall) then
  begin
    if (IsUpgrade()) then
    begin
      UnInstallOldVersion();
    end;
  end;
end;