#pragma once

#include "CoreMinimal.h"
#include "HolypawTypes.h"

/** Mixer buses. WAV overrides still ride these faders. */
enum class EHolypawBus : uint8
{
	Master,
	Music,
	Ambient,
	Sfx,
	Ui,
	Combat,
	Foley
};

enum class EHolypawStem : uint8
{
	Silence,
	Title,
	Explore,
	Combat,
	Victory
};

enum class EHolypawScale : uint8
{
	Major,
	Minor,
	Dorian,
	Mixolydian,
	Lydian,
	MajorPent,
	MinorPent,
	HarmonicMinor
};

enum class EHolypawBed : uint8
{
	Forest,
	Town,
	Harbor,
	Mill,
	Snow,
	Desert,
	Jungle,
	Ocean,
	Chapel,
	Cottage,
	Carnival,
	Peak,
	Mire,
	Aurora,
	Clock
};

enum class EHolypawDayPart : uint8
{
	Morning,
	Day,
	Dusk,
	Night
};

struct FHolypawCueDef
{
	FName Id = NAME_None;
	EHolypawBus Bus = EHolypawBus::Sfx;
	float Volume = 0.7f;
	float Seconds = 0.2f;
	int32 VaryCents = 40;
	bool bTEMP = true;
	const TCHAR* AssetPath = TEXT("");
};

struct FHolypawScore
{
	float Bpm = 72.f;
	int32 RootMidi = 57;
	EHolypawScale Scale = EHolypawScale::MajorPent;
	int32 Chord[8] = {0, 4, 5, 3, 0, 4, 3, 5};
	int32 ChordCount = 4;
	int32 Motif[16] = {0, 2, 4, -1, 4, 2, 0, -1, 5, 4, 2, 0, 2, -1, 0, -1};
	float Pad = 0.52f;
	float Choir = 0.20f;
	float Bell = 0.16f;
	float Bass = 0.26f;
	float Arp = 0.10f;
	float Pulse = 0.f;
	float Bright = 0.34f;
	float Bed = 0.20f;
	EHolypawBed BedKind = EHolypawBed::Forest;
};

struct FHolypawLoopCache
{
	TArray<uint8> Pcm;
	float Seconds = 8.f;
};
