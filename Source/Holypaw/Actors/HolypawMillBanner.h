#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HolypawMillBanner.generated.h"

/** Polyester propaganda. Hearts make it sag, then come down for a handmade ribbon. */
UCLASS()
class HOLYPAW_API AHolypawMillBanner : public AActor
{
	GENERATED_BODY()

public:
	AHolypawMillBanner();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void SnapToHearts(int32 Hearts);
	bool IsDown() const { return ClothPitch >= 70.f; }

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Pole;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Cloth;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HandmadeRibbon;

protected:
	void Colorize(UStaticMeshComponent* Mesh, const FLinearColor& Color);
	void ApplyPitch(float Pitch);

	UStaticMesh* CubeMesh = nullptr;
	UMaterialInterface* ShapeMat = nullptr;
	float ClothPitch = 0.f;
	FVector ClothBase = FVector::ZeroVector;
};
