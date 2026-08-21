#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HolypawTypes.h"
#include "HolypawWorldBuilder.generated.h"

class UProceduralMeshComponent;
class UInstancedStaticMeshComponent;

/**
 * Runtime open-world layout for Phase 1.
 * Same painterly/plush palette everywhere: forest cottage, lantern path, Ribbon City, Velvet Peak.
 */
UCLASS()
class HOLYPAW_API AHolypawWorldBuilder : public AActor
{
	GENERATED_BODY()

public:
	AHolypawWorldBuilder();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	void GenerateWorld();

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	EHolypawZone ResolveZone(const FVector& WorldPos) const;

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	FVector GetCottageSpawn() const { return CottageSpawn; }

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProceduralMeshComponent> TerrainMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> Trees;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> Canopies;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> Grass;

protected:
	void HideTemplateFloor();
	void SpawnAtmosphere();
	void BuildTerrain();
	void ScatterFlora();
	void BuildCottage();
	void BuildPathAndCity();
	void BuildMountain();
	void SpawnGameplayActors();
	void SpawnPlayerStart();

	UStaticMeshComponent* PlaceCube(const FVector& Loc, const FVector& Scale, const FLinearColor& Color, const FName& Name);
	void ColorMesh(UStaticMeshComponent* Mesh, const FLinearColor& Color);
	float SampleHeight(float X, float Y) const;
	float HashRand(int32 X, int32 Y, int32 Salt = 0) const;

	bool bGenerated = false;
	FVector CottageSpawn = FVector(-32000.f, 800.f, 200.f);
	FVector CityCenter = FVector(26000.f, 400.f, 0.f);
	FVector PeakCenter = FVector(2000.f, 16000.f, 0.f);

	int32 GridN = 96;
	float Cell = 1100.f;

	UStaticMesh* CubeMesh = nullptr;
	UStaticMesh* SphereMesh = nullptr;
	UStaticMesh* ConeMesh = nullptr;
	UStaticMesh* CylMesh = nullptr;
	UMaterialInterface* ShapeMat = nullptr;
};
