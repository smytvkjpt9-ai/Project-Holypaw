#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "CampRest.generated.h"

/** Wilderness / city camp — heal on interact. */
UCLASS()
class HOLYPAW_API ACampRest : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	ACampRest();
	virtual void BeginPlay() override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	FText CampName;
};
