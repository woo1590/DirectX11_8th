@echo off
cd /d %~dp0


xcopy /y /e /i .\Engine\public\*.* .\EngineSDK\inc\

xcopy	/y	.\Engine\bin\debug\Engine.dll	.\Client\bin\debug\
xcopy	/y	.\Engine\bin\release\Engine.dll	.\Client\bin\release\

xcopy	/y	.\Engine\bin\debug\Engine.dll	.\Importer\bin\debug\
xcopy	/y	.\Engine\bin\release\Engine.dll	.\Importer\bin\release\

xcopy	/y	.\Engine\bin\debug\Engine.dll	.\MapEditor\bin\debug\
xcopy	/y	.\Engine\bin\release\Engine.dll	.\MapEditor\bin\release\

xcopy	/y	.\Engine\bin\debug\Engine.dll	.\EffectEditor\bin\debug\
xcopy	/y	.\Engine\bin\release\Engine.dll	.\EffectEditor\bin\release\

xcopy	/y	.\Engine\bin\debug\Engine.lib	.\EngineSDK\Lib\debug\
xcopy	/y	.\Engine\bin\release\Engine.lib	.\EngineSDK\Lib\release\

xcopy	/y	.\Engine\bin\shaderfiles\*.*	.\Client\bin\shaderfiles\
xcopy	/y	.\Engine\bin\shaderfiles\*.*	.\Importer\bin\shaderfiles\
xcopy	/y	.\Engine\bin\shaderfiles\*.*	.\MapEditor\bin\shaderfiles\
xcopy	/y	.\Engine\bin\shaderfiles\*.*	.\EffectEditor\bin\shaderfiles\

pause