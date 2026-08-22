#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HolypawTypes.h"
#include "HolypawSaveGame.generated.h"

USTRUCT(BlueprintType)
struct HOLYPAW_API FHolypawHumanRecord
{
	GENERATED_BODY()

	UPROPERTY()
	FString PersonName;

	UPROPERTY()
	float ConvertProgress = 0.f;

	UPROPERTY()
	bool bBeliever = false;

	UPROPERTY()
	bool bKnelt = false;
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FHolypawSkillRecord
{
	GENERATED_BODY()

	UPROPERTY()
	FName Id;

	UPROPERTY()
	bool bOwned = false;
};

UCLASS()
class HOLYPAW_API UHolypawSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 SaveVersion = 1;

	UPROPERTY()
	bool bOccupied = false;

	UPROPERTY()
	FString ZoneName;

	UPROPERTY()
	FString MissionTitle;

	UPROPERTY()
	int32 TotalHearts = 0;

	UPROPERTY()
	float PlaySeconds = 0.f;

	UPROPERTY()
	FDateTime SavedAt;

	UPROPERTY()
	FVector Location = FVector::ZeroVector;

	UPROPERTY()
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY()
	int32 HP = 50;

	UPROPERTY()
	int32 HPMax = 50;

	UPROPERTY()
	int32 Attack = 8;

	UPROPERTY()
	float WalkSpeed = 700.f;

	UPROPERTY()
	bool bHaloVisible = false;

	UPROPERTY()
	int32 AP = 0;

	UPROPERTY()
	int32 FP = 0;

	UPROPERTY()
	float MiracleCharge = 0.f;

	UPROPERTY()
	int32 Level = 1;

	UPROPERTY()
	TArray<FPartyMember> Party;

	UPROPERTY()
	TArray<FHolypawSkillRecord> Skills;

	UPROPERTY()
	EHolypawSkillTree ActiveTree = EHolypawSkillTree::Hug;

	UPROPERTY()
	int32 MissionIndex = 0;

	UPROPERTY()
	int32 Recruits = 0;

	UPROPERTY()
	int32 Kills = 0;

	UPROPERTY()
	int32 Converts = 0;

	UPROPERTY()
	int32 Miracles = 0;

	UPROPERTY()
	bool bCampaignComplete = false;

	UPROPERTY()
	TArray<EHolypawZone> ZonesVisited;

	UPROPERTY()
	TArray<EHolypawVillain> BossesFell;

	UPROPERTY()
	TArray<EHolypawVillain> SeenVillains;

	UPROPERTY()
	TArray<EHolypawVillain> DefeatedVillains;

	UPROPERTY()
	TArray<FHolypawHeartRecord> CityHearts;

	UPROPERTY()
	TArray<EHolypawZone> UnlockedTravel;

	UPROPERTY()
	TArray<FHolypawHumanRecord> Humans;

	UPROPERTY()
	TArray<FHolypawItemStack> Inventory;

	UPROPERTY()
	TArray<FName> QuestActive;

	UPROPERTY()
	TArray<FName> QuestDone;

	FString MakeSummaryLine() const;
};

UCLASS()
class HOLYPAW_API UHolypawSettingsSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bMuted = false;

	UPROPERTY()
	float MasterVolume = 1.f;

	UPROPERTY()
	bool bSubtitles = true;

	UPROPERTY()
	bool bHoldToHug = false;
};
