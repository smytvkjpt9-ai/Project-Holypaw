@echo off
setlocal EnableExtensions
title Generate Holypaw.sln
cd /d "%~dp0"

echo Holypaw.sln is not in git. This script asks Unreal 5.8 to create it
echo in this folder, next to Holypaw.uproject.
echo.

set "UE=%~1"
if "%UE%"=="" (
	if exist "Tools\UE_PATH.txt" (
		set /p UE=<"Tools\UE_PATH.txt"
	)
)

if "%UE%"=="" (
	if exist "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" set "UE=C:\Program Files\Epic Games\UE_5.8"
)
if "%UE%"=="" (
	if exist "D:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" set "UE=D:\Program Files\Epic Games\UE_5.8"
)
if "%UE%"=="" (
	if exist "C:\UE_5.8\Engine\Build\BatchFiles\Build.bat" set "UE=C:\UE_5.8"
)

set "SELECTOR="
if exist "%ProgramFiles(x86)%\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe" (
	set "SELECTOR=%ProgramFiles(x86)%\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"
)
if "%SELECTOR%"=="" if exist "%UE%\Engine\Binaries\Win64\UnrealVersionSelector.exe" (
	set "SELECTOR=%UE%\Engine\Binaries\Win64\UnrealVersionSelector.exe"
)

if not "%SELECTOR%"=="" (
	echo Using UnrealVersionSelector...
	"%SELECTOR%" /projectfiles "%cd%\Holypaw.uproject"
	goto :after
)

if not exist "%UE%\Engine\Build\BatchFiles\Build.bat" (
	echo Could not find Unreal Engine 5.8.
	echo.
	echo Install UE 5.8 from the Epic Games Launcher, then either:
	echo   1. Right-click Holypaw.uproject - Generate Visual Studio project files
	echo   2. Or run this again after writing the engine folder into Tools\UE_PATH.txt
	echo      example: C:\Program Files\Epic Games\UE_5.8
	echo.
	pause
	exit /b 1
)

echo Using engine: %UE%
call "%UE%\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="%cd%\Holypaw.uproject" -game -engine

:after
if exist "Holypaw.sln" (
	echo.
	echo Created Holypaw.sln in:
	echo   %cd%
	echo.
	echo Next: open Holypaw.sln in Visual Studio 2022.
	echo Set the toolbar to Development Editor and Win64.
	echo Build - Build Solution.
	echo When that succeeds, open Holypaw.uproject.
	echo.
	explorer /select,"%cd%\Holypaw.sln"
) else (
	echo.
	echo Holypaw.sln was not created.
	echo Need Unreal Engine 5.8 AND Visual Studio 2022 "Game Development with C++".
	echo If right-click on Holypaw.uproject has no Generate option, open
	echo Holypaw.uproject with Unreal Version Selector once to associate it.
)

echo.
pause
exit /b 0
