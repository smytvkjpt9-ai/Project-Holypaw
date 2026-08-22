#include "Audio/HolypawAudioSubsystem.h"
#include "Audio/HolypawAudioInternal.h"
#include "Audio/HolypawAudioRig.h"
#include "HolypawGameInstance.h"
#include "HolypawTypes.h"
#include "Save/HolypawSaveGame.h"
#include "Character/HolypawCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Sound/SoundBase.h"
#include "Stats/Stats.h"

UHolypawAudioSubsystem* UHolypawAudioSubsystem::Get(const UObject* WorldContext)
{
	const UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr;
	return World ? World->GetSubsystem<UHolypawAudioSubsystem>() : nullptr;
}

void UHolypawAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UHolypawAudioSubsystem::Deinitialize()
{
	StopAll();
	Rig = nullptr;
	Super::Deinitialize();
}

TStatId UHolypawAudioSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UHolypawAudioSubsystem, STATGROUP_Tickables);
}

bool UHolypawAudioSubsystem::IsTickable() const
{
	const UWorld* World = GetWorld();
	return World && World->IsGameWorld();
}

bool UHolypawAudioSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

const UHolypawSettingsSave* UHolypawAudioSubsystem::Settings() const
{
	if (const UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		return GI->Settings;
	}
	return nullptr;
}

bool UHolypawAudioSubsystem::IsMuted() const
{
	const UHolypawSettingsSave* S = Settings();
	return S && S->bMuted;
}

float UHolypawAudioSubsystem::BusGain(const EHolypawBus Bus) const
{
	if (IsMuted())
	{
		return 0.f;
	}
	const UHolypawSettingsSave* S = Settings();
	const float Master = S ? S->MasterVolume : 1.f;
	float BusV = 1.f;
	switch (Bus)
	{
	case EHolypawBus::Music: BusV = S ? S->MusicVolume : 0.85f; break;
	case EHolypawBus::Ambient: BusV = S ? S->AmbientVolume : 0.7f; break;
	case EHolypawBus::Sfx:
	case EHolypawBus::Combat: BusV = S ? S->SfxVolume : 1.f; break;
	case EHolypawBus::Ui: BusV = S ? S->UiVolume : 0.8f; break;
	case EHolypawBus::Foley: BusV = S ? S->SfxVolume * 0.85f : 0.85f; break;
	default: break;
	}
	return FMath::Clamp(Master * BusV, 0.f, 1.5f);
}

EHolypawDayPart UHolypawAudioSubsystem::DayPart() const
{
	if (const UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		if (GI->IsNight())
		{
			return EHolypawDayPart::Night;
		}
		if (GI->IsDusk())
		{
			return EHolypawDayPart::Dusk;
		}
		const float H = GI->GetWorldHour();
		if (H >= 5.f && H < 11.f)
		{
			return EHolypawDayPart::Morning;
		}
	}
	return EHolypawDayPart::Day;
}

void UHolypawAudioSubsystem::EnsureRig()
{
	if (Rig || !GetWorld())
	{
		return;
	}
	FActorSpawnParameters P;
	P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	P.ObjectFlags |= RF_Transient;
	Rig = GetWorld()->SpawnActor<AHolypawAudioRig>(FVector::ZeroVector, FRotator::ZeroRotator, P);
	if (Rig)
	{
		Rig->SetActorHiddenInGame(true);
		Rig->SetActorEnableCollision(false);
	}
}

void UHolypawAudioSubsystem::Keep(USoundWaveProcedural* Wave)
{
	if (!Wave)
	{
		return;
	}
	LiveWaves.Add(Wave);
	if (LiveWaves.Num() > 48)
	{
		LiveWaves.RemoveAt(0);
	}
	if (UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		GI->KeepWave(Wave);
	}
}

USoundWaveProcedural* UHolypawAudioSubsystem::MakeWave(TArray<uint8>& Pcm, const float Seconds, const bool bLoop, UObject* Outer)
{
	USoundWaveProcedural* Wave = NewObject<USoundWaveProcedural>(Outer ? Outer : GetTransientPackage());
	if (!Wave || Pcm.Num() == 0)
	{
		return nullptr;
	}
	Wave->SetSampleRate(HolypawSynth::SampleRate);
	Wave->NumChannels = HolypawSynth::Channels;
	Wave->Duration = Seconds;
	Wave->SoundGroup = bLoop ? SOUNDGROUP_Music : SOUNDGROUP_Default;
	Wave->bLooping = bLoop;
	Wave->QueueAudio(Pcm.GetData(), Pcm.Num());
	if (bLoop)
	{
		TArray<uint8>* Buf = &Pcm;
		Wave->OnSoundWaveProceduralUnderflow.BindLambda([Buf](USoundWaveProcedural* W, int32)
		{
			if (W && Buf && Buf->Num() > 0)
			{
				W->QueueAudio(Buf->GetData(), Buf->Num());
			}
		});
	}
	Keep(Wave);
	return Wave;
}

void UHolypawAudioSubsystem::StartStem(UAudioComponent* Comp, USoundWaveProcedural* Wave, const float Volume)
{
	if (!Comp || !Wave)
	{
		return;
	}
	if (Comp->IsPlaying())
	{
		Comp->Stop();
	}
	Comp->SetSound(Wave);
	Comp->SetVolumeMultiplier(Volume);
	Comp->Play();
}

void UHolypawAudioSubsystem::PlayPcm(TArray<uint8>&& Pcm, const float Seconds, const EHolypawBus Bus, const float ExtraGain, const float Pitch)
{
	USoundWaveProcedural* Wave = NewObject<USoundWaveProcedural>(GetTransientPackage());
	if (!Wave || Pcm.Num() == 0)
	{
		return;
	}
	Wave->SetSampleRate(HolypawSynth::SampleRate);
	Wave->NumChannels = HolypawSynth::Channels;
	Wave->Duration = Seconds;
	Wave->SoundGroup = SOUNDGROUP_Default;
	Wave->bLooping = false;
	Wave->QueueAudio(Pcm.GetData(), Pcm.Num());
	Keep(Wave);
	const float Gain = BusGain(Bus) * ExtraGain;
	if (Gain <= 0.f)
	{
		return;
	}
	UGameplayStatics::PlaySound2D(this, Wave, Gain, Pitch);
}

void UHolypawAudioSubsystem::PlayCue(const FName Cue)
{
	if (IsMuted() || Cue.IsNone())
	{
		return;
	}
	if (const FHolypawCueDef* Def = HolypawCues::Find(Cue))
	{
		if (Def->AssetPath && Def->AssetPath[0])
		{
			if (USoundBase* Asset = LoadObject<USoundBase>(nullptr, Def->AssetPath))
			{
				UGameplayStatics::PlaySound2D(this, Asset, BusGain(Def->Bus) * Def->Volume);
				return;
			}
		}
	}
	HolypawSynth::FStereo Stereo;
	HolypawSfx::RenderCue(Cue, static_cast<uint32>(++SeedCursor) * 0x9E3779B9u, Stereo);
	TArray<uint8> Pcm;
	HolypawSynth::ToPcm16(Stereo, Pcm, -7.f);
	const float Seconds = Stereo.Num() / static_cast<float>(HolypawSynth::SampleRate);
	const FHolypawCueDef* Def = HolypawCues::Find(Cue);
	const EHolypawBus Bus = Def ? Def->Bus : EHolypawBus::Sfx;
	const float Vol = Def ? Def->Volume : 0.6f;
	const float Cents = Def ? static_cast<float>(Def->VaryCents) : 20.f;
	const float Pitch = FMath::Pow(2.f, FMath::FRandRange(-Cents, Cents) / 1200.f);
	PlayPcm(MoveTemp(Pcm), Seconds, Bus, Vol, Pitch);
}

void UHolypawAudioSubsystem::PlayAbility(const FName Kind, const bool bCrit, const bool bStagger)
{
	if (IsMuted())
	{
		return;
	}
	HolypawSynth::FStereo Stereo;
	HolypawSfx::RenderAbility(Kind, bCrit, bStagger, static_cast<uint32>(++SeedCursor) * 0x85EBCA6Bu, Stereo);
	TArray<uint8> Pcm;
	HolypawSynth::ToPcm16(Stereo, Pcm, -6.f);
	const float Seconds = Stereo.Num() / static_cast<float>(HolypawSynth::SampleRate);
	PlayPcm(MoveTemp(Pcm), Seconds, EHolypawBus::Combat, 0.78f,
		FMath::Pow(2.f, FMath::FRandRange(-40.f, 40.f) / 1200.f));
}

void UHolypawAudioSubsystem::RebuildExplore()
{
	EnsureRig();
	if (!Rig)
	{
		return;
	}
	const FHolypawScore Score = bTitle
		? HolypawScore::Title()
		: HolypawScore::ForZone(ExploreZone, bInterior, ExploreDay);
	HolypawSynth::FStereo Stereo;
	HolypawScore::Render(Score, 16.f, false, Stereo);
	TArray<uint8>& Pcm = bMusicOnA ? MusicPcmB : MusicPcmA;
	HolypawSynth::ToPcm16(Stereo, Pcm, -12.f);
	USoundWaveProcedural* Wave = MakeWave(Pcm, 16.f, true, Rig);
	UAudioComponent* Incoming = bMusicOnA ? Rig->MusicB.Get() : Rig->MusicA.Get();
	StartStem(Incoming, Wave, 0.f);
	bMusicOnA = !bMusicOnA;
	MusicFade = 0.f;
}

void UHolypawAudioSubsystem::RebuildCombat()
{
	EnsureRig();
	if (!Rig)
	{
		return;
	}
	const FHolypawScore Score = HolypawScore::Combat(ExploreZone, bBoss, bPhaseTwo);
	HolypawSynth::FStereo Stereo;
	HolypawScore::Render(Score, 10.f, true, Stereo);
	HolypawSynth::ToPcm16(Stereo, CombatPcm, -10.f);
	USoundWaveProcedural* Wave = MakeWave(CombatPcm, 10.f, true, Rig);
	StartStem(Rig->CombatComp.Get(), Wave, 0.f);
}

void UHolypawAudioSubsystem::RebuildAmbient()
{
	EnsureRig();
	if (!Rig)
	{
		return;
	}
	const FHolypawScore Score = HolypawScore::ForZone(ExploreZone, bInterior, ExploreDay);
	HolypawSynth::FStereo Stereo;
	HolypawScore::RenderBed(Score.BedKind, 8.f, 1.f, Stereo);
	HolypawSynth::ToPcm16(Stereo, AmbientPcm, -16.f);
	USoundWaveProcedural* Wave = MakeWave(AmbientPcm, 8.f, true, Rig);
	StartStem(Rig->Ambient.Get(), Wave, 0.f);
}

void UHolypawAudioSubsystem::RebuildMill()
{
	EnsureRig();
	if (!Rig)
	{
		return;
	}
	HolypawSynth::FStereo Stereo;
	HolypawScore::RenderMill(6.f, Stereo);
	HolypawSynth::ToPcm16(Stereo, MillPcm, -14.f);
	USoundWaveProcedural* Wave = MakeWave(MillPcm, 6.f, true, Rig);
	StartStem(Rig->Mill.Get(), Wave, 0.f);
}

void UHolypawAudioSubsystem::SetExplore(const EHolypawZone Zone, const bool bIn)
{
	const EHolypawDayPart Day = DayPart();
	const bool bChanged = !Rig || Zone != ExploreZone || bIn != bInterior || Day != ExploreDay;
	ExploreZone = Zone;
	bInterior = bIn;
	ExploreDay = Day;
	if (bChanged)
	{
		RebuildExplore();
		RebuildAmbient();
		if (!Rig || !Rig->Mill || !Rig->Mill->IsPlaying())
		{
			RebuildMill();
		}
	}
}

void UHolypawAudioSubsystem::SetCombat(const bool bActive, const bool bIsBoss, const bool bPhase)
{
	const bool bNew = bActive != bCombat || (bActive && (bIsBoss != bBoss || bPhase != bPhaseTwo));
	bCombat = bActive;
	bBoss = bIsBoss;
	bPhaseTwo = bPhase;
	if (bActive && bNew)
	{
		RebuildCombat();
		PlayCue(bPhase ? TEXT("BossPhase") : TEXT("BattleStart"));
	}
	if (!bActive && Rig && Rig->CombatComp)
	{
		// mix fades in Tick
	}
}

void UHolypawAudioSubsystem::SetTitle(const bool bOn)
{
	if (bTitle == bOn && Rig)
	{
		return;
	}
	bTitle = bOn;
	RebuildExplore();
	if (bOn)
	{
		PlayCue(TEXT("Title"));
	}
}

void UHolypawAudioSubsystem::RestartMix()
{
	RebuildExplore();
	RebuildAmbient();
	RebuildMill();
	if (bCombat)
	{
		RebuildCombat();
	}
}

void UHolypawAudioSubsystem::NotifyJumped()
{
	PlayCue(TEXT("Jump"));
}

void UHolypawAudioSubsystem::StopAll()
{
	if (!Rig)
	{
		return;
	}
	auto Stop = [](UAudioComponent* C)
	{
		if (C && C->IsPlaying())
		{
			C->Stop();
		}
	};
	Stop(Rig->MusicA.Get());
	Stop(Rig->MusicB.Get());
	Stop(Rig->CombatComp.Get());
	Stop(Rig->Ambient.Get());
	Stop(Rig->Mill.Get());
}

void UHolypawAudioSubsystem::Tick(const float DeltaTime)
{
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		return;
	}
	EnsureRig();
	if (!Rig)
	{
		return;
	}
	if (IsMuted())
	{
		StopAll();
		return;
	}
	if (Rig->MusicA && !Rig->MusicA->IsPlaying() && Rig->MusicB && !Rig->MusicB->IsPlaying())
	{
		RebuildExplore();
		RebuildAmbient();
	}

	AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Pawn)
	{
		const bool bNowTitle = Pawn->Mode == EHolypawPawnMode::Title;
		if (bNowTitle != bTitle)
		{
			SetTitle(bNowTitle);
		}
		const bool bNowCombat = Pawn->Mode == EHolypawPawnMode::Battle;
		if (bNowCombat != bCombat && !bNowCombat)
		{
			bCombat = false;
		}
	}

	const float WantCombat = bCombat ? 1.f : 0.f;
	const float WantExplore = bCombat ? 0.14f : (bTitle ? 1.f : 1.f);
	CombatMix = FMath::FInterpTo(CombatMix, WantCombat, DeltaTime, 2.6f);
	ExploreMix = FMath::FInterpTo(ExploreMix, WantExplore, DeltaTime, 2.2f);
	MusicFade = FMath::Clamp(MusicFade + DeltaTime / 1.35f, 0.f, 1.f);

	const float MusicBus = BusGain(EHolypawBus::Music);
	const float InVol = MusicBus * ExploreMix * MusicFade * (bTitle ? 0.72f : 0.58f);
	const float OutVol = MusicBus * ExploreMix * (1.f - MusicFade) * (bTitle ? 0.72f : 0.58f);
	if (bMusicOnA)
	{
		if (Rig->MusicA) { Rig->MusicA->SetVolumeMultiplier(InVol); }
		if (Rig->MusicB) { Rig->MusicB->SetVolumeMultiplier(OutVol); }
	}
	else
	{
		if (Rig->MusicB) { Rig->MusicB->SetVolumeMultiplier(InVol); }
		if (Rig->MusicA) { Rig->MusicA->SetVolumeMultiplier(OutVol); }
	}
	if (Rig->CombatComp)
	{
		Rig->CombatComp->SetVolumeMultiplier(BusGain(EHolypawBus::Combat) * CombatMix * 0.62f);
		if (CombatMix < 0.02f && !bCombat && Rig->CombatComp->IsPlaying())
		{
			Rig->CombatComp->Stop();
		}
	}

	const float AmbientWant = (bTitle || bCombat) ? 0.08f : (bInterior ? 0.35f : 0.7f);
	if (Rig->Ambient)
	{
		if (!Rig->Ambient->IsPlaying() && AmbientPcm.Num() > 0)
		{
			RebuildAmbient();
		}
		Rig->Ambient->SetVolumeMultiplier(BusGain(EHolypawBus::Ambient) * AmbientWant);
	}

	float MillWant = 0.f;
	if (Pawn && !bTitle)
	{
		const FHolypawCity Ribbon = HolypawCatalog::GetCity(EHolypawZone::RibbonCity);
		const FVector2D MillPos = Ribbon.Pos + FVector2D(5200.f, -800.f);
		const FVector2D P(Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y);
		const float Dist = FVector2D::Distance(P, MillPos);
		MillWant = FMath::Clamp(1.f - Dist / 9000.f, 0.f, 1.f);
		MillWant = MillWant * MillWant;
	}
	if (Rig->Mill)
	{
		if (MillWant > 0.02f && !Rig->Mill->IsPlaying())
		{
			RebuildMill();
		}
		Rig->Mill->SetVolumeMultiplier(BusGain(EHolypawBus::Ambient) * MillWant * 0.85f);
		if (MillWant < 0.01f && Rig->Mill->IsPlaying())
		{
			Rig->Mill->Stop();
		}
	}

	if (Pawn && Pawn->Mode == EHolypawPawnMode::Play)
	{
		UCharacterMovementComponent* Move = Pawn->GetCharacterMovement();
		const bool bFalling = Move && Move->IsFalling();
		if (bWasFalling && !bFalling)
		{
			PlayCue(TEXT("Land"));
		}
		bWasFalling = bFalling;
		const float Speed = Pawn->GetVelocity().Size();
		if (!bFalling && Speed > 90.f)
		{
			FootAcc += DeltaTime * (1.7f + Speed / 420.f);
			if (FootAcc >= 1.f)
			{
				FootAcc -= 1.f;
				HolypawSynth::FStereo Stereo;
				HolypawSfx::RenderFootstep(Pawn->CurrentZone, bInterior, static_cast<uint32>(++SeedCursor), Stereo);
				TArray<uint8> Pcm;
				HolypawSynth::ToPcm16(Stereo, Pcm, -18.f);
				PlayPcm(MoveTemp(Pcm), Stereo.Num() / static_cast<float>(HolypawSynth::SampleRate), EHolypawBus::Foley, 0.55f);
			}
		}
		else
		{
			FootAcc = 0.35f;
		}
	}

	const EHolypawDayPart Now = DayPart();
	if (!bTitle && Now != ExploreDay)
	{
		ExploreDay = Now;
		RebuildExplore();
		RebuildAmbient();
	}
}
