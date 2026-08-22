#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "HolypawTypes.h"
#include "Templates/UniquePtr.h"
#include "WildFluffy.generated.h"

struct FHolypawFluffyMotion;

UCLASS()
class HOLYPAW_API AWildFluffy : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	AWildFluffy();
	virtual ~AWildFluffy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	FFluffyTypeDef Type;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	bool bRecruited = false;

	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	FVector Home = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> EarL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> EarR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Tail;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> EyeL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> EyeR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Snout;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PawL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PawR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HindL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HindR;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WingL;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WingR;

private:
	float WanderT = 0.f;
	FVector Vel = FVector::ZeroVector;
	float AnimClock = 0.f;
	FVector MeshBase = FVector::ZeroVector;
	TUniquePtr<FHolypawFluffyMotion> FluffyMotion;
};
