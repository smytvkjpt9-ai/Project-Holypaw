#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "HolypawShrine.generated.h"

UENUM(BlueprintType)
enum class EHolypawShrineKind : uint8
{
	Inn,
	Chapel,
	Workshop,
	Wish,
	Crate
};

UCLASS()
class HOLYPAW_API AHolypawShrine : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	AHolypawShrine();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	EHolypawShrineKind Kind = EHolypawShrineKind::Inn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	FText PlaceName;

	virtual void BeginPlay() override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;
};
