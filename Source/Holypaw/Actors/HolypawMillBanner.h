#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HolypawTypes.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HolypawMillBanner.generated.h"

class UStaticMesh;
class UMaterialInterface;
class UMaterialInstanceDynamic;

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
	void BoostDrop(float Seconds = 2.8f);
	bool IsDown() const { return ClothPitch >= 70.f; }
	EHolypawZone HomeZone = EHolypawZone::RibbonCity;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Pole;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Cloth;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HandmadeRibbon;

protected:
	void Colorize(UStaticMeshComponent* Mesh, const FLinearColor& Color, TObjectPtr<UMaterialInstanceDynamic>& Mid);
	void ApplyPitch(float Pitch);

	UStaticMesh* CubeMesh = nullptr;
	UMaterialInterface* ShapeMat = nullptr;
	float ClothPitch = 0.f;
	float DropBoost = 0.f;
	int32 ColorBucket = -1;
	FVector ClothBase = FVector::ZeroVector;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> PoleMid;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ClothMid;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> RibbonMid;
};
