@echo off

set MY_PATH=%~dp0

if not exist "%MY_PATH%\binkw32_o.dll" (
	echo Gizmod is not installed.
	pause
	exit /b 1
)

echo Uninstalling...

del "%MY_PATH%\binkw32.dll" >NUL 2>&1

rename "%MY_PATH%\binkw32_o.dll" binkw32.dll >NUL 2>&1
if %ERRORLEVEL% NEQ 0 (echo Bink DLL rename failed. Did you run the script as admin? & pause & exit /b 1)

pause
