; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppVersion "1.4.7"

#define MyAppName "WKE FactoryView 2014"#define MyAppPublisher "Wayne Kerr Electronics"
#define MyAppURL "http://www.waynekerrtest.com/"
#define MyAppExeName "WKEFactoryView2014.exe"
#define SpecialCompanyVersion "-3260HFCalFinnal"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{E7100E37-96EC-4F3C-943F-A874B332EB6C}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=C:\Program Files (x86)\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=D:\QT\WKEFactoryView2014Install\
OutputBaseFilename= {#MyAppName} V{#MyAppVersion}{#SpecialCompanyVersion}
SetupIconFile=D:\QT\WKEFactoryView2014\wk.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]                                          
Source: "D:\QT\WKEFactoryView2014\libs\WKEFactoryView2014.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\QT\WKEFactoryView2014\libs\WKEFactoryView2014.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\QT\WKEFactoryView2014\Run\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\VisaRunTime\setup.exe"; Description: "{cm:LaunchProgram,{#StringChange("Install visa driver", '&', '&&')}}"; Flags: postinstall skipifsilent
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

