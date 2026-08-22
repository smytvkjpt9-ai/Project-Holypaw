@echo off
setlocal EnableExtensions
rem Compile HolypawEditor and print the real UBT/UHT/C++ errors.
rem The editor "missing module / rebuild now" dialog hides this log.

set "ROOT=%~dp0.."
pushd "%ROOT%" >nul

set "UE=%~1"
if "%UE%"=="" (
	if exist "%~dp0UE_PATH.txt" (
		set /p UE=<"%~dp0UE_PATH.txt"
	)
)
if "%UE%"=="" (
	if exist "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" set "UE=C:\Program Files\Epic Games\UE_5.8"
)
if "%UE%"=="" (
	if exist "D:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" set "UE=D:\Program Files\Epic Games\UE_5.8"
)

if not exist "%UE%\Engine\Build\BatchFiles\Build.bat" (
	echo Could not find Unreal Engine 5.8.
	echo Pass the engine folder:
	echo   Tools\BuildHolypaw.bat "C:\Program Files\Epic Games\UE_5.8"
	echo Or write that path into Tools\UE_PATH.txt
	echo.
	echo Also required: Visual Studio 2022 workload "Game Development with C++"
	echo plus the Windows 10/11 SDK. Without those, the editor rebuild fails
	echo with no error in the "missing module" dialog.
	popd >nul
	exit /b 1
)

echo Using engine: %UE%
echo Project: %ROOT%\Holypaw.uproject
echo.

call "%UE%\Engine\Build\BatchFiles\Build.bat" HolypawEditor Win64 Development -Project="%ROOT%\Holypaw.uproject" -WaitMutex
set "ERR=%ERRORLEVEL%"

echo.
if not "%ERR%"=="0" (
	echo BUILD FAILED. First error lines:
	echo --------------------------------
	if exist "Saved\Logs\Holypaw.log" findstr /i /c:"error C" /c:"error :" /c:"Error:" "Saved\Logs\Holypaw.log"
	echo --------------------------------
	echo Full log: Saved\Logs  or the UnrealBuildTool window above.
	echo Send the first "error C" / "Error:" line if you need another fix.
) else (
	echo BUILD OK. Open Holypaw.uproject — the missing-module dialog should be gone.
)

popd >nul
exit /b %ERR%
