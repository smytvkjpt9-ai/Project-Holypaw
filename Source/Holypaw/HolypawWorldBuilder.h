#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HolypawTypes.h"
#include "Actors/HolypawShrine.h"
#include "HolypawWorldBuilder.generated.h"

class UProceduralMeshComponent;
class UInstancedStaticMeshComponent;
class UAudioComponent;
class ADirectionalLight;
class ASkyLight;
class AExponentialHeightFog;

USTRUCT()
struct FHolypawLandmark
{
	GENERATED_BODY()

	FString Name;
	FVector2D Pos = FVector2D::ZeroVector;
};

/**
 * Runtime open world: Earth-analog globe from HolypawAtlas (cities, roads, landmasses).
 * Mass geometry uses instanced kits. Same painterly/plush palette. No Megascans.
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
	FVector2D GetPeakCenter() const { return PeakCenter; }
	FVector GetTravelLocation(EHolypawZone Zone) const;
	void TickClockLighting(float DeltaSeconds);
	virtual void Tick(float DeltaSeconds) override;

	FString GetCompassLine(const FVector& From) const;
	TArray<FString> GetMapLines(const FVector& From) const;
	void TickWorldStream();
	void RequestDress(EHolypawZone Zone);
	bool IsPlayerIndoors(const FVector& WorldPos) const;
	void RefreshCityTheme();

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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> WallRose;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> WallMint;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> WallGold;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> Roofs;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> RoadTiles;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> WaterTiles;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> Cacti;

protected:
	void HideTemplateFloor();
	void SpawnAtmosphere();
	void BindKits();
	void AddKit(UInstancedStaticMeshComponent* ISM, const FVector& Loc, const FRotator& Rot, const FVector& Scale);
	void BuildTerrain();
	void ScatterFlora();
	void BuildCottage();
	void BuildRibbonDistricts();
	void BuildTidewellDistricts();
	void BuildHearthfoldDistricts();
	void BuildEmberfenDistricts();
	void BuildSnowveilDistricts();
	void BuildLanternAngelesDistricts();
	void BuildMossgateDistricts();
	void BuildPalmaDuskDistricts();
	void BuildCherryLoomDistricts();
	void BuildQuiltlandDistricts();
	void BuildDustMesaDistricts();
	void BuildClockhavenDistricts();
	void BuildVelvetSeineDistricts();
	void BuildMarbleForumDistricts();
	void BuildIvorySpireDistricts();
	void BuildSandHymnDistricts();
	void BuildCapePlushDistricts();
	void BuildSavannahBellDistricts();
	void BuildCarnivalBahiaDistricts();
	void BuildAndesLoomDistricts();
	void BuildSilkDeltaDistricts();
	void BuildSpiceHarborDistricts();
	void BuildCoralChoirDistricts();
	void BuildAuroraBoroughDistricts();
	void BuildTundraParishDistricts();
	void BuildFeltIceCampDistricts();
	void BuildSkyRift();
	void BuildRoads();
	void BuildAllSettlements();
	void BuildMountain();
	void BuildWater();
	void SpawnGameplayActors();
	void SpawnVillainAt(EHolypawVillain Id, const FVector2D& XY);
	void SpawnVillainRing(EHolypawVillain Id, const FVector2D& Center, int32 Count, float Radius);
	void BuildPolyMill();
	void SpawnPlayerStart();
	void CacheCityCoords();
	FVector2D CityXY(EHolypawZone Zone) const;
	void PlaceWaterSheet(float OriginX, float OriginY, int32 NX, int32 NY, float Step, float Z);
	void BuildTown(const FHolypawCity& City);
	void BuildRoad(const FVector2D& A, const FVector2D& B, int32 Steps, int32 Salt);
	void PlaceCamp(const FVector2D& XY, const FText& Name);
	void PlaceLantern(const FVector2D& XY, EHolypawZone Zone);
	void PlaceShrine(const FVector2D& XY, EHolypawShrineKind Kind, const FText& Name);
	void DressInterior(const FVector& Origin, EHolypawShrineKind Kind);
	void DressRoomShell(const FVector& Origin, const FLinearColor& Wall, const FLinearColor& Floor, const FLinearColor& Trim);
	void DressInnRoom(const FVector& Origin);
	void DressChapelRoom(const FVector& Origin);
	void DressWorkshopRoom(const FVector& Origin);
	void DressCottageRooms(const FVector& Origin, float GroundZ);
	void DressShopRoom(const FVector& Origin);
	void DressMillHall(const FVector& Origin);
	void DressCity(EHolypawZone Zone);
	void PlacePickup(const FVector2D& XY, FName ItemId, const FText& Label);
	void PlaceStall(const FVector2D& XY);
	void PlaceSign(const FVector2D& XY, const FText& Message);
	bool IsInAnyTown(float X, float Y, float Extra = 0.f) const;
	void FlattenNearTowns(float X, float Y, float& InOutHeight) const;
	void PlaceRangeMass(const FVector2D& Center, float ExtraH, const FLinearColor& Color, const TCHAR* Name);

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
	FVector2D LanternAngeles = FVector2D(-82000.f, -22000.f);
	FVector2D Mossgate = FVector2D(-86000.f, 18000.f);
	FVector2D PalmaDusk = FVector2D(12000.f, -72000.f);
	FVector2D CherryLoom = FVector2D(104000.f, 10000.f);
	FVector2D Quiltland = FVector2D(-62000.f, 38000.f);
	FVector2D DustMesa = FVector2D(-48000.f, -52000.f);
	FVector2D Clockhaven = FVector2D(50000.f, 34000.f);
	FVector2D VelvetSeine = FVector2D(56000.f, 22000.f);
	FVector2D MarbleForum = FVector2D(62000.f, 8000.f);
	FVector2D IvorySpire = FVector2D(58000.f, -40000.f);
	FVector2D SandHymn = FVector2D(32000.f, -58000.f);
	FVector2D CapePlush = FVector2D(18000.f, -98000.f);
	FVector2D SavannahBell = FVector2D(44000.f, -80000.f);
	FVector2D CarnivalBahia = FVector2D(-8000.f, -86000.f);
	FVector2D AndesLoom = FVector2D(-28000.f, -70000.f);
	FVector2D SilkDelta = FVector2D(80000.f, 22000.f);
	FVector2D SpiceHarbor = FVector2D(74000.f, -14000.f);
	FVector2D CoralChoir = FVector2D(110000.f, -58000.f);
	FVector2D AuroraBorough = FVector2D(8000.f, 82000.f);
	FVector2D TundraParish = FVector2D(-18000.f, 98000.f);
	FVector2D FeltIceCamp = FVector2D(2000.f, -124000.f);
	FVector2D PeakCenter = FVector2D(2000.f, 16000.f);

	int32 GridN = 168;
	float Cell = 1620.f;

	UStaticMesh* CubeMesh = nullptr;
	UStaticMesh* SphereMesh = nullptr;
	UStaticMesh* ConeMesh = nullptr;
	UStaticMesh* CylMesh = nullptr;
	UMaterialInterface* ShapeMat = nullptr;

	UPROPERTY()
	TObjectPtr<ADirectionalLight> SunLight;

	UPROPERTY()
	TObjectPtr<ASkyLight> SkyLight;

	UPROPERTY()
	TObjectPtr<AExponentialHeightFog> HeightFog;

	TArray<EHolypawZone> DressedCities;
	EHolypawZone ThemeZone = EHolypawZone::ForestCottage;
	bool bThemeInterior = false;

	UPROPERTY()
	TObjectPtr<UAudioComponent> ThemeComp;
};
