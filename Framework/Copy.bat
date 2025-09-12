@echo off
cd /d %~dp0


xcopy /y /e /i .\Engine\public\*.* .\EngineSDK\inc\

xcopy	/y	.\Engine\bin\debug\Engine.dll	.\Client\bin\debug\
xcopy	/y	.\Engine\bin\release\Engine.dll	.\Client\bin\release\

xcopy	/y	.\Engine\bin\debug\Engine.dll	.\Importer\bin\debug\
xcopy	/y	.\Engine\bin\release\Engine.dll	.\Importer\bin\release\

xcopy	/y	.\Engine\bin\debug\Engine.lib	.\EngineSDK\Lib\debug\
xcopy	/y	.\Engine\bin\release\Engine.lib	.\EngineSDK\Lib\release\

pause