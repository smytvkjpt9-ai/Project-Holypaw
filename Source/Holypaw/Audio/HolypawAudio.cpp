#include "Audio/HolypawAudio.h"
#include "HolypawGameInstance.h"
#include "HolypawTypes.h"
#include "Save/HolypawSaveGame.h"
#include "Sound/SoundWaveProcedural.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

namespace HolypawAudio
{
	static void BuildTone(TArray<uint8>& PCM, int32 SampleRate, float Seconds, float FreqA, float FreqB, float Volume)
	{
		const int32 N = FMath::Max(32, FMath::RoundToInt(static_cast<float>(SampleRate) * Seconds));
		PCM.SetNumZeroed(N * sizeof(int16));
		int16* Samples = reinterpret_cast<int16*>(PCM.GetData());
		for (int32 I = 0; I < N; ++I)
		{
			const float T = static_cast<float>(I) / static_cast<float>(SampleRate);
			const float Env = FMath::Pow(FMath::Clamp(1.f - T / Seconds, 0.f, 1.f), 1.6f);
			const float Wave = 0.62f * FMath::Sin(2.f * PI * FreqA * T) + 0.38f * FMath::Sin(2.f * PI * FreqB * T);
			Samples[I] = static_cast<int16>(FMath::Clamp(Wave * Env * Volume, -1.f, 1.f) * 30000.f);
		}
	}

	void PlayCue(const UObject* WorldContext, FName Cue)
	{
		UHolypawGameInstance* GI = UHolypawGameInstance::Get(WorldContext);
		if (GI && GI->Settings && GI->Settings->bMuted)
		{
			return;
		}
		if (!WorldContext || !WorldContext->GetWorld())
		{
			return;
		}

		float Seconds = 0.16f;
		float FreqA = 520.f;
		float FreqB = 780.f;
		float Volume = 0.45f;
		if (Cue == TEXT("Hug"))
		{
			FreqA = 480.f;
			FreqB = 720.f;
			Seconds = 0.14f;
		}
		else if (Cue == TEXT("Convert"))
		{
			FreqA = 392.f;
			FreqB = 587.f;
			Seconds = 0.42f;
			Volume = 0.55f;
		}
		else if (Cue == TEXT("Miracle"))
		{
			FreqA = 330.f;
			FreqB = 495.f;
			Seconds = 0.7f;
			Volume = 0.6f;
		}
		else if (Cue == TEXT("Save"))
		{
			FreqA = 880.f;
			FreqB = 1320.f;
			Seconds = 0.09f;
			Volume = 0.28f;
		}
		else if (Cue == TEXT("BattleHit"))
		{
			FreqA = 180.f;
			FreqB = 90.f;
			Seconds = 0.12f;
			Volume = 0.5f;
		}
		else if (Cue == TEXT("BattleWin"))
		{
			FreqA = 523.f;
			FreqB = 784.f;
			Seconds = 0.35f;
		}
		else if (Cue == TEXT("Hurt"))
		{
			FreqA = 140.f;
			FreqB = 70.f;
			Seconds = 0.18f;
			Volume = 0.48f;
		}
		else if (Cue == TEXT("Travel"))
		{
			FreqA = 440.f;
			FreqB = 880.f;
			Seconds = 0.28f;
		}
		else if (Cue == TEXT("Talk"))
		{
			FreqA = 620.f;
			FreqB = 640.f;
			Seconds = 0.11f;
			Volume = 0.3f;
		}
		else if (Cue == TEXT("Shop"))
		{
			FreqA = 700.f;
			FreqB = 1050.f;
			Seconds = 0.12f;
		}
		else if (Cue == TEXT("Title"))
		{
			FreqA = 294.f;
			FreqB = 440.f;
			Seconds = 0.5f;
		}
		else if (Cue == TEXT("Door"))
		{
			FreqA = 196.f;
			FreqB = 247.f;
			Seconds = 0.22f;
			Volume = 0.32f;
		}
		else if (Cue == TEXT("DuskHymn"))
		{
			FreqA = 196.f;
			FreqB = 294.f;
			Seconds = 0.85f;
			Volume = 0.42f;
		}
		else if (Cue == TEXT("BannerDown"))
		{
			FreqA = 110.f;
			FreqB = 165.f;
			Seconds = 0.38f;
			Volume = 0.4f;
		}
		else if (Cue == TEXT("ShopOpen"))
		{
			FreqA = 523.f;
			FreqB = 784.f;
			Seconds = 0.22f;
			Volume = 0.4f;
		}

		TArray<uint8> PCM;
		BuildTone(PCM, 18000, Seconds, FreqA, FreqB, Volume);
		USoundWaveProcedural* Wave = NewObject<USoundWaveProcedural>(GetTransientPackage());
		if (!Wave)
		{
			return;
		}
		Wave->SetSampleRate(18000);
		Wave->NumChannels = 1;
		Wave->Duration = Seconds;
		Wave->SoundGroup = SOUNDGROUP_Default;
		Wave->bLooping = false;
		Wave->QueueAudio(PCM.GetData(), PCM.Num());
		const float Mix = (GI && GI->Settings) ? GI->Settings->MasterVolume : 1.f;
		UGameplayStatics::PlaySound2D(WorldContext, Wave, Mix);
		if (GI)
		{
			GI->KeepWave(Wave);
		}
	}

	static void ThemePitch(const EHolypawZone Zone, const bool bInterior, float& FreqA, float& FreqB, float& Volume)
	{
		FreqA = 196.f;
		FreqB = 247.f;
		Volume = bInterior ? 0.18f : 0.14f;
		switch (Zone)
		{
		case EHolypawZone::ForestCottage:
			FreqA = 174.f;
			FreqB = 261.f;
			break;
		case EHolypawZone::RibbonCity:
			FreqA = 220.f;
			FreqB = 330.f;
			break;
		case EHolypawZone::Tidewell:
		case EHolypawZone::CapePlush:
		case EHolypawZone::CoralChoir:
			FreqA = 164.f;
			FreqB = 246.f;
			break;
		case EHolypawZone::Snowveil:
		case EHolypawZone::AuroraBorough:
		case EHolypawZone::TundraParish:
		case EHolypawZone::FeltIceCamp:
			FreqA = 293.f;
			FreqB = 349.f;
			break;
		case EHolypawZone::Emberfen:
			FreqA = 155.f;
			FreqB = 185.f;
			break;
		case EHolypawZone::LanternAngeles:
			FreqA = 246.f;
			FreqB = 370.f;
			break;
		case EHolypawZone::CherryLoom:
			FreqA = 277.f;
			FreqB = 415.f;
			break;
		case EHolypawZone::CarnivalBahia:
			FreqA = 196.f;
			FreqB = 294.f;
			break;
		case EHolypawZone::SilkDelta:
		case EHolypawZone::SpiceHarbor:
			FreqA = 185.f;
			FreqB = 277.f;
			break;
		default:
			break;
		}
		if (bInterior)
		{
			FreqA *= 0.85f;
			FreqB *= 0.9f;
		}
	}

	void StopTheme(UAudioComponent* Comp)
	{
		if (Comp && Comp->IsPlaying())
		{
			Comp->Stop();
		}
	}

	void PlayTheme(const UObject* WorldContext, AActor* Owner, TObjectPtr<UAudioComponent>& Comp, const EHolypawZone Zone, const bool bInterior)
	{
		UHolypawGameInstance* GI = UHolypawGameInstance::Get(WorldContext);
		if (GI && GI->Settings && GI->Settings->bMuted)
		{
			StopTheme(Comp.Get());
			return;
		}
		if (!Owner)
		{
			return;
		}
		float FreqA = 196.f;
		float FreqB = 247.f;
		float Volume = 0.14f;
		ThemePitch(Zone, bInterior, FreqA, FreqB, Volume);
		TArray<uint8> PCM;
		const float Seconds = 2.4f;
		const int32 SampleRate = 16000;
		const int32 N = FMath::RoundToInt(static_cast<float>(SampleRate) * Seconds);
		PCM.SetNumZeroed(N * sizeof(int16));
		int16* Samples = reinterpret_cast<int16*>(PCM.GetData());
		for (int32 I = 0; I < N; ++I)
		{
			const float T = static_cast<float>(I) / static_cast<float>(SampleRate);
			const float Fade = FMath::Min(T / 0.08f, FMath::Min(1.f, (Seconds - T) / 0.08f));
			const float Lfo = 0.5f + 0.5f * FMath::Sin(2.f * PI * 0.25f * T);
			const float Osc = 0.55f * FMath::Sin(2.f * PI * FreqA * T) + 0.35f * FMath::Sin(2.f * PI * FreqB * T) + 0.1f * Lfo;
			Samples[I] = static_cast<int16>(FMath::Clamp(Osc * Fade * Volume, -1.f, 1.f) * 22000.f);
		}
		USoundWaveProcedural* Pad = NewObject<USoundWaveProcedural>(Owner);
		if (!Pad)
		{
			return;
		}
		Pad->SetSampleRate(SampleRate);
		Pad->NumChannels = 1;
		Pad->Duration = Seconds;
		Pad->SoundGroup = SOUNDGROUP_Default;
		Pad->bLooping = true;
		Pad->QueueAudio(PCM.GetData(), PCM.Num());
		if (!Comp)
		{
			Comp = NewObject<UAudioComponent>(Owner, TEXT("CityTheme"));
			if (!Comp)
			{
				return;
			}
			Comp->CreationMethod = EComponentCreationMethod::Instance;
			Owner->AddInstanceComponent(Comp);
			Comp->RegisterComponent();
			Comp->bAutoActivate = false;
		}
		StopTheme(Comp.Get());
		Comp->SetSound(Pad);
		const float Mix = (GI && GI->Settings) ? GI->Settings->MasterVolume : 1.f;
		Comp->SetVolumeMultiplier(Mix * (bInterior ? 0.7f : 0.55f));
		Comp->Play();
		if (GI)
		{
			GI->KeepWave(Pad);
		}
	}

	void PlayHymn(const UObject* WorldContext, AActor* Owner, TObjectPtr<UAudioComponent>& Comp)
	{
		UHolypawGameInstance* GI = UHolypawGameInstance::Get(WorldContext);
		if (GI && GI->Settings && GI->Settings->bMuted)
		{
			StopTheme(Comp.Get());
			return;
		}
		if (!Owner)
		{
			return;
		}
		TArray<uint8> PCM;
		const float Seconds = 4.8f;
		const int32 SampleRate = 16000;
		const int32 N = FMath::RoundToInt(static_cast<float>(SampleRate) * Seconds);
		PCM.SetNumZeroed(N * sizeof(int16));
		int16* Samples = reinterpret_cast<int16*>(PCM.GetData());
		const float FreqA = 196.f;
		const float FreqB = 247.f;
		const float FreqC = 294.f;
		const float FreqD = 392.f;
		for (int32 I = 0; I < N; ++I)
		{
			const float T = static_cast<float>(I) / static_cast<float>(SampleRate);
			const float Fade = FMath::Min(T / 0.16f, FMath::Min(1.f, (Seconds - T) / 0.16f));
			const float Breath = 0.72f + 0.28f * FMath::Sin(2.f * PI * 0.22f * T);
			const float Osc =
				0.28f * FMath::Sin(2.f * PI * FreqA * T) +
				0.24f * FMath::Sin(2.f * PI * FreqB * T) +
				0.22f * FMath::Sin(2.f * PI * FreqC * T) +
				0.16f * FMath::Sin(2.f * PI * FreqD * T);
			Samples[I] = static_cast<int16>(FMath::Clamp(Osc * Fade * Breath * 0.2f, -1.f, 1.f) * 22000.f);
		}
		USoundWaveProcedural* Pad = NewObject<USoundWaveProcedural>(Owner);
		if (!Pad)
		{
			return;
		}
		Pad->SetSampleRate(SampleRate);
		Pad->NumChannels = 1;
		Pad->Duration = Seconds;
		Pad->SoundGroup = SOUNDGROUP_Default;
		Pad->bLooping = true;
		Pad->QueueAudio(PCM.GetData(), PCM.Num());
		if (!Comp)
		{
			Comp = NewObject<UAudioComponent>(Owner, TEXT("DuskHymn"));
			if (!Comp)
			{
				return;
			}
			Comp->CreationMethod = EComponentCreationMethod::Instance;
			Owner->AddInstanceComponent(Comp);
			Comp->RegisterComponent();
			Comp->bAutoActivate = false;
		}
		StopTheme(Comp.Get());
		Comp->SetSound(Pad);
		const float Mix = (GI && GI->Settings) ? GI->Settings->MasterVolume : 1.f;
		Comp->SetVolumeMultiplier(Mix * 0.62f);
		Comp->Play();
		if (GI)
		{
			GI->KeepWave(Pad);
		}
	}
}
