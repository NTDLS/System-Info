[Setup]
;-- Main Setup Information
 AppName                  = SystemInfo
 AppVerName               = SystemInfo 1.0.1.9
 AppCopyright             = Copyright © 1995-2009 NetworkDLS
 DefaultDirName           = {pf}\NetworkDLS\SystemInfo
 DefaultGroupName         = NetworkDLS\SystemInfo
 UninstallDisplayIcon     = {app}\SystemInfo.Exe
 PrivilegesRequired       = PowerUser
 Uninstallable            = Yes
 AppMutex                 = SystemInfo
 Compression              = BZIP/9
 OutputBaseFilename       = Setup
 DisableStartupPrompt     = Yes
 DirExistsWarning         = No
 WizardImageFile          = \..\..\@Resources\Setup\LgSetup.bmp
 WizardSmallImageFile     = \..\..\@Resources\Setup\SmSetup.bmp
 LicenseFile              = \..\..\@Resources\Setup\EULA.txt
 VersionInfoVersion       = 1.0.1.8
 MinVersion               = 0.0,5.0

;-- Windows 2000 & XP (Support Dialog)
 AppPublisher    = NetworkDLS
 AppPublisherURL = http://www.NetworkDLS.com/
 AppUpdatesURL   = http://www.NetworkDLS.com/
 AppVersion      = 1.0.1.9

[Files]
 Source: "..\Release\SystemInfo.Exe";             DestDir: "{app}"; Flags: IgnoreVersion;
 Source: "..\..\@AutoUpdate\Win32\AutoUpdate.Exe"; DestDir: "{app}"; Flags: RestartReplace;
 Source: "AutoUpdate.xml";                        DestDir: "{app}"; Flags: IgnoreVersion;

[Icons]
 Name: "{group}\SystemInfo";  Filename: "{app}\SystemInfo.Exe";
 Name: "{group}\AutoUpdate";  Filename: "{app}\AutoUpdate.Exe";
 Name: "{group}\Uninstall";   Filename: "{uninstallexe}";

[InstallDelete]
 Type: files; Name: "{app}\InfoCmd.Exe"
 Type: files; Name: "{app}\AutoUpdate.Exe"
 Type: files; Name: "{group}\AutoUpdate.Lnk"

[RUN]
Filename: "{app}\SystemInfo.Exe"; Description: "Start SystemInfo now?"; Flags: PostInstall NoWait;

