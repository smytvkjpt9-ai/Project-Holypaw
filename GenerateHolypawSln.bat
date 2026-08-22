@echo off
setlocal EnableExtensions
title Generate Holypaw Visual Studio files
cd /d "%~dp0"

echo.
echo Holypaw.sln is NOT in git. Unreal creates it on your PC.
echo On VS 2022 17.12+ you may get a WORKSPACE instead of .sln — that is OK.
echo Easiest path: double-click BuildHolypaw.bat (no .sln needed).
echo.

set "UE=%~1"
if "%UE%"=="" if exist "Tools\UE_PATH.txt" set /p UE=<"Tools\UE_PATH.txt"
if "%UE%"=="" if exist "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" set "UE=C:\Program Files\Epic Games\UE_5.8"
if "%UE%"=="" if exist "D:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" set "UE=D:\Program Files\Epic Games\UE_5.8"

set "SELECTOR="
if exist "%ProgramFiles(x86)%\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe" (
	set "SELECTOR=%ProgramFiles(x86)%\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"
)

if not "%SELECTOR%"=="" (
	echo Running UnrealVersionSelector...
	"%SELECTOR%" /projectfiles "%cd%\Holypaw.uproject"
) else if exist "%UE%\Engine\Build\BatchFiles\Build.bat" (
	echo Using engine: %UE%
	call "%UE%\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="%cd%\Holypaw.uproject" -game -engine
) else (
	echo Could not find Unreal 5.8. Install from Epic Launcher first.
	goto :done
)

echo.
echo --- Look for project files here ---
if exist "Holypaw.sln" (
	echo   FOUND: %cd%\Holypaw.sln
	explorer /select,"%cd%\Holypaw.sln"
) else if exist "Intermediate\ProjectFiles\Holypaw.sln" (
	echo   FOUND: %cd%\Intermediate\ProjectFiles\Holypaw.sln
	explorer /select,"%cd%\Intermediate\ProjectFiles\Holypaw.sln"
) else if exist "Holypaw.code-workspace" (
	echo   FOUND: Holypaw.code-workspace ^(VS Code / Cursor — use BuildHolypaw.bat^)
) else (
	echo   No Holypaw.sln in root or Intermediate\ProjectFiles\
	echo   If Visual Studio opened a folder workspace, use Build - Build All there.
	echo   Or skip .sln and run BuildHolypaw.bat
)

:done
echo.
pause
