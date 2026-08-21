#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HolypawTypes.h"
#include "HolypawWorldBuilder.generated.h"

class UProceduralMeshComponent;
class UInstancedStaticMeshComponent;

USTRUCT()
struct FHolypawLandmark
{
	GENERATED_BODY()

	FString Name;
	FVector2D Pos = FVector2D::ZeroVector;
};

/**
 * Runtime open world: cottage start, roads, five settlements, biomes, water.
 * Same painterly/plush palette everywhere. No Megascans.
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

	FString GetCompassLine(const FVector& From) const;
	TArray<FString> GetMapLines(const FVector& From) const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProceduralMeshComponent> TerrainMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> Trees;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> Canopies;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> Grass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> Rocks;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> Reeds;

protected:
	void HideTemplateFloor();
	void SpawnAtmosphere();
	void BuildTerrain();
	void ScatterFlora();
	void BuildCottage();
	void BuildRoads();
	void BuildAllSettlements();
	void BuildMountain();
	void BuildWater();
	void SpawnGameplayActors();
	void SpawnVillainAt(EHolypawVillain Id, const FVector2D& XY);
	void SpawnVillainRing(EHolypawVillain Id, const FVector2D& Center, int32 Count, float Radius);
	void BuildPolyMill();
	void SpawnPlayerStart();
	void BuildTown(const FVector2D& Center, const FName& Prefix, const FLinearColor& Accent, int32 Cols, int32 Rows, bool bTallSpire);
	void BuildRoad(const FVector2D& A, const FVector2D& B, int32 Steps, int32 Salt);
	void PlaceCamp(const FVector2D& XY, const FText& Name);
	void PlaceStall(const FVector2D& XY);
	void PlaceSign(const FVector2D& XY, const FText& Message);
	bool IsInAnyTown(float X, float Y, float Extra = 0.f) const;
	void FlattenNearTowns(float X, float Y, float& InOutHeight) const;

	UStaticMeshComponent* PlaceCube(const FVector& Loc, const FVector& Scale, const FLinearColor& Color, const FName& Name);
	void ColorMesh(UStaticMeshComponent* Mesh, const FLinearColor& Color);
	float SampleHeight(float X, float Y) const;
	float HashRand(int32 X, int32 Y, int32 Salt = 0) const;
	FName MakeName(const TCHAR* Prefix);

	bool bGenerated = false;
	int32 NameSerial = 0;

	FVector CottageSpawn = FVector(-32000.f, 800.f, 200.f);
	FVector2D RibbonCity = FVector2D(26000.f, 400.f);
	FVector2D Tidewell = FVector2D(40000.f, 7000.f);
	FVector2D Hearthfold = FVector2D(2000.f, 24000.f);
	FVector2D Emberfen = FVector2D(6000.f, -23000.f);
	FVector2D Snowveil = FVector2D(-2000.f, 20000.f);
	FVector2D PeakCenter = FVector2D(2000.f, 16000.f);

	int32 GridN = 120;
	float Cell = 1050.f;

	UStaticMesh* CubeMesh = nullptr;
	UStaticMesh* SphereMesh = nullptr;
	UStaticMesh* ConeMesh = nullptr;
	UStaticMesh* CylMesh = nullptr;
	UMaterialInterface* ShapeMat = nullptr;
};
