#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "HolypawTypes.h"
#include "TravelLantern.generated.h"

UCLASS()
class HOLYPAW_API ATravelLantern : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	ATravelLantern();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	EHolypawZone AnchorZone = EHolypawZone::ForestCottage;

	virtual void BeginPlay() override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;
};
