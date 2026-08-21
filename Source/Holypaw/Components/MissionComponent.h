#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HolypawTypes.h"
#include "MissionComponent.generated.h"

UCLASS(ClassGroup = (Holypaw), meta = (BlueprintSpawnableComponent))
class HOLYPAW_API UHolypawMissionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHolypawMissionComponent();

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	int32 CurrentIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	int32 Recruits = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	int32 Kills = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	int32 Converts = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	int32 Miracles = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	bool bCampaignComplete = false;

	UPROPERTY()
	TArray<EHolypawZone> ZonesVisited;

	UPROPERTY()
	TArray<EHolypawVillain> BossesFell;

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	void NotifyZone(EHolypawZone Zone);

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	void NotifyRecruit();

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	void NotifyKill(EHolypawVillain Id, EVillainRank Rank);

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	void NotifyConvert();

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	void NotifyMiracle(EHolypawZone Zone);

	FMissionDef GetCurrent() const;
	TArray<FString> GetJournalLines() const;
	void TryAdvance(class AHolypawCharacter* Pawn);

private:
	bool IsComplete(EHolypawMission Id) const;
	bool HasZone(EHolypawZone Zone) const;
	bool HasBoss(EHolypawVillain Id) const;
	int32 RiteCount() const;
};
