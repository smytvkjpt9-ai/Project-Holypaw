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

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	FLinearColor ShirtColor = FLinearColor(0.55f, 0.5f, 0.62f);

	FVector HomeLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HeadMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ArmL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ArmR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Hair;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Bangs;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Neck;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> EyeL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> EyeR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HighlightL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HighlightR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Hat;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HandL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HandR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> LegL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> LegR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ShoeL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ShoeR;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;

	void BecomeBeliever();
	void KneelInWorship();
	void ReceiveHug();
	void ResetFaith();
	void RestoreFaith(float Progress, bool bNowBeliever, bool bNowKnelt);
	bool IsKnelt() const { return bKnelt; }
	FString GetSkepticLine(int32 Pct) const;
	FString GetBelieverLine() const;

protected:
	bool bKnelt = false;
	float BounceT = 0.f;
	float HugPulse = 0.f;
	FVector BaseScale = FVector::OneVector;
};
