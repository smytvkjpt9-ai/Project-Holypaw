#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "HugHuman.generated.h"

UCLASS()
class HOLYPAW_API AHugHuman : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	AHugHuman();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	FText PersonName;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	float ConvertProgress = 0.f;

	virtual void BeginPlay() override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;
};
