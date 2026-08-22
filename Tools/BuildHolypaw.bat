@echo off
setlocal EnableExtensions
title Build Holypaw
rem Compile HolypawEditor and print the real UBT/UHT/C++ errors.

set "ROOT=%~dp0.."
pushd "%ROOT%" >nul

set "LOG=%ROOT%\BuildHolypaw-last-run.txt"
echo Holypaw build started: %DATE% %TIME% > "%LOG%"

set "UE=%~1"
if "%UE%"=="" if exist "%~dp0UE_PATH.txt" set /p UE=<"%~dp0UE_PATH.txt"
if "%UE%"=="" if exist "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" set "UE=C:\Program Files\Epic Games\UE_5.8"
if "%UE%"=="" if exist "D:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" set "UE=D:\Program Files\Epic Games\UE_5.8"

if not exist "%UE%\Engine\Build\BatchFiles\Build.bat" (
	echo Could not find Unreal Engine 5.8.
	echo Install UE 5.8 from Epic Launcher, or run:
	echo   BuildHolypaw.bat "C:\Program Files\Epic Games\UE_5.8"
	echo Could not find Unreal Engine 5.8. >> "%LOG%"
	popd >nul
	echo Log: %LOG%
	pause
	exit /b 1
)

echo Using engine: %UE%
echo Project: %ROOT%\Holypaw.uproject
echo.
echo First build often takes 20-40 minutes. Leave this window open.
echo Using engine: %UE% >> "%LOG%"
echo Project: %ROOT%\Holypaw.uproject >> "%LOG%"
echo.

call "%UE%\Engine\Build\BatchFiles\Build.bat" HolypawEditor Win64 Development -Project="%ROOT%\Holypaw.uproject" -WaitMutex
set "ERR=%ERRORLEVEL%"

echo. >> "%LOG%"
echo Exit code: %ERR% >> "%LOG%"

echo.
if not "%ERR%"=="0" (
	echo ========================================
	echo BUILD FAILED ^(exit code %ERR%^)
	echo ========================================
	echo.
	if exist "%ROOT%\Saved\Logs\Holypaw.log" (
		echo First error lines from Saved\Logs\Holypaw.log:
		findstr /i /c:"error C" /c:"error :" /c:"Error:" "%ROOT%\Saved\Logs\Holypaw.log"
	)
	echo.
	echo Full build log: %LOG%
	echo BUILD FAILED >> "%LOG%"
) else (
	echo ========================================
	echo BUILD OK
	echo ========================================
	echo.
	echo Next: double-click Holypaw.uproject in:
	echo   %ROOT%
	echo.
	echo Then click Play in the editor.
	echo BUILD OK >> "%LOG%"
)

popd >nul
echo Log saved: %LOG%
echo.
echo Press any key to close this window...
pause >nul
exit /b %ERR%
