#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "Signpost.generated.h"

UCLASS()
class HOLYPAW_API ASignpost : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	ASignpost();
	virtual void BeginPlay() override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	FText Message;

	UPROPERTY(EditAnywhere, Category = "Holypaw")
	FName LivingId;

	void RefreshFromHearts(int32 Hearts);
};
