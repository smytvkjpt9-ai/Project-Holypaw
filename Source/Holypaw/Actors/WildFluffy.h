#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "HolypawTypes.h"
#include "WildFluffy.generated.h"

UCLASS()
class HOLYPAW_API AWildFluffy : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	AWildFluffy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	FFluffyTypeDef Type;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	bool bRecruited = false;

	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	FVector Home = FVector::ZeroVector;

private:
	float WanderT = 0.f;
	FVector Vel = FVector::ZeroVector;
};
