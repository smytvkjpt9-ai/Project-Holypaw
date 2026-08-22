#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "FaithStall.generated.h"

/** Spend Affection for Faith at city markets. Shutters stay down until city Hearts open shops. */
UCLASS()
class HOLYPAW_API AFaithStall : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	AFaithStall();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;

	bool IsUnlatched() const;

	UPROPERTY(EditAnywhere, Category = "Holypaw")
	int32 ApCost = 10;

	UPROPERTY(EditAnywhere, Category = "Holypaw")
	int32 FpGain = 8;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Shutter;

protected:
	void ApplyShutter(float OpenAlpha);

	float ShutterAlpha = 0.f;
};
