#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "HolypawTypes.h"
#include "TravelLantern.generated.h"

class UPointLightComponent;

UCLASS()
class HOLYPAW_API ATravelLantern : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	ATravelLantern();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	EHolypawZone AnchorZone = EHolypawZone::ForestCottage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Globe;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Hood;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPointLightComponent> Glow;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;
};
