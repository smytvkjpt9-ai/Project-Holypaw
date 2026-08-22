#include "Audio/HolypawAudioInternal.h"

namespace HolypawScore
{
	static FHolypawScore Make(
		const float Bpm,
		const int32 Root,
		const EHolypawScale Scale,
		const TArray<int32>& Chords,
		const TArray<int32>& Motif,
		const float Pad,
		const float Choir,
		const float Bell,
		const float Bass,
		const float Arp,
		const float Bright,
		const float Bed,
		const EHolypawBed Kind)
	{
		FHolypawScore S;
		S.Bpm = Bpm;
		S.RootMidi = Root;
		S.Scale = Scale;
		S.ChordCount = FMath::Clamp(Chords.Num(), 1, 8);
		for (int32 I = 0; I < S.ChordCount; ++I)
		{
			S.Chord[I] = Chords[I];
		}
		for (int32 I = 0; I < 16; ++I)
		{
			S.Motif[I] = Motif.IsValidIndex(I) ? Motif[I] : -1;
		}
		S.Pad = Pad;
		S.Choir = Choir;
		S.Bell = Bell;
		S.Bass = Bass;
		S.Arp = Arp;
		S.Bright = Bright;
		S.Bed = Bed;
		S.BedKind = Kind;
		return S;
	}

	FHolypawScore Title()
	{
		return Make(64.f, 53, EHolypawScale::Lydian,
			{0, 4, 5, 3},
			{4, 2, 0, -1, 5, 4, 2, 0, 7, 5, 4, 2, 4, -1, 0, -1},
			0.58f, 0.34f, 0.28f, 0.22f, 0.08f, 0.30f, 0.12f, EHolypawBed::Chapel);
	}

	FHolypawScore Victory()
	{
		return Make(96.f, 60, EHolypawScale::Major,
			{0, 4, 5, 0},
			{0, 2, 4, 7, 4, 2, 0, -1, 4, 5, 7, 9, 7, 4, 0, -1},
			0.40f, 0.22f, 0.42f, 0.24f, 0.16f, 0.45f, 0.06f, EHolypawBed::Town);
	}

	FHolypawScore ForZone(const EHolypawZone Zone, const bool bInterior, const EHolypawDayPart Day)
	{
		FHolypawScore S;
		switch (Zone)
		{
		case EHolypawZone::ForestCottage:
			S = Make(68.f, 55, EHolypawScale::MajorPent,
				{0, 3, 4, 0},
				{0, 2, 4, -1, 2, 0, -1, 4, 5, 4, 2, 0, -1, 2, 0, -1},
				0.56f, 0.18f, 0.22f, 0.24f, 0.06f, 0.28f, 0.28f, EHolypawBed::Cottage);
			break;
		case EHolypawZone::NurseryHills:
			S = Make(74.f, 57, EHolypawScale::Major,
				{0, 4, 1, 5},
				{2, 4, 5, 4, 2, -1, 0, 2, 4, -1, 5, 4, 2, 0, -1, -1},
				0.50f, 0.16f, 0.24f, 0.22f, 0.10f, 0.32f, 0.22f, EHolypawBed::Forest);
			break;
		case EHolypawZone::RibbonCity:
			S = Make(86.f, 58, EHolypawScale::Mixolydian,
				{0, 5, 3, 4},
				{0, 4, 2, 5, -1, 4, 0, 2, 7, 5, 4, -1, 2, 4, 0, -1},
				0.48f, 0.14f, 0.20f, 0.28f, 0.14f, 0.40f, 0.18f, EHolypawBed::Town);
			break;
		case EHolypawZone::Tidewell:
			S = Make(70.f, 50, EHolypawScale::Dorian,
				{0, 5, 3, 4, 0, 3, 5, 0},
				{4, -1, 2, 0, 5, 4, -1, 2, 0, 2, 4, -1, 7, 5, 4, -1},
				0.54f, 0.20f, 0.12f, 0.30f, 0.08f, 0.26f, 0.32f, EHolypawBed::Harbor);
			break;
		case EHolypawZone::Hearthfold:
			S = Make(76.f, 53, EHolypawScale::Major,
				{0, 3, 4, 0},
				{0, 2, 4, 2, 0, -1, 5, 4, 2, 4, 0, -1, 2, 0, -1, -1},
				0.52f, 0.18f, 0.16f, 0.26f, 0.07f, 0.30f, 0.16f, EHolypawBed::Town);
			break;
		case EHolypawZone::Emberfen:
			S = Make(62.f, 46, EHolypawScale::Minor,
				{0, 5, 3, 6},
				{0, -1, 3, 2, 0, 5, -1, 3, 7, 5, 3, -1, 2, 0, -1, 3},
				0.58f, 0.24f, 0.08f, 0.28f, 0.05f, 0.22f, 0.30f, EHolypawBed::Mire);
			break;
		case EHolypawZone::Snowveil:
			S = Make(60.f, 64, EHolypawScale::Lydian,
				{0, 4, 1, 5},
				{4, 5, 7, -1, 5, 4, 2, -1, 7, 9, 7, 5, 4, -1, 2, -1},
				0.60f, 0.30f, 0.18f, 0.18f, 0.09f, 0.24f, 0.26f, EHolypawBed::Snow);
			break;
		case EHolypawZone::Homestead:
			S = Make(72.f, 55, EHolypawScale::MajorPent,
				{0, 4, 3, 0},
				{0, 2, 4, 0, -1, 2, 4, 5, 4, 2, 0, -1, 4, 2, 0, -1},
				0.50f, 0.12f, 0.20f, 0.24f, 0.08f, 0.30f, 0.18f, EHolypawBed::Cottage);
			break;
		case EHolypawZone::Coast:
			S = Make(78.f, 52, EHolypawScale::Mixolydian,
				{0, 4, 5, 2},
				{2, 4, -1, 5, 4, 2, 0, -1, 5, 7, 5, 4, -1, 2, 0, -1},
				0.48f, 0.16f, 0.14f, 0.26f, 0.11f, 0.34f, 0.30f, EHolypawBed::Harbor);
			break;
		case EHolypawZone::Mire:
			S = Make(58.f, 45, EHolypawScale::HarmonicMinor,
				{0, 6, 5, 3},
				{0, 3, -1, 6, 5, 3, 0, -1, 7, 6, 5, -1, 3, 0, -1, 2},
				0.62f, 0.22f, 0.06f, 0.30f, 0.04f, 0.18f, 0.34f, EHolypawBed::Mire);
			break;
		case EHolypawZone::Highlands:
			S = Make(66.f, 48, EHolypawScale::MinorPent,
				{0, 3, 4, 0, 5, 3, 4, 0},
				{0, 2, 4, 7, -1, 4, 2, 0, 7, 4, 2, -1, 4, 2, 0, -1},
				0.64f, 0.36f, 0.12f, 0.22f, 0.05f, 0.20f, 0.22f, EHolypawBed::Peak);
			break;
		case EHolypawZone::Snow:
			S = Make(56.f, 62, EHolypawScale::MajorPent,
				{0, 4, 5, 4},
				{4, -1, 2, 4, 7, 5, -1, 4, 2, 0, -1, 4, 5, 4, 2, -1},
				0.58f, 0.28f, 0.16f, 0.16f, 0.07f, 0.22f, 0.28f, EHolypawBed::Snow);
			break;
		case EHolypawZone::LanternAngeles:
			S = Make(92.f, 61, EHolypawScale::Mixolydian,
				{0, 5, 7, 4},
				{0, 2, 4, 5, 7, -1, 5, 4, 2, 0, -1, 4, 7, 5, 4, -1},
				0.44f, 0.12f, 0.26f, 0.30f, 0.18f, 0.48f, 0.14f, EHolypawBed::Town);
			break;
		case EHolypawZone::Mossgate:
			S = Make(70.f, 54, EHolypawScale::Dorian,
				{0, 3, 5, 4},
				{0, 2, -1, 3, 5, 3, 2, 0, 4, -1, 5, 3, 2, 0, -1, -1},
				0.54f, 0.20f, 0.14f, 0.24f, 0.08f, 0.28f, 0.26f, EHolypawBed::Forest);
			break;
		case EHolypawZone::Quiltland:
			S = Make(80.f, 57, EHolypawScale::Major,
				{0, 4, 3, 5},
				{4, 2, 0, 2, 4, 5, -1, 4, 2, 0, -1, 5, 4, 2, 0, -1},
				0.50f, 0.16f, 0.22f, 0.24f, 0.10f, 0.34f, 0.14f, EHolypawBed::Town);
			break;
		case EHolypawZone::DustMesa:
			S = Make(84.f, 50, EHolypawScale::Mixolydian,
				{0, 5, 0, 4},
				{0, -1, 4, 5, 4, 0, -1, 2, 4, 7, 5, 4, -1, 0, 4, -1},
				0.42f, 0.08f, 0.12f, 0.32f, 0.06f, 0.36f, 0.28f, EHolypawBed::Desert);
			break;
		case EHolypawZone::PalmaDusk:
			S = Make(88.f, 56, EHolypawScale::Dorian,
				{0, 4, 5, 3},
				{5, 4, 2, -1, 0, 2, 4, 5, 7, -1, 5, 4, 2, 0, -1, 4},
				0.46f, 0.14f, 0.18f, 0.26f, 0.12f, 0.38f, 0.22f, EHolypawBed::Jungle);
			break;
		case EHolypawZone::IvorySpire:
			S = Make(64.f, 59, EHolypawScale::Lydian,
				{0, 1, 4, 5},
				{4, 6, 7, 4, -1, 2, 4, 7, 9, 7, 6, 4, -1, 2, 0, -1},
				0.56f, 0.32f, 0.20f, 0.18f, 0.09f, 0.26f, 0.12f, EHolypawBed::Peak);
			break;
		case EHolypawZone::SandHymn:
			S = Make(72.f, 51, EHolypawScale::HarmonicMinor,
				{0, 5, 6, 3},
				{0, 3, 5, -1, 6, 5, 3, 0, 7, -1, 5, 3, 0, 2, -1, 0},
				0.50f, 0.26f, 0.16f, 0.24f, 0.08f, 0.30f, 0.24f, EHolypawBed::Desert);
			break;
		case EHolypawZone::CapePlush:
			S = Make(76.f, 52, EHolypawScale::MajorPent,
				{0, 4, 2, 5},
				{0, 2, 4, 7, 4, 2, -1, 0, 5, 4, 2, -1, 4, 2, 0, -1},
				0.48f, 0.18f, 0.14f, 0.28f, 0.10f, 0.32f, 0.34f, EHolypawBed::Harbor);
			break;
		case EHolypawZone::CherryLoom:
			S = Make(78.f, 61, EHolypawScale::MajorPent,
				{0, 3, 4, 0, 5, 3, 4, 2},
				{4, 5, 7, 4, 2, -1, 0, 2, 4, -1, 7, 5, 4, 2, 0, -1},
				0.46f, 0.20f, 0.30f, 0.20f, 0.16f, 0.36f, 0.12f, EHolypawBed::Town);
			break;
		case EHolypawZone::AuroraBorough:
			S = Make(58.f, 65, EHolypawScale::Lydian,
				{0, 4, 6, 3},
				{7, 6, 4, -1, 9, 7, 6, 4, 2, -1, 4, 6, 7, 4, -1, 0},
				0.62f, 0.38f, 0.14f, 0.16f, 0.06f, 0.20f, 0.24f, EHolypawBed::Aurora);
			break;
		case EHolypawZone::TundraParish:
			S = Make(54.f, 53, EHolypawScale::Minor,
				{0, 5, 3, 4},
				{0, 2, 3, -1, 5, 3, 2, 0, 7, 5, -1, 3, 2, 0, -1, -1},
				0.60f, 0.28f, 0.10f, 0.22f, 0.05f, 0.18f, 0.30f, EHolypawBed::Snow);
			break;
		case EHolypawZone::Desert:
			S = Make(82.f, 49, EHolypawScale::Mixolydian,
				{0, 4, 0, 5},
				{0, 4, -1, 5, 4, 0, 2, -1, 4, 7, 5, 4, 0, -1, 4, -1},
				0.40f, 0.06f, 0.10f, 0.30f, 0.05f, 0.38f, 0.32f, EHolypawBed::Desert);
			break;
		case EHolypawZone::Jungle:
			S = Make(90.f, 55, EHolypawScale::Dorian,
				{0, 3, 5, 4, 0, 5, 3, 0},
				{0, 2, 3, 5, -1, 3, 2, 0, 7, 5, 3, -1, 5, 3, 0, -1},
				0.44f, 0.12f, 0.16f, 0.26f, 0.14f, 0.42f, 0.28f, EHolypawBed::Jungle);
			break;
		case EHolypawZone::Ocean:
			S = Make(52.f, 47, EHolypawScale::MinorPent,
				{0, 3, 4, 0},
				{0, -1, 2, 4, 2, 0, -1, 4, 5, 4, 2, -1, 0, 2, -1, -1},
				0.66f, 0.24f, 0.08f, 0.32f, 0.04f, 0.16f, 0.42f, EHolypawBed::Ocean);
			break;
		case EHolypawZone::Ice:
			S = Make(50.f, 66, EHolypawScale::MajorPent,
				{0, 4, 5, 4},
				{4, 7, 9, 7, 4, -1, 2, 4, 7, -1, 5, 4, 2, -1, 0, -1},
				0.58f, 0.26f, 0.18f, 0.14f, 0.08f, 0.20f, 0.30f, EHolypawBed::Snow);
			break;
		case EHolypawZone::CarnivalBahia:
			S = Make(108.f, 57, EHolypawScale::Mixolydian,
				{0, 4, 5, 3, 0, 5, 4, 0},
				{0, 2, 4, 5, 4, 2, 0, 4, 7, 5, 4, 2, 0, -1, 4, -1},
				0.38f, 0.10f, 0.22f, 0.32f, 0.22f, 0.50f, 0.16f, EHolypawBed::Carnival);
			break;
		case EHolypawZone::AndesLoom:
			S = Make(68.f, 50, EHolypawScale::MinorPent,
				{0, 3, 5, 0},
				{0, 2, 4, -1, 5, 4, 2, 0, 7, -1, 4, 2, 0, 2, -1, 0},
				0.52f, 0.18f, 0.12f, 0.28f, 0.07f, 0.28f, 0.20f, EHolypawBed::Peak);
			break;
		case EHolypawZone::Clockhaven:
			S = Make(96.f, 54, EHolypawScale::Major,
				{0, 4, 1, 5, 0, 3, 4, 0},
				{0, 4, 2, 0, 5, 4, 2, -1, 7, 5, 4, 2, 0, 4, -1, 0},
				0.42f, 0.12f, 0.24f, 0.28f, 0.20f, 0.36f, 0.10f, EHolypawBed::Clock);
			break;
		case EHolypawZone::VelvetSeine:
			S = Make(74.f, 56, EHolypawScale::Dorian,
				{0, 4, 5, 3},
				{4, 2, 0, 5, 4, -1, 2, 4, 7, 5, 4, 2, -1, 0, 2, -1},
				0.50f, 0.22f, 0.18f, 0.22f, 0.11f, 0.32f, 0.12f, EHolypawBed::Town);
			break;
		case EHolypawZone::MarbleForum:
			S = Make(70.f, 53, EHolypawScale::Lydian,
				{0, 4, 5, 1},
				{4, 6, 7, 4, 2, -1, 0, 4, 7, 9, 7, 4, -1, 2, 0, -1},
				0.54f, 0.28f, 0.16f, 0.20f, 0.08f, 0.28f, 0.10f, EHolypawBed::Chapel);
			break;
		case EHolypawZone::SavannahBell:
			S = Make(84.f, 55, EHolypawScale::MajorPent,
				{0, 4, 5, 0},
				{0, 2, 4, -1, 4, 5, 7, 4, 2, 0, -1, 4, 2, 0, -1, 2},
				0.46f, 0.14f, 0.26f, 0.26f, 0.10f, 0.34f, 0.22f, EHolypawBed::Desert);
			break;
		case EHolypawZone::SilkDelta:
			S = Make(76.f, 58, EHolypawScale::Major,
				{0, 5, 3, 4},
				{2, 4, 5, 4, 2, 0, -1, 4, 7, 5, 4, -1, 2, 4, 0, -1},
				0.48f, 0.18f, 0.16f, 0.24f, 0.12f, 0.32f, 0.20f, EHolypawBed::Harbor);
			break;
		case EHolypawZone::SpiceHarbor:
			S = Make(88.f, 54, EHolypawScale::Mixolydian,
				{0, 3, 4, 5},
				{0, 3, 5, 4, -1, 5, 3, 0, 7, 5, 4, 3, 0, -1, 4, -1},
				0.44f, 0.12f, 0.20f, 0.30f, 0.15f, 0.40f, 0.24f, EHolypawBed::Harbor);
			break;
		case EHolypawZone::CoralChoir:
			S = Make(64.f, 59, EHolypawScale::Lydian,
				{0, 3, 4, 6},
				{4, 6, 7, 4, -1, 2, 4, 6, 9, 7, 6, 4, -1, 4, 0, -1},
				0.56f, 0.34f, 0.14f, 0.22f, 0.07f, 0.24f, 0.36f, EHolypawBed::Ocean);
			break;
		case EHolypawZone::FeltIceCamp:
			S = Make(48.f, 51, EHolypawScale::MinorPent,
				{0, 3, 4, 0},
				{0, -1, 2, 4, 2, 0, -1, 3, 4, 2, 0, -1, 2, 0, -1, -1},
				0.60f, 0.20f, 0.10f, 0.26f, 0.04f, 0.16f, 0.28f, EHolypawBed::Snow);
			break;
		default:
			S = Make(72.f, 57, EHolypawScale::MajorPent,
				{0, 4, 5, 3},
				{0, 2, 4, -1, 4, 2, 0, -1, 5, 4, 2, 0, 2, -1, 0, -1},
				0.50f, 0.18f, 0.16f, 0.24f, 0.08f, 0.30f, 0.20f, EHolypawBed::Forest);
			break;
		}

		if (bInterior)
		{
			S.Pad *= 0.86f;
			S.Bed *= 0.45f;
			S.Bright *= 0.72f;
			S.Arp *= 0.65f;
			S.Choir *= 1.15f;
			if (S.BedKind == EHolypawBed::Town || S.BedKind == EHolypawBed::Cottage)
			{
				S.BedKind = EHolypawBed::Chapel;
			}
		}
		if (Day == EHolypawDayPart::Night)
		{
			S.Bpm *= 0.92f;
			S.Bell *= 0.55f;
			S.Choir *= 1.25f;
			S.Bright *= 0.7f;
			S.Pad *= 1.08f;
		}
		else if (Day == EHolypawDayPart::Dusk)
		{
			S.Choir *= 1.18f;
			S.Bell *= 1.12f;
			S.Bpm *= 0.97f;
		}
		else if (Day == EHolypawDayPart::Morning)
		{
			S.Bell *= 1.2f;
			S.Arp *= 1.15f;
		}
		return S;
	}

	FHolypawScore Combat(const EHolypawZone Zone, const bool bBoss, const bool bPhaseTwo)
	{
		FHolypawScore S = ForZone(Zone, false, EHolypawDayPart::Day);
		S.Bpm = bBoss ? (bPhaseTwo ? 132.f : 118.f) : 108.f;
		S.Scale = bBoss ? EHolypawScale::HarmonicMinor : EHolypawScale::Minor;
		S.Pad *= 0.72f;
		S.Bell *= 0.35f;
		S.Choir *= 0.55f;
		S.Bass *= 1.45f;
		S.Arp = bBoss ? 0.22f : 0.16f;
		S.Pulse = bBoss ? (bPhaseTwo ? 0.85f : 0.70f) : 0.52f;
		S.Bright = FMath::Clamp(S.Bright + 0.18f, 0.f, 0.7f);
		S.Bed *= 0.35f;
		S.Chord[0] = 0;
		S.Chord[1] = 6;
		S.Chord[2] = 5;
		S.Chord[3] = 3;
		S.ChordCount = 4;
		const int32 Ostinato[16] = {0, 0, 2, -1, 0, 3, 0, -1, 0, 2, 3, 0, 5, 3, 0, -1};
		for (int32 I = 0; I < 16; ++I)
		{
			S.Motif[I] = Ostinato[I];
		}
		return S;
	}

	static void RenderBedKind(const EHolypawBed Kind, const int32 I, const float T, uint32& Rng,
		HolypawSynth::FOnePole& Air, float& L, float& R, const float Gain)
	{
		using namespace HolypawSynth;
		const float N = White(Rng);
		float Cut = 900.f;
		float Amp = 0.18f;
		switch (Kind)
		{
		case EHolypawBed::Forest:
			Cut = 1800.f + 400.f * FastSin(T * 0.17f);
			Amp = 0.16f + 0.04f * FastSin(T * 0.11f);
			if ((HashU(static_cast<uint32>(T * 1.4f) + 17) & 255) < 3)
			{
				Amp += 0.08f * ExpDecay(FMath::Fmod(T * 1.4f, 1.f), 8.f);
			}
			break;
		case EHolypawBed::Town:
			Cut = 2400.f;
			Amp = 0.10f + 0.03f * FastSin(T * 0.4f);
			break;
		case EHolypawBed::Harbor:
			Cut = 700.f + 500.f * FastSin(T * 0.09f);
			Amp = 0.22f + 0.06f * FastSin(T * 0.07f);
			break;
		case EHolypawBed::Mill:
			Cut = 420.f;
			Amp = 0.28f;
			L += OscWarm(55.f * T, 0.5f) * 0.12f * Gain;
			R += OscWarm(55.3f * T, 0.5f) * 0.12f * Gain;
			break;
		case EHolypawBed::Snow:
			Cut = 3200.f;
			Amp = 0.14f;
			break;
		case EHolypawBed::Desert:
			Cut = 1100.f;
			Amp = 0.18f + 0.05f * FastSin(T * 0.05f);
			break;
		case EHolypawBed::Jungle:
			Cut = 2600.f;
			Amp = 0.17f;
			if ((HashU(static_cast<uint32>(T * 2.2f) + 9) & 255) < 4)
			{
				Amp += 0.1f * ExpDecay(FMath::Fmod(T * 2.2f, 1.f), 10.f);
			}
			break;
		case EHolypawBed::Ocean:
			Cut = 600.f + 700.f * FastSin(T * 0.06f);
			Amp = 0.30f;
			break;
		case EHolypawBed::Chapel:
			Cut = 1600.f;
			Amp = 0.08f;
			break;
		case EHolypawBed::Cottage:
			Cut = 1400.f;
			Amp = 0.10f;
			break;
		case EHolypawBed::Carnival:
			Cut = 2800.f;
			Amp = 0.12f + 0.04f * FastSin(T * 2.f);
			break;
		case EHolypawBed::Peak:
			Cut = 2000.f;
			Amp = 0.20f;
			break;
		case EHolypawBed::Mire:
			Cut = 500.f;
			Amp = 0.22f;
			break;
		case EHolypawBed::Aurora:
			Cut = 2200.f;
			Amp = 0.12f;
			L += OscSine(220.f * T) * 0.04f * Gain * FastSin(T * 0.13f);
			R += OscSine(277.f * T) * 0.04f * Gain * FastCos(T * 0.11f);
			break;
		case EHolypawBed::Clock:
			Cut = 1800.f;
			Amp = 0.08f;
			{
				const float Tick = FMath::Fmod(T, 1.f);
				if (Tick < 0.03f)
				{
					Amp += 0.2f * (1.f - Tick / 0.03f);
				}
			}
			break;
		default:
			break;
		}
		const float Filt = Air.Tick(N, OnePoleCoeff(Cut));
		const float G = Amp * Gain;
		const float Pan = 0.15f * FastSin(T * 0.07f + static_cast<float>(I) * 0.00001f);
		L += Filt * G * (0.7f - Pan);
		R += Filt * G * (0.7f + Pan);
	}

	void RenderBed(const EHolypawBed Kind, const float Seconds, const float Gain, HolypawSynth::FStereo& Out)
	{
		using namespace HolypawSynth;
		InitLut();
		const int32 N = FMath::Max(64, FMath::RoundToInt(Seconds * SampleRate));
		Out.Zero(N);
		FOnePole Air;
		uint32 Rng = 0xBEEFu + static_cast<uint32>(Kind) * 17u;
		for (int32 I = 0; I < N; ++I)
		{
			const float T = static_cast<float>(I) / static_cast<float>(SampleRate);
			const float Fade = FMath::Min(T / 0.2f, FMath::Min(1.f, (Seconds - T) / 0.2f));
			float L = 0.f;
			float R = 0.f;
			RenderBedKind(Kind, I, T, Rng, Air, L, R, Gain * Fade);
			Out.L[I] = L;
			Out.R[I] = R;
		}
		AddDelayWidth(Out, 420, 0.12f);
		SoftLimit(Out);
	}

	void RenderMill(const float Seconds, HolypawSynth::FStereo& Out)
	{
		RenderBed(EHolypawBed::Mill, Seconds, 1.f, Out);
		using namespace HolypawSynth;
		const int32 N = Out.Num();
		for (int32 I = 0; I < N; ++I)
		{
			const float T = static_cast<float>(I) / static_cast<float>(SampleRate);
			const float Loom = OscWarm(36.f * T, 0.7f) * 0.18f + OscSine(72.1f * T) * 0.08f;
			const float Clack = ExpDecay(FMath::Fmod(T * 3.2f, 1.f), 14.f) * OscSine(180.f * T) * 0.07f;
			Out.L[I] += Loom * 0.9f + Clack;
			Out.R[I] += Loom * 1.05f + Clack * 0.7f;
		}
		SoftLimit(Out);
	}

	void Render(const FHolypawScore& Score, const float Seconds, const bool bCombat, HolypawSynth::FStereo& Out)
	{
		using namespace HolypawSynth;
		InitLut();
		const int32 N = FMath::Max(64, FMath::RoundToInt(Seconds * SampleRate));
		Out.Zero(N);
		const float Beat = 60.f / FMath::Max(40.f, Score.Bpm);
		const float Bar = Beat * 4.f;
		FOnePole PadL;
		FOnePole PadR;
		FOnePole Air;
		uint32 Rng = 0x51A3u * static_cast<uint32>(Score.RootMidi + 11);
		const float Cut = (bCombat ? 4200.f : 2800.f) * FMath::Clamp(0.45f + Score.Bright, 0.3f, 1.2f);
		const float Coeff = OnePoleCoeff(Cut);

		for (int32 I = 0; I < N; ++I)
		{
			const float T = static_cast<float>(I) / static_cast<float>(SampleRate);
			const float Fade = FMath::Min(T / 0.14f, FMath::Min(1.f, (Seconds - T) / 0.16f));
			const int32 BarI = FMath::FloorToInt(T / Bar);
			const int32 ChordDeg = Score.Chord[((BarI % Score.ChordCount) + Score.ChordCount) % Score.ChordCount];
			const float F0 = NoteHz(Score.RootMidi, Score.Scale, ChordDeg, 0);
			const float F3 = NoteHz(Score.RootMidi, Score.Scale, ChordDeg + 2, 0);
			const float F5 = NoteHz(Score.RootMidi, Score.Scale, ChordDeg + 4, 0);
			const float Trem = 0.94f + 0.06f * FastSin(T * 0.23f * 2.f * PI);
			float L = 0.f;
			float R = 0.f;

			const float PadG = Score.Pad * Trem * Fade;
			L += OscWarm(F0 * T, Score.Bright) * PadG * 0.38f;
			R += OscWarm(F0 * 1.002f * T, Score.Bright) * PadG * 0.38f;
			L += OscWarm(F3 * 0.998f * T, Score.Bright * 0.85f) * PadG * 0.28f;
			R += OscWarm(F3 * T, Score.Bright * 0.85f) * PadG * 0.28f;
			L += OscSine(F5 * 0.5f * T) * PadG * 0.18f;
			R += OscSine(F5 * 0.501f * T) * PadG * 0.18f;

			const float ChoirG = Score.Choir * Fade * (0.85f + 0.15f * FastSin(T * 0.09f));
			L += OscSine(F0 * 2.f * T) * ChoirG * 0.16f;
			R += OscSine(F3 * 2.003f * T) * ChoirG * 0.16f;
			L += OscSine(F5 * 2.f * T) * ChoirG * 0.12f;
			R += OscSine(F0 * 3.f * T) * ChoirG * 0.08f;

			const float BassHz = F0 * 0.5f;
			const float BassEnv = Score.Bass * Fade * (0.7f + 0.3f * ExpDecay(FMath::Fmod(T, Beat), 3.5f));
			L += OscSine(BassHz * T) * BassEnv * 0.55f;
			R += OscSine(BassHz * 1.001f * T) * BassEnv * 0.55f;
			L += OscSine(BassHz * 0.5f * T) * BassEnv * 0.22f;
			R += OscSine(BassHz * 0.5f * T) * BassEnv * 0.22f;

			const float Step = Beat * 0.5f;
			const int32 StepI = FMath::FloorToInt(T / Step);
			const int32 Mot = Score.Motif[((StepI % 16) + 16) % 16];
			if (Mot >= 0)
			{
				const float MT = T - static_cast<float>(StepI) * Step;
				const float MHz = NoteHz(Score.RootMidi, Score.Scale, Mot, bCombat ? 0 : 1);
				const float MG = (bCombat ? Score.Arp * 1.4f : Score.Bell) * EnvADSR(MT, Step * 0.95f, 0.01f, 0.08f, 0.35f, 0.12f) * Fade;
				const float Bell = OscBell(MHz, MT, bCombat ? 0.8f : 1.6f);
				const float Pan = (Mot % 3 == 0) ? -0.2f : 0.2f;
				L += Bell * MG * (0.75f - Pan);
				R += Bell * MG * (0.75f + Pan);
			}

			if (Score.Arp > 0.01f)
			{
				const float Sixteenth = Beat * 0.25f;
				const int32 AStep = FMath::FloorToInt(T / Sixteenth);
				const int32 ArpDeg = ChordDeg + (AStep % 3) * 2;
				const float AT = T - static_cast<float>(AStep) * Sixteenth;
				const float AHz = NoteHz(Score.RootMidi, Score.Scale, ArpDeg, 1);
				const float AG = Score.Arp * ExpDecay(AT, 18.f) * Fade * (bCombat ? 1.1f : 0.7f);
				L += OscTri(AHz * T) * AG * 0.22f;
				R += OscTri(AHz * 1.004f * T) * AG * 0.22f;
			}

			if (Score.Pulse > 0.01f || bCombat)
			{
				const float Pulse = FMath::Max(Score.Pulse, bCombat ? 0.45f : 0.f);
				const float BT = FMath::Fmod(T, Beat);
				const int32 BeatI = FMath::FloorToInt(T / Beat);
				if ((BeatI % 2) == 0)
				{
					const float KHz = FMath::Lerp(150.f, 42.f, FMath::Clamp(BT / 0.09f, 0.f, 1.f));
					const float Kick = OscSine(KHz * BT) * ExpDecay(BT, 14.f) * Pulse;
					L += Kick * 0.7f;
					R += Kick * 0.7f;
				}
				const float Hat = White(Rng) * ExpDecay(FMath::Fmod(T, Beat * 0.5f), 48.f) * Pulse * 0.12f;
				L += Hat * 0.7f;
				R += Hat;
				if (bCombat && (BeatI % 4) == 2)
				{
					const float Sn = White(Rng) * ExpDecay(BT, 22.f) * Pulse * 0.35f + OscSine(180.f * BT) * ExpDecay(BT, 16.f) * Pulse * 0.2f;
					L += Sn * 0.8f;
					R += Sn;
				}
			}

			RenderBedKind(Score.BedKind, I, T, Rng, Air, L, R, Score.Bed * Fade);

			Out.L[I] = PadL.Tick(L, Coeff);
			Out.R[I] = PadR.Tick(R, Coeff);
		}
		AddDelayWidth(Out, bCombat ? 180 : 520, bCombat ? 0.08f : 0.16f);
		SoftLimit(Out);
	}
}
