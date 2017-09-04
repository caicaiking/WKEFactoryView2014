; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppVersion "1.3.5.9.1"

#define MyAppName "KE FactoryView 2014"#define MyAppPublisher "KE Electronics"
#define MyAppURL "http://www.waynekerr.com/"

#define MyAppExeName "KEFactoryView2014.exe"
#define SpecialCompanyVersion "-3255MeterMode"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{E7FAC558-7336-42E6-8198-6DC52DA5C3FE}
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
Source: "D:\QT\WKEFactoryView2014\libs\KEFactoryView2014.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\QT\WKEFactoryView2014\libs\KEFactoryView2014.exe"; DestDir: "{app}"; Flags: ignoreversion
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

