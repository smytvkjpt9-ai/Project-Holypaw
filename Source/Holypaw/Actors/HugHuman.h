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

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	bool bBeliever = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HeadMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ArmL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ArmR;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;

	void BecomeBeliever();
	void KneelInWorship();
	void ReceiveHug();
	FString GetSkepticLine(int32 Pct) const;
	FString GetBelieverLine() const;

protected:
	bool bKnelt = false;
	float BounceT = 0.f;
	float HugPulse = 0.f;
	FVector BaseScale = FVector::OneVector;
};
