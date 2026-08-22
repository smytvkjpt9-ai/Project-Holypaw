#include "Audio/HolypawAudio.h"
#include "Audio/HolypawAudioSubsystem.h"
#include "Audio/HolypawAudioInternal.h"
#include "HolypawGameInstance.h"
#include "Save/HolypawSaveGame.h"
#include "Sound/SoundWaveProcedural.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HolypawTypes.h"

namespace HolypawAudio
{
	void PlayCue(const UObject* WorldContext, const FName Cue)
	{
		if (UHolypawAudioSubsystem* Sys = UHolypawAudioSubsystem::Get(WorldContext))
		{
			Sys->PlayCue(Cue);
			return;
		}
		UHolypawGameInstance* GI = UHolypawGameInstance::Get(WorldContext);
		if (GI && GI->Settings && GI->Settings->bMuted)
		{
			return;
		}
		if (!WorldContext || !WorldContext->GetWorld())
		{
			return;
		}
		HolypawSynth::FStereo Stereo;
		HolypawSfx::RenderCue(Cue, 0x51u, Stereo);
		TArray<uint8> Pcm;
		HolypawSynth::ToPcm16(Stereo, Pcm, -7.f);
		USoundWaveProcedural* Wave = NewObject<USoundWaveProcedural>(GetTransientPackage());
		if (!Wave || Pcm.Num() == 0)
		{
			return;
		}
		Wave->SetSampleRate(HolypawSynth::SampleRate);
		Wave->NumChannels = HolypawSynth::Channels;
		Wave->Duration = Stereo.Num() / static_cast<float>(HolypawSynth::SampleRate);
		Wave->QueueAudio(Pcm.GetData(), Pcm.Num());
		const float Mix = (GI && GI->Settings) ? GI->Settings->MasterVolume : 1.f;
		UGameplayStatics::PlaySound2D(WorldContext, Wave, Mix);
		if (GI)
		{
			GI->KeepWave(Wave);
		}
	}

	void PlayAbility(const UObject* WorldContext, const FName Kind, const bool bCrit, const bool bStagger)
	{
		if (UHolypawAudioSubsystem* Sys = UHolypawAudioSubsystem::Get(WorldContext))
		{
			Sys->PlayAbility(Kind, bCrit, bStagger);
			return;
		}
		PlayCue(WorldContext, TEXT("BattleHit"));
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
		if (UHolypawAudioSubsystem* Sys = UHolypawAudioSubsystem::Get(WorldContext))
		{
			Sys->SetExplore(Zone, bInterior);
			if (Comp)
			{
				StopTheme(Comp.Get());
			}
			return;
		}
		if (!Owner)
		{
			return;
		}
		UHolypawGameInstance* GI = UHolypawGameInstance::Get(WorldContext);
		if (GI && GI->Settings && GI->Settings->bMuted)
		{
			StopTheme(Comp.Get());
			return;
		}
		HolypawSynth::FStereo Stereo;
		const FHolypawScore Score = HolypawScore::ForZone(Zone, bInterior, EHolypawDayPart::Day);
		HolypawScore::Render(Score, HolypawScore::LoopSeconds(Score), false, Stereo, false);
		TArray<uint8> Pcm;
		HolypawSynth::ToPcm16(Stereo, Pcm, -12.f);
		USoundWaveProcedural* Pad = NewObject<USoundWaveProcedural>(Owner);
		if (!Pad)
		{
			return;
		}
		Pad->SetSampleRate(HolypawSynth::SampleRate);
		Pad->NumChannels = HolypawSynth::Channels;
		Pad->Duration = 8.f;
		Pad->SoundGroup = SOUNDGROUP_Music;
		Pad->bLooping = true;
		Pad->QueueAudio(Pcm.GetData(), Pcm.Num());
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
		const float Mix = (GI && GI->Settings) ? GI->Settings->MasterVolume * GI->Settings->MusicVolume : 0.55f;
		Comp->SetVolumeMultiplier(Mix * (bInterior ? 0.7f : 0.55f));
		Comp->Play();
		if (GI)
		{
			GI->KeepWave(Pad);
		}
	}

	void PlayHymn(const UObject* WorldContext, AActor* Owner, TObjectPtr<UAudioComponent>& Comp)
	{
		PlayCue(WorldContext, TEXT("DuskHymn"));
		if (!Owner)
		{
			return;
		}
		UHolypawGameInstance* GI = UHolypawGameInstance::Get(WorldContext);
		if (GI && GI->Settings && GI->Settings->bMuted)
		{
			StopTheme(Comp.Get());
			return;
		}
		HolypawSynth::FStereo Stereo;
		HolypawSfx::RenderCue(TEXT("DuskHymn"), 0x77u, Stereo);
		TArray<uint8> Pcm;
		HolypawSynth::ToPcm16(Stereo, Pcm, -11.f);
		USoundWaveProcedural* Pad = NewObject<USoundWaveProcedural>(Owner);
		if (!Pad)
		{
			return;
		}
		Pad->SetSampleRate(HolypawSynth::SampleRate);
		Pad->NumChannels = HolypawSynth::Channels;
		Pad->Duration = FMath::Max(1.f, Stereo.Num() / static_cast<float>(HolypawSynth::SampleRate));
		Pad->SoundGroup = SOUNDGROUP_Music;
		Pad->bLooping = true;
		Pad->QueueAudio(Pcm.GetData(), Pcm.Num());
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
		const float Mix = (GI && GI->Settings) ? GI->Settings->MasterVolume * GI->Settings->MusicVolume : 0.62f;
		Comp->SetVolumeMultiplier(Mix);
		Comp->Play();
		if (GI)
		{
			GI->KeepWave(Pad);
		}
	}

	void SetCombat(const UObject* WorldContext, const bool bActive, const bool bBoss, const bool bPhaseTwo)
	{
		if (UHolypawAudioSubsystem* Sys = UHolypawAudioSubsystem::Get(WorldContext))
		{
			Sys->SetCombat(bActive, bBoss, bPhaseTwo);
		}
	}

	void NotifyJumped(const UObject* WorldContext)
	{
		if (UHolypawAudioSubsystem* Sys = UHolypawAudioSubsystem::Get(WorldContext))
		{
			Sys->NotifyJumped();
			return;
		}
		PlayCue(WorldContext, TEXT("Jump"));
	}

	void NotifyVictory(const UObject* WorldContext)
	{
		if (UHolypawAudioSubsystem* Sys = UHolypawAudioSubsystem::Get(WorldContext))
		{
			Sys->NotifyVictory();
			return;
		}
		PlayCue(WorldContext, TEXT("BattleWin"));
	}

	void ApplyMute(const UObject* WorldContext)
	{
		if (UHolypawAudioSubsystem* Sys = UHolypawAudioSubsystem::Get(WorldContext))
		{
			if (const UHolypawGameInstance* GI = UHolypawGameInstance::Get(WorldContext))
			{
				if (GI->Settings && GI->Settings->bMuted)
				{
					Sys->StopAll();
					return;
				}
			}
			Sys->RestartMix();
		}
	}

	void RestartMix(const UObject* WorldContext)
	{
		if (UHolypawAudioSubsystem* Sys = UHolypawAudioSubsystem::Get(WorldContext))
		{
			Sys->RestartMix();
		}
	}
}
