#!/usr/bin/env bash
# Compile HolypawEditor and print the real UBT/UHT/C++ errors.
# The editor "missing module / rebuild now" dialog hides this log.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
UE="${1:-}"
if [[ -z "$UE" && -f "$ROOT/Tools/UE_PATH.txt" ]]; then
	UE="$(tr -d '\r\n' < "$ROOT/Tools/UE_PATH.txt")"
fi
if [[ -z "$UE" ]]; then
	for candidate in \
		"/Users/Shared/Epic Games/UE_5.8" \
		"$HOME/UE_5.8" \
		"/opt/UnrealEngine"
	do
		if [[ -x "$candidate/Engine/Build/BatchFiles/Mac/Build.sh" || -x "$candidate/Engine/Build/BatchFiles/Linux/Build.sh" ]]; then
			UE="$candidate"
			break
		fi
	done
fi

BUILD=""
TARGET_PLATFORM=""
if [[ -x "${UE:-}/Engine/Build/BatchFiles/Mac/Build.sh" ]]; then
	BUILD="$UE/Engine/Build/BatchFiles/Mac/Build.sh"
	TARGET_PLATFORM="Mac"
elif [[ -x "${UE:-}/Engine/Build/BatchFiles/Linux/Build.sh" ]]; then
	BUILD="$UE/Engine/Build/BatchFiles/Linux/Build.sh"
	TARGET_PLATFORM="Linux"
fi

if [[ -z "$BUILD" ]]; then
	echo "Could not find Unreal Engine 5.8."
	echo "Pass the engine folder:"
	echo "  Tools/BuildHolypaw.sh \"/Users/Shared/Epic Games/UE_5.8\""
	echo "Or write that path into Tools/UE_PATH.txt"
	echo
	echo "On Mac you need Xcode command-line tools. On Linux you need the"
	echo "engine's native toolchain. Without those, the editor rebuild fails"
	echo "with no error in the missing-module dialog."
	exit 1
fi

echo "Using engine: $UE"
echo "Project: $ROOT/Holypaw.uproject"
echo

set +e
"$BUILD" HolypawEditor "$TARGET_PLATFORM" Development -Project="$ROOT/Holypaw.uproject" -WaitMutex
ERR=$?
set -e

echo
if [[ "$ERR" -ne 0 ]]; then
	echo "BUILD FAILED. First error lines:"
	echo "--------------------------------"
	if [[ -f "$ROOT/Saved/Logs/Holypaw.log" ]]; then
		grep -iE 'error C|error :|Error:' "$ROOT/Saved/Logs/Holypaw.log" | head -n 40
	fi
	echo "--------------------------------"
	echo "Send the first error C / Error: line if you need another fix."
	exit "$ERR"
fi

echo "BUILD OK. Open Holypaw.uproject — the missing-module dialog should be gone."
exit 0
