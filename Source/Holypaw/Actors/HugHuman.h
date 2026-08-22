#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "Anim/HolypawProcAnim.h"
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

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	FLinearColor ShirtColor = FLinearColor(0.55f, 0.5f, 0.62f);

	FVector HomeLocation = FVector::ZeroVector;

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
	void PlayConvertBow();
	void ReceiveHug();
	void ReceiveHug(const FVector& FromWorld);
	void ResetFaith();
	void RestoreFaith(float Progress, bool bNowBeliever, bool bNowKnelt);
	bool IsKnelt() const { return bKnelt; }
	FString GetSkepticLine(int32 Pct) const;
	FString GetBelieverLine() const;

protected:
	bool bKnelt = false;
	HolypawAnim::FHumanState HumanAnim;
	HolypawAnim::FHumanRest HumanRest;
};
