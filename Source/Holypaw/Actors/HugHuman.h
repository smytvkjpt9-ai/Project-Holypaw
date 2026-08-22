#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "HolypawTypes.h"
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
	FVector ChapelGoal = FVector::ZeroVector;
	FVector InnGoal = FVector::ZeroVector;
	FVector PlazaGoal = FVector::ZeroVector;
	FVector MarketGoal = FVector::ZeroVector;
	EHolypawZone HomeZone = EHolypawZone::RibbonCity;
	bool bHomeZoneReady = false;
	bool bAnchorsReady = false;
	bool bTendsStall = false;
	float BounceT = 0.f;
	float ClapBurst = 0.f;
	float ParadeKick = 0.f;
	float ParadeSalt = 0.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HeadMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ArmL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ArmR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Sash;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;

	void BecomeBeliever(bool bCelebrate = true);
	void KneelInWorship();
	void ReceiveHug();
	void ResetFaith();
	void RestoreFaith(float Progress, bool bNowBeliever, bool bNowKnelt);
	bool IsKnelt() const { return bKnelt; }
	bool IsClapping() const;
	FString GetSkepticLine(int32 Pct) const;
	FString GetBelieverLine() const;

protected:
	bool bKnelt = false;
	float HugPulse = 0.f;
	FVector BaseScale = FVector::OneVector;
};
