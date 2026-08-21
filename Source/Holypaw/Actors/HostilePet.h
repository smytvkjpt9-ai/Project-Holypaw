#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "HostilePet.generated.h"

UCLASS()
class HOLYPAW_API AHostilePet : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	AHostilePet();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 HP = 28;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 HPMax = 28;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 Attack = 7;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	bool bDefeated = false;

	UPROPERTY(EditAnywhere, Category = "Holypaw")
	float AggroRange = 900.f;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void Defeat(bool bGrantRewards);
	void RespawnLater();

	FVector Home = FVector::ZeroVector;

private:
	FTimerHandle RespawnTimer;
	FVector WanderVel = FVector::ZeroVector;
};
