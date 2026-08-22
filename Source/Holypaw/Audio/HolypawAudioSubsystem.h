#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Audio/HolypawAudioTypes.h"
#include "HolypawAudioSubsystem.generated.h"

class AHolypawAudioRig;
class USoundWaveProcedural;
class UAudioComponent;
class UHolypawSettingsSave;

UCLASS()
class HOLYPAW_API UHolypawAudioSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	static UHolypawAudioSubsystem* Get(const UObject* WorldContext);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override { return false; }
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	void PlayCue(FName Cue);
	void PlayAbility(FName Kind, bool bCrit, bool bStagger);
	void SetExplore(EHolypawZone Zone, bool bInterior);
	void SetCombat(bool bActive, bool bBoss, bool bPhaseTwo);
	void SetTitle(bool bTitle);
	void StopAll();
	void NotifyJumped();
	void RestartMix();

protected:
	void EnsureRig();
	USoundWaveProcedural* MakeWave(TArray<uint8>& Pcm, float Seconds, bool bLoop, UObject* Outer);
	void StartStem(UAudioComponent* Comp, USoundWaveProcedural* Wave, float Volume);
	float BusGain(EHolypawBus Bus) const;
	const UHolypawSettingsSave* Settings() const;
	bool IsMuted() const;
	EHolypawDayPart DayPart() const;
	void RebuildExplore();
	void RebuildCombat();
	void RebuildAmbient();
	void RebuildMill();
	void PlayPcm(TArray<uint8>&& Pcm, float Seconds, EHolypawBus Bus, float ExtraGain, float Pitch = 1.f);
	void Keep(USoundWaveProcedural* Wave);

	UPROPERTY()
	TObjectPtr<AHolypawAudioRig> Rig;

	UPROPERTY()
	TArray<TObjectPtr<USoundWaveProcedural>> LiveWaves;

	TArray<uint8> MusicPcmA;
	TArray<uint8> MusicPcmB;
	TArray<uint8> CombatPcm;
	TArray<uint8> AmbientPcm;
	TArray<uint8> MillPcm;

	EHolypawZone ExploreZone = EHolypawZone::ForestCottage;
	EHolypawDayPart ExploreDay = EHolypawDayPart::Day;
	bool bInterior = false;
	bool bCombat = false;
	bool bBoss = false;
	bool bPhaseTwo = false;
	bool bTitle = true;
	bool bMusicOnA = true;
	float MusicFade = 1.f;
	float CombatMix = 0.f;
	float ExploreMix = 1.f;
	float FootAcc = 0.f;
	bool bWasFalling = false;
	int32 SeedCursor = 1;
};
