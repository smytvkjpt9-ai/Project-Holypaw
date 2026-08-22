#include "Audio/HolypawAudioInternal.h"

namespace HolypawSfx
{
	using namespace HolypawSynth;

	static int32 Frames(const float Seconds)
	{
		return FMath::Max(32, FMath::RoundToInt(Seconds * SampleRate));
	}

	static void Mix(FStereo& Out, const int32 I, const float L, const float R)
	{
		if (Out.L.IsValidIndex(I))
		{
			Out.L[I] += L;
			Out.R[I] += R;
		}
	}

	static void Thump(FStereo& Out, const float Start, const float Hz0, const float Hz1, const float Amp, const float Decay)
	{
		const int32 N = Out.Num();
		for (int32 I = 0; I < N; ++I)
		{
			const float T = static_cast<float>(I) / static_cast<float>(SampleRate) - Start;
			if (T < 0.f)
			{
				continue;
			}
			const float Hz = FMath::Lerp(Hz0, Hz1, FMath::Clamp(T / 0.08f, 0.f, 1.f));
			const float S = OscSine(Hz * T) * ExpDecay(T, Decay) * Amp;
			Mix(Out, I, S, S);
		}
	}

	static void NoiseBurst(FStereo& Out, uint32& Rng, const float Start, const float Amp, const float Decay, const float Cut)
	{
		FOnePole F;
		const int32 N = Out.Num();
		const float C = OnePoleCoeff(Cut);
		for (int32 I = 0; I < N; ++I)
		{
			const float T = static_cast<float>(I) / static_cast<float>(SampleRate) - Start;
			if (T < 0.f)
			{
				continue;
			}
			const float S = F.Tick(White(Rng), C) * ExpDecay(T, Decay) * Amp;
			Mix(Out, I, S * 0.85f, S);
		}
	}

	static void Chime(FStereo& Out, const float Start, const float Hz, const float Amp, const float Index, const float Pan)
	{
		const int32 N = Out.Num();
		for (int32 I = 0; I < N; ++I)
		{
			const float T = static_cast<float>(I) / static_cast<float>(SampleRate) - Start;
			if (T < 0.f)
			{
				continue;
			}
			const float S = OscBell(Hz, T, Index) * Amp;
			Mix(Out, I, S * (0.75f - Pan), S * (0.75f + Pan));
		}
	}

	static void ChoirHit(FStereo& Out, const float Start, const float Root, const float Amp, const float Seconds)
	{
		const int32 N = Out.Num();
		for (int32 I = 0; I < N; ++I)
		{
			const float T = static_cast<float>(I) / static_cast<float>(SampleRate) - Start;
			if (T < 0.f || T > Seconds)
			{
				continue;
			}
			const float E = EnvADSR(T, Seconds, 0.04f, 0.18f, 0.55f, 0.35f);
			const float S = (OscWarm(Root * T, 0.4f) + 0.5f * OscWarm(Root * 1.25f * T, 0.3f) + 0.35f * OscSine(Root * 1.5f * T)) * E * Amp;
			Mix(Out, I, S * 0.9f, S);
		}
	}

	static void Tear(FStereo& Out, uint32& Rng, const float Start, const float Amp)
	{
		FOnePole F;
		const int32 N = Out.Num();
		for (int32 I = 0; I < N; ++I)
		{
			const float T = static_cast<float>(I) / static_cast<float>(SampleRate) - Start;
			if (T < 0.f || T > 0.5f)
			{
				continue;
			}
			const float Sweep = 1800.f - T * 2400.f;
			const float S = F.Tick(White(Rng), OnePoleCoeff(FMath::Max(180.f, Sweep))) * ExpDecay(T, 7.f) * Amp;
			const float Body = OscTri(90.f * T) * ExpDecay(T, 9.f) * Amp * 0.35f;
			Mix(Out, I, S + Body, S * 1.1f + Body * 0.8f);
		}
	}

	void RenderCue(const FName Cue, uint32 Seed, FStereo& Out)
	{
		InitLut();
		if (Seed == 0)
		{
			Seed = 0xA11u;
		}
		uint32 Rng = HashU(Seed);
		const FHolypawCueDef* Def = HolypawCues::Find(Cue);
		const float Seconds = Def ? Def->Seconds : 0.2f;
		Out.Zero(Frames(Seconds));

		if (Cue == TEXT("Hug"))
		{
			Thump(Out, 0.f, 220.f, 90.f, 0.55f, 14.f);
			NoiseBurst(Out, Rng, 0.f, 0.22f, 28.f, 1800.f);
			Chime(Out, 0.04f, 523.f, 0.18f, 1.2f, -0.2f);
			Chime(Out, 0.09f, 659.f, 0.12f, 1.1f, 0.25f);
		}
		else if (Cue == TEXT("Convert"))
		{
			Chime(Out, 0.00f, 392.f, 0.42f, 2.0f, -0.25f);
			Chime(Out, 0.12f, 523.f, 0.40f, 1.8f, 0.05f);
			Chime(Out, 0.24f, 659.f, 0.36f, 1.6f, 0.22f);
			Chime(Out, 0.38f, 784.f, 0.28f, 1.4f, -0.1f);
			ChoirHit(Out, 0.08f, 196.f, 0.28f, 0.9f);
		}
		else if (Cue == TEXT("Miracle") || Cue == TEXT("Chapel"))
		{
			ChoirHit(Out, 0.f, 165.f, 0.42f, Seconds);
			ChoirHit(Out, 0.05f, 220.f, 0.28f, Seconds * 0.9f);
			Chime(Out, 0.10f, 659.f, 0.22f, 2.4f, -0.3f);
			Chime(Out, 0.22f, 784.f, 0.18f, 2.1f, 0.25f);
			Chime(Out, 0.40f, 988.f, 0.14f, 1.8f, 0.05f);
			NoiseBurst(Out, Rng, 0.02f, 0.08f, 4.f, 6000.f);
		}
		else if (Cue == TEXT("Save") || Cue == TEXT("UiConfirm"))
		{
			Chime(Out, 0.00f, 880.f, 0.28f, 1.4f, -0.1f);
			Chime(Out, 0.05f, 1320.f, 0.18f, 1.1f, 0.15f);
		}
		else if (Cue == TEXT("Shop") || Cue == TEXT("Pickup"))
		{
			Chime(Out, 0.00f, 698.f, 0.28f, 1.3f, -0.15f);
			Chime(Out, 0.06f, 880.f, 0.22f, 1.1f, 0.2f);
			NoiseBurst(Out, Rng, 0.f, 0.08f, 40.f, 4000.f);
		}
		else if (Cue == TEXT("Talk") || Cue == TEXT("UiMove"))
		{
			Thump(Out, 0.f, 640.f, 520.f, 0.22f, 30.f);
			Chime(Out, 0.f, 720.f + White(Rng) * 40.f, 0.12f, 0.6f, White(Rng) * 0.3f);
		}
		else if (Cue == TEXT("Title"))
		{
			ChoirHit(Out, 0.f, 147.f, 0.4f, Seconds);
			Chime(Out, 0.08f, 294.f, 0.28f, 1.8f, -0.2f);
			Chime(Out, 0.22f, 440.f, 0.24f, 1.6f, 0.2f);
			Chime(Out, 0.40f, 587.f, 0.18f, 1.4f, 0.0f);
		}
		else if (Cue == TEXT("Door"))
		{
			NoiseBurst(Out, Rng, 0.00f, 0.28f, 18.f, 900.f);
			Thump(Out, 0.02f, 140.f, 70.f, 0.35f, 12.f);
			NoiseBurst(Out, Rng, 0.16f, 0.16f, 22.f, 1400.f);
		}
		else if (Cue == TEXT("Travel") || Cue == TEXT("Lantern"))
		{
			ChoirHit(Out, 0.f, 196.f, 0.22f, Seconds);
			Chime(Out, 0.05f, 523.f, 0.24f, 1.5f, -0.2f);
			Chime(Out, 0.18f, 784.f, 0.20f, 1.3f, 0.2f);
			NoiseBurst(Out, Rng, 0.f, 0.12f, 8.f, 2500.f);
		}
		else if (Cue == TEXT("Jump"))
		{
			Thump(Out, 0.f, 280.f, 160.f, 0.28f, 18.f);
			NoiseBurst(Out, Rng, 0.f, 0.12f, 32.f, 2200.f);
		}
		else if (Cue == TEXT("Land"))
		{
			Thump(Out, 0.f, 160.f, 70.f, 0.42f, 16.f);
			NoiseBurst(Out, Rng, 0.f, 0.18f, 24.f, 1200.f);
		}
		else if (Cue == TEXT("Hurt"))
		{
			Thump(Out, 0.f, 110.f, 48.f, 0.62f, 11.f);
			NoiseBurst(Out, Rng, 0.f, 0.28f, 16.f, 800.f);
			Chime(Out, 0.02f, 185.f, 0.12f, 0.4f, 0.1f);
		}
		else if (Cue == TEXT("BattleHit") || Cue == TEXT("Slap"))
		{
			Thump(Out, 0.f, 180.f, 70.f, 0.62f, 16.f);
			NoiseBurst(Out, Rng, 0.f, 0.32f, 26.f, 1600.f);
			Thump(Out, 0.015f, 340.f, 180.f, 0.18f, 22.f);
		}
		else if (Cue == TEXT("BattleCrit"))
		{
			Thump(Out, 0.f, 220.f, 60.f, 0.7f, 12.f);
			NoiseBurst(Out, Rng, 0.f, 0.4f, 18.f, 2800.f);
			Chime(Out, 0.04f, 1174.f, 0.22f, 1.2f, -0.2f);
			Chime(Out, 0.08f, 1568.f, 0.16f, 1.0f, 0.25f);
		}
		else if (Cue == TEXT("BattleStagger"))
		{
			Thump(Out, 0.f, 90.f, 40.f, 0.7f, 8.f);
			NoiseBurst(Out, Rng, 0.02f, 0.35f, 10.f, 600.f);
			Chime(Out, 0.08f, 233.f, 0.16f, 0.5f, 0.f);
		}
		else if (Cue == TEXT("BattleStart"))
		{
			Thump(Out, 0.f, 80.f, 40.f, 0.55f, 6.f);
			ChoirHit(Out, 0.04f, 110.f, 0.32f, 0.8f);
			NoiseBurst(Out, Rng, 0.f, 0.22f, 7.f, 1500.f);
			Chime(Out, 0.18f, 330.f, 0.18f, 0.8f, -0.2f);
			Chime(Out, 0.30f, 392.f, 0.16f, 0.8f, 0.2f);
		}
		else if (Cue == TEXT("BattleWin"))
		{
			Chime(Out, 0.00f, 523.f, 0.36f, 1.6f, -0.2f);
			Chime(Out, 0.12f, 659.f, 0.34f, 1.5f, 0.0f);
			Chime(Out, 0.24f, 784.f, 0.32f, 1.4f, 0.2f);
			Chime(Out, 0.42f, 1046.f, 0.24f, 1.2f, 0.05f);
			ChoirHit(Out, 0.10f, 196.f, 0.22f, 1.1f);
		}
		else if (Cue == TEXT("BattleLose"))
		{
			ChoirHit(Out, 0.f, 98.f, 0.4f, Seconds);
			Thump(Out, 0.f, 70.f, 32.f, 0.5f, 5.f);
			Chime(Out, 0.20f, 196.f, 0.16f, 0.7f, -0.1f);
			Chime(Out, 0.45f, 175.f, 0.12f, 0.5f, 0.15f);
		}
		else if (Cue == TEXT("BossPhase") || Cue == TEXT("MillSting"))
		{
			Thump(Out, 0.f, 55.f, 28.f, 0.8f, 5.f);
			Tear(Out, Rng, 0.04f, 0.55f);
			ChoirHit(Out, 0.08f, 82.f, 0.35f, 1.2f);
			NoiseBurst(Out, Rng, 0.f, 0.3f, 6.f, 900.f);
		}
		else if (Cue == TEXT("Beam"))
		{
			ChoirHit(Out, 0.f, 330.f, 0.28f, 0.5f);
			NoiseBurst(Out, Rng, 0.f, 0.2f, 9.f, 4000.f);
			Chime(Out, 0.02f, 880.f, 0.16f, 2.0f, 0.f);
			Thump(Out, 0.08f, 140.f, 80.f, 0.22f, 14.f);
		}
		else if (Cue == TEXT("Unstuff") || Cue == TEXT("PolyRip"))
		{
			Tear(Out, Rng, 0.f, Cue == TEXT("PolyRip") ? 0.7f : 0.55f);
			Thump(Out, 0.02f, 100.f, 45.f, 0.5f, 10.f);
			if (Cue == TEXT("PolyRip"))
			{
				Chime(Out, 0.1f, 185.f, 0.1f, 0.3f, 0.2f);
			}
		}
		else if (Cue == TEXT("Party"))
		{
			Thump(Out, 0.00f, 200.f, 90.f, 0.4f, 16.f);
			Thump(Out, 0.07f, 240.f, 100.f, 0.34f, 16.f);
			Thump(Out, 0.14f, 180.f, 80.f, 0.38f, 14.f);
			NoiseBurst(Out, Rng, 0.f, 0.18f, 20.f, 1800.f);
		}
		else if (Cue == TEXT("Guard"))
		{
			Thump(Out, 0.f, 160.f, 90.f, 0.35f, 14.f);
			NoiseBurst(Out, Rng, 0.f, 0.14f, 20.f, 1200.f);
			Chime(Out, 0.04f, 392.f, 0.12f, 0.8f, 0.f);
		}
		else if (Cue == TEXT("Hymn") || Cue == TEXT("DuskHymn") || Cue == TEXT("Lullaby") || Cue == TEXT("Shrine") || Cue == TEXT("BannerDown") || Cue == TEXT("ShopOpen"))
		{
			ChoirHit(Out, 0.f, 196.f, 0.34f, Seconds * 0.9f);
			Chime(Out, 0.06f, 392.f, 0.22f, 1.6f, -0.2f);
			Chime(Out, 0.18f, 494.f, 0.18f, 1.4f, 0.15f);
			if (Cue == TEXT("Lullaby"))
			{
				Chime(Out, 0.32f, 587.f, 0.16f, 1.5f, 0.05f);
			}
		}
		else if (Cue == TEXT("Flee"))
		{
			NoiseBurst(Out, Rng, 0.f, 0.22f, 14.f, 2000.f);
			Thump(Out, 0.f, 300.f, 180.f, 0.22f, 18.f);
			Thump(Out, 0.08f, 260.f, 140.f, 0.18f, 18.f);
		}
		else if (Cue == TEXT("Fountain"))
		{
			NoiseBurst(Out, Rng, 0.f, 0.22f, 8.f, 5000.f);
			Chime(Out, 0.04f, 784.f, 0.18f, 1.4f, -0.2f);
			Chime(Out, 0.14f, 988.f, 0.14f, 1.2f, 0.25f);
			ChoirHit(Out, 0.1f, 247.f, 0.16f, 0.7f);
		}
		else if (Cue == TEXT("Inn") || Cue == TEXT("Kneel"))
		{
			ChoirHit(Out, 0.f, 175.f, 0.28f, Seconds);
			Chime(Out, 0.08f, 349.f, 0.16f, 1.2f, -0.1f);
		}
		else if (Cue == TEXT("UiOpen"))
		{
			Chime(Out, 0.f, 523.f, 0.2f, 1.0f, -0.15f);
			Chime(Out, 0.04f, 659.f, 0.14f, 0.9f, 0.15f);
		}
		else if (Cue == TEXT("UiClose") || Cue == TEXT("UiDeny"))
		{
			Chime(Out, 0.f, 392.f, 0.16f, 0.7f, 0.1f);
			Thump(Out, 0.f, 180.f, 120.f, 0.12f, 20.f);
		}
		else if (Cue == TEXT("FootstepSoft") || Cue == TEXT("FootstepWood") || Cue == TEXT("FootstepStone")
			|| Cue == TEXT("FootstepSnow") || Cue == TEXT("FootstepSand") || Cue == TEXT("FootstepWater"))
		{
			RenderFootstep(EHolypawZone::ForestCottage, Cue == TEXT("FootstepWood"), Seed, Out);
			return;
		}
		else
		{
			Thump(Out, 0.f, 480.f, 240.f, 0.28f, 20.f);
			Chime(Out, 0.f, 640.f, 0.12f, 0.8f, 0.f);
		}
		AddDelayWidth(Out, 90, 0.08f);
		SoftLimit(Out);
	}

	void RenderAbility(const FName Kind, const bool bCrit, const bool bStagger, const uint32 Seed, FStereo& Out)
	{
		FName Cue = TEXT("BattleHit");
		if (Kind == TEXT("slap")) { Cue = TEXT("Slap"); }
		else if (Kind == TEXT("beam") || Kind == TEXT("buttonBeam")) { Cue = TEXT("Beam"); }
		else if (Kind == TEXT("party")) { Cue = TEXT("Party"); }
		else if (Kind == TEXT("unstuff")) { Cue = TEXT("Unstuff"); }
		else if (Kind == TEXT("polyRip")) { Cue = TEXT("PolyRip"); }
		else if (Kind == TEXT("guard") || Kind == TEXT("stitch") || Kind == TEXT("seamGuard")) { Cue = TEXT("Guard"); }
		else if (Kind == TEXT("hymn")) { Cue = TEXT("Hymn"); }
		else if (Kind == TEXT("lullaby")) { Cue = TEXT("Lullaby"); }
		else if (Kind == TEXT("flee")) { Cue = TEXT("Flee"); }
		RenderCue(Cue, Seed, Out);
		if (bCrit)
		{
			FStereo Extra;
			RenderCue(TEXT("BattleCrit"), Seed ^ 0x51u, Extra);
			const int32 N = FMath::Min(Out.Num(), Extra.Num());
			for (int32 I = 0; I < N; ++I)
			{
				Out.L[I] += Extra.L[I] * 0.55f;
				Out.R[I] += Extra.R[I] * 0.55f;
			}
		}
		if (bStagger)
		{
			FStereo Extra;
			RenderCue(TEXT("BattleStagger"), Seed ^ 0xA3u, Extra);
			const int32 N = FMath::Min(Out.Num(), Extra.Num());
			for (int32 I = 0; I < N; ++I)
			{
				Out.L[I] += Extra.L[I] * 0.45f;
				Out.R[I] += Extra.R[I] * 0.45f;
			}
		}
		SoftLimit(Out);
	}

	void RenderFootstep(const EHolypawZone Zone, const bool bInterior, uint32 Seed, FStereo& Out)
	{
		InitLut();
		if (Seed == 0)
		{
			Seed = 0xF00u;
		}
		uint32 Rng = HashU(Seed ^ static_cast<uint32>(Zone));
		Out.Zero(Frames(0.13f));
		float Cut = 1400.f;
		float Amp = 0.22f;
		float Hz = 180.f;
		if (bInterior)
		{
			Cut = 1100.f;
			Hz = 220.f;
			Amp = 0.24f;
		}
		else
		{
			switch (Zone)
			{
			case EHolypawZone::Snowveil:
			case EHolypawZone::Snow:
			case EHolypawZone::Ice:
			case EHolypawZone::TundraParish:
			case EHolypawZone::FeltIceCamp:
			case EHolypawZone::AuroraBorough:
				Cut = 3200.f;
				Amp = 0.16f;
				Hz = 90.f;
				break;
			case EHolypawZone::Desert:
			case EHolypawZone::DustMesa:
			case EHolypawZone::SandHymn:
			case EHolypawZone::PalmaDusk:
				Cut = 900.f;
				Amp = 0.18f;
				Hz = 120.f;
				break;
			case EHolypawZone::Ocean:
			case EHolypawZone::Tidewell:
			case EHolypawZone::Coast:
			case EHolypawZone::CoralChoir:
			case EHolypawZone::CapePlush:
				Cut = 2400.f;
				Amp = 0.20f;
				Hz = 140.f;
				break;
			case EHolypawZone::RibbonCity:
			case EHolypawZone::Clockhaven:
			case EHolypawZone::MarbleForum:
			case EHolypawZone::LanternAngeles:
				Cut = 1600.f;
				Hz = 240.f;
				Amp = 0.26f;
				break;
			default:
				break;
			}
		}
		Thump(Out, 0.f, Hz, Hz * 0.45f, Amp, 22.f);
		NoiseBurst(Out, Rng, 0.f, Amp * 0.7f, 30.f, Cut);
	}

	int32 FootSurface(const EHolypawZone Zone, const bool bInterior)
	{
		if (bInterior)
		{
			return 1;
		}
		switch (Zone)
		{
		case EHolypawZone::Snowveil:
		case EHolypawZone::Snow:
		case EHolypawZone::Ice:
		case EHolypawZone::TundraParish:
		case EHolypawZone::FeltIceCamp:
		case EHolypawZone::AuroraBorough:
			return 3;
		case EHolypawZone::Desert:
		case EHolypawZone::DustMesa:
		case EHolypawZone::SandHymn:
		case EHolypawZone::PalmaDusk:
			return 4;
		case EHolypawZone::Ocean:
		case EHolypawZone::Tidewell:
		case EHolypawZone::Coast:
		case EHolypawZone::CoralChoir:
		case EHolypawZone::CapePlush:
			return 5;
		case EHolypawZone::RibbonCity:
		case EHolypawZone::Clockhaven:
		case EHolypawZone::MarbleForum:
		case EHolypawZone::LanternAngeles:
			return 2;
		default:
			return 0;
		}
	}
}
