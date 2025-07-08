@echo off

echo Gizmod Install
echo Enter the path to your Pitfall installation (it should be "C:\Program Files (x86)\Aspyr\PITFALL The Lost Expedition" by default)
set /P PITFALL_PATH="> "

set MY_PATH=%~dp0

if not exist "%PITFALL_PATH%\Start.exe" (
	echo Path isn't valid. The install directory is the one containing "Start.exe"
	pause
	exit /b 1
)

echo Installing...

if not exist "%PITFALL_PATH%\Game\binkw32_o.dll" (
	rename "%PITFALL_PATH%\Game\binkw32.dll" binkw32_o.dll >NUL 2>&1
	if %ERRORLEVEL% NEQ 0 (echo Bink DLL rename failed. Did you run the script as admin? & pause & exit /b 1)
)

takeown /R /F "%MY_PATH%\*" >NUL 2>&1
if %ERRORLEVEL% NEQ 0 (echo Could not assign ownership to you. & pause & exit /b 1)

xcopy /E /I /Y /O "%MY_PATH%\" "%PITFALL_PATH%\Game\" >NUL 2>&1
if %ERRORLEVEL% NEQ 0 (echo Copy failed. & pause & exit /b 1)

del "%PITFALL_PATH%\Game\kbhook.dll" >NUL 2>&1

del "%PITFALL_PATH%\Game\install.bat" >NUL 2>&1

echo Install successful!

pause
