#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "FaithStall.generated.h"

/** Spend Affection for Faith at city markets. */
UCLASS()
class HOLYPAW_API AFaithStall : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	AFaithStall();
	virtual void BeginPlay() override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;

	UPROPERTY(EditAnywhere, Category = "Holypaw")
	int32 ApCost = 10;

	UPROPERTY(EditAnywhere, Category = "Holypaw")
	int32 FpGain = 8;
};
