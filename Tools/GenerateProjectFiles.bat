@echo off
setlocal EnableExtensions
rem Creates Holypaw.sln so you can compile HolypawEditor in Visual Studio.
rem Required before the editor can find the Holypaw module.

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
	echo   Tools\GenerateProjectFiles.bat "C:\Program Files\Epic Games\UE_5.8"
	popd >nul
	exit /b 1
)

echo Generating Visual Studio project files...
call "%UE%\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="%ROOT%\Holypaw.uproject" -game -engine
set "ERR=%ERRORLEVEL%"
if not "%ERR%"=="0" (
	echo Generate project files FAILED.
	echo Install Visual Studio 2022 workload "Game Development with C++".
	popd >nul
	exit /b %ERR%
)
echo.
echo Open Holypaw.sln, set Development Editor / Win64, build HolypawEditor.
echo Or run Tools\BuildHolypaw.bat
popd >nul
exit /b 0
