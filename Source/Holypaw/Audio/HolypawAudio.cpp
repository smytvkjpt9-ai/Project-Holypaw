#include "Audio/HolypawAudio.h"
#include "HolypawGameInstance.h"
#include "Save/HolypawSaveGame.h"
#include "Sound/SoundWaveProcedural.h"
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
}
