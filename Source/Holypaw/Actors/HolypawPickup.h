#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "HolypawPickup.generated.h"

UCLASS()
class HOLYPAW_API AHolypawPickup : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	AHolypawPickup();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	FText Label;

	virtual void BeginPlay() override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;
};
