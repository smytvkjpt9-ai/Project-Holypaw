#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "CottageRest.generated.h"

/** Forest home — heal on interact. Player spawns on the porch. */
UCLASS()
class HOLYPAW_API ACottageRest : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	ACottageRest();
	virtual void BeginPlay() override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;
};
