#include "HolypawWorldBuilder.h"
#include "Holypaw.h"
#include "HolypawGameInstance.h"
#include "Actors/WildFluffy.h"
#include "Actors/HostilePet.h"
#include "Actors/HugHuman.h"
#include "Actors/CottageRest.h"
#include "Actors/CampRest.h"
#include "Actors/FaithStall.h"
#include "Actors/Signpost.h"
#include "Actors/TravelLantern.h"
#include "Actors/HolypawPickup.h"
#include "ProceduralMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/SkyAtmosphere.h"
#include "Engine/VolumetricCloud.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/PlayerStart.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

AHolypawWorldBuilder::AHolypawWorldBuilder()
{
	PrimaryActorTick.bCanEverTick = true;

	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Terrain"));
	SetRootComponent(TerrainMesh);
	TerrainMesh->bUseAsyncCooking = true;

	Trees = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Trees"));
	Trees->SetupAttachment(RootComponent);
	Trees->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Trees->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	Canopies = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Canopies"));
	Canopies->SetupAttachment(RootComponent);
	Canopies->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Grass = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Grass"));
	Grass->SetupAttachment(RootComponent);
	Grass->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Rocks = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Rocks"));
	Rocks->SetupAttachment(RootComponent);
	Rocks->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Rocks->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	Reeds = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Reeds"));
	Reeds->SetupAttachment(RootComponent);
	Reeds->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WallRose = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallRose"));
	WallRose->SetupAttachment(RootComponent);
	WallRose->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WallRose->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	WallMint = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallMint"));
	WallMint->SetupAttachment(RootComponent);
	WallMint->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WallMint->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	WallGold = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallGold"));
	WallGold->SetupAttachment(RootComponent);
	WallGold->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WallGold->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	Roofs = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Roofs"));
	Roofs->SetupAttachment(RootComponent);
	Roofs->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Roofs->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	RoadTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RoadTiles"));
	RoadTiles->SetupAttachment(RootComponent);
	RoadTiles->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WaterTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WaterTiles"));
	WaterTiles->SetupAttachment(RootComponent);
	WaterTiles->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Cacti = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cacti"));
	Cacti->SetupAttachment(RootComponent);
	Cacti->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeFinder(TEXT("/Engine/BasicShapes/Cone.Cone"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (CubeFinder.Succeeded()) { CubeMesh = CubeFinder.Object; }
	if (SphereFinder.Succeeded()) { SphereMesh = SphereFinder.Object; }
	if (ConeFinder.Succeeded()) { ConeMesh = ConeFinder.Object; }
	if (CylFinder.Succeeded()) { CylMesh = CylFinder.Object; }
	if (MatFinder.Succeeded()) { ShapeMat = MatFinder.Object; }
}

void AHolypawWorldBuilder::BeginPlay()
{
	Super::BeginPlay();
	GenerateWorld();
}

void AHolypawWorldBuilder::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TickClockLighting(DeltaSeconds);
}

void AHolypawWorldBuilder::TickClockLighting(float DeltaSeconds)
{
	UHolypawGameInstance* GI = UHolypawGameInstance::Get(this);
	if (!GI)
	{
		return;
	}
	const float Hour = GI->GetWorldHour();
	float SunInt = 9.5f;
	FLinearColor SunCol(1.f, 0.93f, 0.82f);
	float SkyInt = 1.15f;
	FLinearColor FogCol(0.78f, 0.84f, 0.95f);
	if (Hour >= 17.f && Hour < 20.f)
	{
		SunInt = 4.2f;
		SunCol = FLinearColor(1.f, 0.55f, 0.38f);
		SkyInt = 0.7f;
		FogCol = FLinearColor(0.92f, 0.62f, 0.48f);
	}
	else if (Hour < 5.f || Hour >= 20.f)
	{
		SunInt = 0.55f;
		SunCol = FLinearColor(0.45f, 0.55f, 0.95f);
		SkyInt = 0.28f;
		FogCol = FLinearColor(0.22f, 0.28f, 0.48f);
	}
	else if (Hour >= 5.f && Hour < 8.f)
	{
		SunInt = 6.5f;
		SunCol = FLinearColor(1.f, 0.78f, 0.62f);
		SkyInt = 0.9f;
	}

	float FogDensity = 0.016f;
	if (Hour >= 17.f && Hour < 20.f)
	{
		FogDensity = 0.022f;
	}
	else if (Hour < 5.f || Hour >= 20.f)
	{
		FogDensity = 0.028f;
	}

	float Pitch = 18.f;
	if (Hour >= 5.f && Hour < 20.f)
	{
		const float T = (Hour - 5.f) / 15.f;
		Pitch = FMath::Lerp(-12.f, -70.f, FMath::Sin(T * PI));
	}

	if (APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		const FVector2D Mill = RibbonCity + FVector2D(5200.f, -800.f);
		const FVector2D P(Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y);
		const float Dist = FVector2D::Distance(P, Mill);
		if (Dist < 6200.f)
		{
			const float W = 1.f - Dist / 6200.f;
			FogDensity += 0.034f * W;
			FogCol = FMath::Lerp(FogCol, FLinearColor(0.58f, 0.54f, 0.48f), W);
			SunCol = FMath::Lerp(SunCol, FLinearColor(0.78f, 0.72f, 0.62f), W * 0.45f);
		}
	}

	if (SunLight)
	{
		SunLight->SetActorRotation(FRotator(Pitch, 35.f, 0.f));
		if (UDirectionalLightComponent* C = SunLight->FindComponentByClass<UDirectionalLightComponent>())
		{
			C->SetIntensity(FMath::FInterpTo(C->Intensity, SunInt, DeltaSeconds, 1.2f));
			C->SetLightColor(SunCol);
		}
	}
	if (SkyLight)
	{
		if (USkyLightComponent* C = SkyLight->FindComponentByClass<USkyLightComponent>())
		{
			C->SetIntensity(FMath::FInterpTo(C->Intensity, SkyInt, DeltaSeconds, 1.2f));
		}
	}
	if (HeightFog)
	{
		if (UExponentialHeightFogComponent* C = HeightFog->FindComponentByClass<UExponentialHeightFogComponent>())
		{
			C->SetFogInscatteringColor(FogCol);
			C->SetFogDensity(FogDensity);
		}
	}
}

void AHolypawWorldBuilder::GenerateWorld()
{
	if (bGenerated)
	{
		return;
	}
	bGenerated = true;
	UE_LOG(LogHolypaw, Log, TEXT("Holypaw world: Earth-analog globe, %d cities, oceans, deserts, ice."), HolypawCatalog::GetCities().Num());
	CacheCityCoords();
	BindKits();
	HideTemplateFloor();
	SpawnAtmosphere();
	BuildTerrain();
	ScatterFlora();
	BuildWater();
	BuildMountain();
	BuildCottage();
	BuildRoads();
	BuildAllSettlements();
	BuildRibbonDistricts();
	BuildSkyRift();
	SpawnGameplayActors();
	SpawnPlayerStart();
}

FName AHolypawWorldBuilder::MakeName(const TCHAR* Prefix)
{
	return FName(*FString::Printf(TEXT("%s_%d"), Prefix, NameSerial++));
}

void AHolypawWorldBuilder::HideTemplateFloor()
{
	for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
	{
		AStaticMeshActor* SMA = *It;
		if (!SMA || SMA->GetOwner() == this)
		{
			continue;
		}
		const FVector S = SMA->GetActorScale3D();
		if (S.X > 8.f && S.Y > 8.f && S.Z < 2.f)
		{
			SMA->SetActorHiddenInGame(true);
			SMA->SetActorEnableCollision(false);
		}
	}
}

void AHolypawWorldBuilder::SpawnAtmosphere()
{
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ADirectionalLight* Sun = GetWorld()->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-42.f, 35.f, 0.f), Sp))
	{
		SunLight = Sun;
		if (UDirectionalLightComponent* C = Sun->FindComponentByClass<UDirectionalLightComponent>())
		{
			C->SetIntensity(9.5f);
			C->SetLightColor(FLinearColor(1.f, 0.93f, 0.82f));
			C->SetAtmosphereSunLight(true);
			C->SetCastShadows(true);
			C->SetDynamicShadowDistanceMovableLight(90000.f);
		}
	}
	if (ASkyLight* Sky = GetWorld()->SpawnActor<ASkyLight>(FVector(0.f, 0.f, 400.f), FRotator::ZeroRotator, Sp))
	{
		SkyLight = Sky;
		if (USkyLightComponent* C = Sky->FindComponentByClass<USkyLightComponent>())
		{
			C->SetIntensity(1.15f);
			C->SetLightColor(FLinearColor(0.72f, 0.82f, 0.98f));
			C->bRealTimeCapture = true;
		}
	}
	if (AExponentialHeightFog* Fog = GetWorld()->SpawnActor<AExponentialHeightFog>(FVector(0.f, 0.f, 200.f), FRotator::ZeroRotator, Sp))
	{
		HeightFog = Fog;
		if (UExponentialHeightFogComponent* C = Fog->FindComponentByClass<UExponentialHeightFogComponent>())
		{
			C->SetFogDensity(0.016f);
			C->SetFogHeightFalloff(0.12f);
			C->SetFogInscatteringColor(FLinearColor(0.78f, 0.84f, 0.95f));
			C->SetVolumetricFog(true);
		}
	}
	GetWorld()->SpawnActor<ASkyAtmosphere>(FVector::ZeroVector, FRotator::ZeroRotator, Sp);
	GetWorld()->SpawnActor<AVolumetricCloud>(FVector::ZeroVector, FRotator::ZeroRotator, Sp);

	if (APostProcessVolume* PP = GetWorld()->SpawnActor<APostProcessVolume>(FVector::ZeroVector, FRotator::ZeroRotator, Sp))
	{
		PP->bUnbound = true;
		PP->BlendWeight = 1.f;
		PP->Settings.bOverride_AutoExposureBias = true;
		PP->Settings.AutoExposureBias = 0.35f;
		PP->Settings.bOverride_ColorSaturation = true;
		PP->Settings.ColorSaturation = FVector4(1.06f, 1.02f, 1.08f, 1.f);
		PP->Settings.bOverride_BloomIntensity = true;
		PP->Settings.BloomIntensity = 0.35f;
		PP->Settings.bOverride_VignetteIntensity = true;
		PP->Settings.VignetteIntensity = 0.28f;
		PP->Settings.bOverride_FilmGrainIntensity = true;
		PP->Settings.FilmGrainIntensity = 0.04f;
		PP->Settings.bOverride_ColorGain = true;
		PP->Settings.ColorGain = FVector4(1.02f, 0.98f, 1.05f, 1.f);
	}
}

float AHolypawWorldBuilder::HashRand(int32 X, int32 Y, int32 Salt) const
{
	uint32 H = static_cast<uint32>(X * 374761393 + Y * 668265263 + Salt * 1274126177);
	H = (H ^ (H >> 13)) * 1274126177u;
	H ^= H >> 16;
	return (H & 0xFFFF) / 65535.f;
}

bool AHolypawWorldBuilder::IsInAnyTown(float X, float Y, float Extra) const
{
	const FVector2D P(X, Y);
	for (const FHolypawCity& C : HolypawCatalog::GetCities())
	{
		if (FVector2D::Distance(P, C.Pos) < C.Radius + Extra)
		{
			return true;
		}
	}
	return false;
}

void AHolypawWorldBuilder::CacheCityCoords()
{
	RibbonCity = CityXY(EHolypawZone::RibbonCity);
	Tidewell = CityXY(EHolypawZone::Tidewell);
	Hearthfold = CityXY(EHolypawZone::Hearthfold);
	Emberfen = CityXY(EHolypawZone::Emberfen);
	Snowveil = CityXY(EHolypawZone::Snowveil);
}

FVector2D AHolypawWorldBuilder::CityXY(EHolypawZone Zone) const
{
	return HolypawCatalog::GetCity(Zone).Pos;
}

void AHolypawWorldBuilder::FlattenNearTowns(float X, float Y, float& InOutHeight) const
{
	auto Flatten = [&](const FVector2D& C, float Radius, float Target)
	{
		const float D = FVector2D::Distance(FVector2D(X, Y), C);
		if (D < Radius)
		{
			const float T = 1.f - D / Radius;
			InOutHeight = FMath::Lerp(InOutHeight, Target, T * T);
		}
	};
	for (const FHolypawCity& C : HolypawCatalog::GetCities())
	{
		Flatten(C.Pos, C.Radius * 1.15f, C.FlattenZ);
	}
	const float Fd = FVector2D::Distance(FVector2D(X, Y), FVector2D(CottageSpawn.X, CottageSpawn.Y));
	if (Fd < 3500.f)
	{
		InOutHeight = FMath::Lerp(InOutHeight, 70.f, 1.f - Fd / 3500.f);
	}
}

float AHolypawWorldBuilder::SampleHeight(float X, float Y) const
{
	const float Nx = X / 22000.f;
	const float Ny = Y / 22000.f;
	float H = 40.f + HolypawCatalog::LandHeightBias(FVector2D(X, Y));
	H += 90.f * FMath::Sin(Nx * 1.15f) * FMath::Cos(Ny * 0.95f);
	H += 48.f * FMath::Sin(Nx * 2.8f + Ny * 1.7f);
	H += 28.f * FMath::Sin(Nx * 6.1f) * FMath::Sin(Ny * 5.4f);

	const float Md = FVector2D::Distance(FVector2D(X, Y), PeakCenter);
	H += 3200.f * FMath::Exp(-FMath::Square(Md / 8500.f));
	if (Md < 3500.f)
	{
		H += 900.f * (1.f - Md / 3500.f);
	}

	const float Andes = FVector2D::Distance(FVector2D(X, Y), CityXY(EHolypawZone::AndesLoom));
	H += 1100.f * FMath::Exp(-FMath::Square(Andes / 11000.f));

	const float SilkRidge = FVector2D::Distance(FVector2D(X, Y), FVector2D(82000.f, 4000.f));
	H += 1500.f * FMath::Exp(-FMath::Square(SilkRidge / 9000.f));

	FlattenNearTowns(X, Y, H);

	const EHolypawZone ZGuess = ResolveZone(FVector(X, Y, H));
	if (ZGuess == EHolypawZone::Ocean)
	{
		H = 4.f;
	}
	if (ZGuess == EHolypawZone::Desert)
	{
		H = FMath::Lerp(H, 36.f, 0.55f);
	}
	if (ZGuess == EHolypawZone::Ice)
	{
		H += 80.f;
	}
	if (ZGuess == EHolypawZone::Jungle)
	{
		H += 18.f * FMath::Sin(X / 2400.f);
	}
	return H;
}

void AHolypawWorldBuilder::BuildTerrain()
{
	TArray<FVector> Verts;
	TArray<int32> Tris;
	TArray<FVector2D> UV;
	TArray<FColor> Colors;
	const int32 N = GridN;
	Verts.Reserve(N * N);

	for (int32 J = 0; J < N; ++J)
	{
		for (int32 I = 0; I < N; ++I)
		{
			const float X = -(N - 1) * Cell * 0.5f + I * Cell;
			const float Y = -(N - 1) * Cell * 0.5f + J * Cell;
			const float Z = SampleHeight(X, Y);
			Verts.Add(FVector(X, Y, Z));
			UV.Add(FVector2D(I / float(N - 1) * 18.f, J / float(N - 1) * 18.f));

			const EHolypawZone Zid = ResolveZone(FVector(X, Y, Z));
			Colors.Add(HolypawCatalog::ZoneTerrainColor(Zid).ToFColor(true));
		}
	}
	for (int32 J = 0; J < N - 1; ++J)
	{
		for (int32 I = 0; I < N - 1; ++I)
		{
			const int32 A = J * N + I;
			const int32 B = A + 1;
			const int32 CIdx = A + N;
			const int32 D = CIdx + 1;
			Tris.Add(A); Tris.Add(CIdx); Tris.Add(B);
			Tris.Add(B); Tris.Add(CIdx); Tris.Add(D);
		}
	}
	TerrainMesh->CreateMeshSection(0, Verts, Tris, TArray<FVector>(), UV, Colors, TArray<FProcMeshTangent>(), true);
	if (ShapeMat)
	{
		UMaterialInstanceDynamic* Mid = UMaterialInstanceDynamic::Create(ShapeMat, this);
		Mid->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.48f, 0.64f, 0.40f));
		TerrainMesh->SetMaterial(0, Mid);
	}
	TerrainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TerrainMesh->SetCollisionObjectType(ECC_WorldStatic);
}

void AHolypawWorldBuilder::ColorMesh(UStaticMeshComponent* Mesh, const FLinearColor& Color)
{
	if (!Mesh || !ShapeMat) { return; }
	if (UMaterialInstanceDynamic* Mid = Mesh->CreateDynamicMaterialInstance(0, ShapeMat))
	{
		Mid->SetVectorParameterValue(TEXT("Color"), Color);
	}
}

void AHolypawWorldBuilder::BindKits()
{
	auto Bind = [&](UInstancedStaticMeshComponent* ISM, UStaticMesh* Mesh, const FLinearColor& Color)
	{
		if (!ISM || !Mesh) { return; }
		ISM->SetStaticMesh(Mesh);
		if (ShapeMat)
		{
			UMaterialInstanceDynamic* Mid = UMaterialInstanceDynamic::Create(ShapeMat, this);
			Mid->SetVectorParameterValue(TEXT("Color"), Color);
			ISM->SetMaterial(0, Mid);
		}
	};
	Bind(Trees, CylMesh ? CylMesh : CubeMesh, FLinearColor(0.38f, 0.24f, 0.16f));
	Bind(Canopies, ConeMesh ? ConeMesh : SphereMesh, FLinearColor(0.28f, 0.52f, 0.32f));
	Bind(Grass, CubeMesh, FLinearColor(0.40f, 0.68f, 0.36f));
	Bind(Rocks, SphereMesh ? SphereMesh : CubeMesh, FLinearColor(0.55f, 0.52f, 0.48f));
	Bind(Reeds, CubeMesh, FLinearColor(0.35f, 0.42f, 0.28f));
	Bind(WallRose, CubeMesh, FLinearColor(0.85f, 0.62f, 0.58f));
	Bind(WallMint, CubeMesh, FLinearColor(0.62f, 0.75f, 0.70f));
	Bind(WallGold, CubeMesh, FLinearColor(0.90f, 0.78f, 0.55f));
	Bind(Roofs, CubeMesh, FLinearColor(0.55f, 0.28f, 0.32f));
	Bind(RoadTiles, CubeMesh, FLinearColor(0.72f, 0.62f, 0.46f));
	Bind(WaterTiles, CubeMesh, FLinearColor(0.35f, 0.55f, 0.72f));
	Bind(Cacti, ConeMesh ? ConeMesh : CubeMesh, FLinearColor(0.42f, 0.62f, 0.32f));
}

void AHolypawWorldBuilder::AddKit(UInstancedStaticMeshComponent* ISM, const FVector& Loc, const FRotator& Rot, const FVector& Scale)
{
	if (!ISM) { return; }
	ISM->AddInstance(FTransform(Rot, Loc, Scale), true);
}

UStaticMeshComponent* AHolypawWorldBuilder::PlaceCube(const FVector& Loc, const FVector& Scale, const FLinearColor& Color, const FName& Name)
{
	UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(this, Name);
	if (!Comp || !CubeMesh) { return nullptr; }
	Comp->SetStaticMesh(CubeMesh);
	Comp->SetMobility(EComponentMobility::Movable);
	Comp->CreationMethod = EComponentCreationMethod::Instance;
	if (ShapeMat) { Comp->SetMaterial(0, ShapeMat); }
	Comp->SetWorldLocation(Loc);
	Comp->SetWorldScale3D(Scale);
	Comp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Comp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	AddInstanceComponent(Comp);
	Comp->RegisterComponent();
	Comp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	ColorMesh(Comp, Color);
	return Comp;
}

void AHolypawWorldBuilder::ScatterFlora()
{
	const float Half = (GridN - 1) * Cell * 0.5f;
	int32 TreeCount = 0;
	for (int32 N = 0; N < 2800 && TreeCount < 1200; ++N)
	{
		const float X = FMath::FRandRange(-Half * 0.92f, Half * 0.92f);
		const float Y = FMath::FRandRange(-Half * 0.92f, Half * 0.92f);
		if (IsInAnyTown(X, Y, 400.f)) { continue; }
		const EHolypawZone Z = ResolveZone(FVector(X, Y, 0.f));
		if (Z == EHolypawZone::Coast || Z == EHolypawZone::Ocean || Z == EHolypawZone::Desert || Z == EHolypawZone::Ice) { continue; }
		const float Zs = SampleHeight(X, Y);
		float H = 160.f + HashRand(int32(X), int32(Y), 7) * 180.f;
		if (Z == EHolypawZone::Snow || Z == EHolypawZone::Snowveil || Z == EHolypawZone::TundraParish || Z == EHolypawZone::AuroraBorough || Z == EHolypawZone::FeltIceCamp) { H *= 0.7f; }
		if (Z == EHolypawZone::Jungle || Z == EHolypawZone::PalmaDusk || Z == EHolypawZone::Quiltland || Z == EHolypawZone::CarnivalBahia) { H *= 1.28f; }
		AddKit(Trees, FVector(X, Y, Zs + H * 0.5f), FRotator::ZeroRotator, FVector(0.45f, 0.45f, H / 100.f));
		AddKit(Canopies, FVector(X, Y, Zs + H + 80.f), FRotator::ZeroRotator, FVector(2.4f, 2.4f, 2.1f));
		++TreeCount;
	}
	int32 GrassCount = 0;
	for (int32 N = 0; N < 2000 && GrassCount < 1400; ++N)
	{
		const float X = FMath::FRandRange(-Half * 0.88f, Half * 0.88f);
		const float Y = FMath::FRandRange(-Half * 0.88f, Half * 0.88f);
		if (IsInAnyTown(X, Y)) { continue; }
		const EHolypawZone Z = ResolveZone(FVector(X, Y, 0.f));
		if (Z == EHolypawZone::Ocean || Z == EHolypawZone::Ice || Z == EHolypawZone::Desert) { continue; }
		const float Zs = SampleHeight(X, Y);
		AddKit(Grass, FVector(X, Y, Zs + 18.f), FRotator::ZeroRotator, FVector(0.12f, 0.12f, 0.35f + HashRand(int32(X), int32(Y), 11) * 0.3f));
		++GrassCount;
	}
	int32 RockCount = 0;
	for (int32 N = 0; N < 700 && RockCount < 280; ++N)
	{
		const float X = FMath::FRandRange(-Half * 0.85f, Half * 0.85f);
		const float Y = FMath::FRandRange(-Half * 0.85f, Half * 0.85f);
		if (IsInAnyTown(X, Y, 200.f)) { continue; }
		const EHolypawZone Z = ResolveZone(FVector(X, Y, 0.f));
		if (Z == EHolypawZone::Ocean) { continue; }
		const float Zs = SampleHeight(X, Y);
		const float S = 0.8f + HashRand(int32(X), int32(Y), 19) * 1.6f;
		AddKit(Rocks, FVector(X, Y, Zs + 20.f), FRotator(HashRand(int32(X), int32(Y), 4) * 40.f, HashRand(int32(X), int32(Y), 5) * 90.f, 0.f), FVector(S));
		++RockCount;
	}
	int32 ReedCount = 0;
	for (int32 N = 0; N < 500 && ReedCount < 320; ++N)
	{
		const float X = FMath::FRandRange(-20000.f, 22000.f);
		const float Y = FMath::FRandRange(-Half * 0.9f, -14000.f);
		if (IsInAnyTown(X, Y)) { continue; }
		const EHolypawZone Z = ResolveZone(FVector(X, Y, 0.f));
		if (Z == EHolypawZone::Ocean || Z == EHolypawZone::Desert) { continue; }
		const float Zs = SampleHeight(X, Y);
		AddKit(Reeds, FVector(X, Y, Zs + 40.f), FRotator::ZeroRotator, FVector(0.08f, 0.08f, 0.9f + HashRand(int32(X), int32(Y), 21) * 0.6f));
		++ReedCount;
	}
	int32 CactusCount = 0;
	for (int32 N = 0; N < 400 && CactusCount < 180; ++N)
	{
		const float X = FMath::FRandRange(-Half * 0.8f, Half * 0.8f);
		const float Y = FMath::FRandRange(-Half * 0.8f, Half * 0.4f);
		if (IsInAnyTown(X, Y, 300.f)) { continue; }
		const EHolypawZone Z = ResolveZone(FVector(X, Y, 0.f));
		if (Z != EHolypawZone::Desert && Z != EHolypawZone::DustMesa && Z != EHolypawZone::SandHymn) { continue; }
		const float Zs = SampleHeight(X, Y);
		const float H = 90.f + HashRand(int32(X), int32(Y), 31) * 110.f;
		AddKit(Cacti, FVector(X, Y, Zs + H * 0.5f), FRotator::ZeroRotator, FVector(0.55f, 0.55f, H / 100.f));
		++CactusCount;
	}
}

void AHolypawWorldBuilder::BuildCottage()
{
	const float Z = SampleHeight(CottageSpawn.X, CottageSpawn.Y);
	CottageSpawn.Z = Z + 120.f;
	PlaceCube(FVector(CottageSpawn.X, CottageSpawn.Y, Z + 160.f), FVector(6.5f, 5.2f, 3.2f), FLinearColor(0.72f, 0.48f, 0.36f), MakeName(TEXT("CottageBody")));
	PlaceCube(FVector(CottageSpawn.X, CottageSpawn.Y, Z + 340.f), FVector(7.4f, 6.0f, 1.6f), FLinearColor(0.55f, 0.28f, 0.28f), MakeName(TEXT("CottageRoof")));
	PlaceCube(FVector(CottageSpawn.X + 280.f, CottageSpawn.Y, Z + 90.f), FVector(3.2f, 3.8f, 0.25f), FLinearColor(0.62f, 0.5f, 0.38f), MakeName(TEXT("Porch")));
	PlaceCube(FVector(CottageSpawn.X + 40.f, CottageSpawn.Y - 90.f, Z + 175.f), FVector(0.9f, 0.12f, 0.9f), FLinearColor(0.55f, 0.82f, 0.95f), MakeName(TEXT("Window")));
	PlaceCube(FVector(CottageSpawn.X - 180.f, CottageSpawn.Y + 220.f, Z + 70.f), FVector(1.2f, 1.2f, 1.4f), FLinearColor(0.42f, 0.32f, 0.22f), MakeName(TEXT("Woodpile")));
	PlaceCube(FVector(CottageSpawn.X - 80.f, CottageSpawn.Y, Z + 28.f), FVector(5.4f, 4.2f, 0.12f), FLinearColor(0.62f, 0.44f, 0.32f), MakeName(TEXT("CottageFloor")));
	PlaceCube(FVector(CottageSpawn.X - 140.f, CottageSpawn.Y + 40.f, Z + 70.f), FVector(2.4f, 1.3f, 0.45f), FLinearColor(0.78f, 0.55f, 0.62f), MakeName(TEXT("CottageBed")));
	PlaceCube(FVector(CottageSpawn.X - 200.f, CottageSpawn.Y + 40.f, Z + 95.f), FVector(0.7f, 1.1f, 0.28f), FLinearColor(0.95f, 0.88f, 0.92f), MakeName(TEXT("CottagePillow")));
	PlaceCube(FVector(CottageSpawn.X + 40.f, CottageSpawn.Y + 80.f, Z + 85.f), FVector(1.1f, 1.1f, 0.9f), FLinearColor(0.55f, 0.38f, 0.28f), MakeName(TEXT("CottageTable")));
	PlaceCube(FVector(CottageSpawn.X + 310.f, CottageSpawn.Y, Z + 150.f), FVector(0.18f, 1.5f, 2.4f), FLinearColor(0.38f, 0.24f, 0.18f), MakeName(TEXT("CottageDoor")));
	PlaceCube(FVector(CottageSpawn.X + 1400.f, CottageSpawn.Y - 200.f, Z + 40.f), FVector(4.2f, 0.7f, 0.55f), FLinearColor(0.42f, 0.32f, 0.22f), MakeName(TEXT("FallenTree")));
	PlaceCube(FVector(CottageSpawn.X - 40.f, CottageSpawn.Y, Z + 280.f), FVector(4.2f, 3.4f, 0.16f), FLinearColor(0.68f, 0.5f, 0.4f), MakeName(TEXT("CottageLoft")));
	PlaceCube(FVector(CottageSpawn.X - 90.f, CottageSpawn.Y - 40.f, Z + 310.f), FVector(1.4f, 0.7f, 0.5f), FLinearColor(0.55f, 0.4f, 0.32f), MakeName(TEXT("LoftTrunk")));
	PlaceCube(FVector(CottageSpawn.X + 20.f, CottageSpawn.Y + 60.f, Z - 20.f), FVector(3.2f, 2.6f, 0.9f), FLinearColor(0.32f, 0.24f, 0.2f), MakeName(TEXT("CottageCellar")));
	PlaceCube(FVector(CottageSpawn.X + 240.f, CottageSpawn.Y + 70.f, Z + 70.f), FVector(0.7f, 0.7f, 0.55f), FLinearColor(0.72f, 0.52f, 0.38f), MakeName(TEXT("PorchChair")));
	PlacePickup(FVector2D(CottageSpawn.X + 20.f, CottageSpawn.Y + 60.f), TEXT("hymnSheet"), NSLOCTEXT("Holypaw", "HymnSheetPick", "cellar hymn sheet"));
	PlacePickup(FVector2D(CottageSpawn.X + 260.f, CottageSpawn.Y - 40.f), TEXT("stuffedPostcard"), NSLOCTEXT("Holypaw", "PostcardPick", "porch postcard"));

	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ACottageRest* C = GetWorld()->SpawnActor<ACottageRest>(FVector(CottageSpawn.X + 420.f, CottageSpawn.Y, Z + 40.f), FRotator::ZeroRotator, Sp))
	{
		C->SetActorScale3D(FVector(1.4f, 1.4f, 0.15f));
	}
	PlaceLantern(FVector2D(CottageSpawn.X + 520.f, CottageSpawn.Y + 160.f), EHolypawZone::ForestCottage);
	PlaceSign(FVector2D(CottageSpawn.X + 900.f, CottageSpawn.Y + 80.f),
		NSLOCTEXT("Holypaw", "CottageSign", "Lantern road east -> Ribbon City. N for map. Rest saves."));
}

void AHolypawWorldBuilder::BuildRoad(const FVector2D& A, const FVector2D& B, int32 Steps, int32 Salt)
{
	const FVector2D Delta = B - A;
	const float Yaw = FMath::RadiansToDegrees(FMath::Atan2(Delta.Y, Delta.X));
	const FRotator Facing(0.f, Yaw, 0.f);
	for (int32 I = 0; I <= Steps; ++I)
	{
		const float T = I / float(Steps);
		const float X = FMath::Lerp(A.X, B.X, T);
		const float Y = FMath::Lerp(A.Y, B.Y, T) + FMath::Sin(T * PI * 1.1f + Salt) * 700.f;
		const float Z = SampleHeight(X, Y);
		AddKit(RoadTiles, FVector(X, Y, Z + 8.f), Facing, FVector(7.2f, 2.0f, 0.12f));
		if (I % 4 == 0)
		{
			PlaceCube(FVector(X, Y + 150.f, Z + 90.f), FVector(0.18f, 0.18f, 1.7f), FLinearColor(0.35f, 0.28f, 0.22f), MakeName(TEXT("LampPole")));
			PlaceCube(FVector(X, Y + 150.f, Z + 180.f), FVector(0.35f, 0.35f, 0.35f), FLinearColor(1.f, 0.85f, 0.45f), MakeName(TEXT("Lamp")));
		}
	}
}

void AHolypawWorldBuilder::BuildRoads()
{
	const FVector2D Cottage(CottageSpawn.X, CottageSpawn.Y);
	BuildRoad(Cottage, RibbonCity, 32, 0);
	BuildRoad(Cottage, CityXY(EHolypawZone::Quiltland), 18, 8);
	BuildRoad(Snowveil, PeakCenter, 10, 5);
	for (const HolypawCatalog::FHolypawRoadLink& R : HolypawCatalog::GetRoads())
	{
		BuildRoad(CityXY(R.A), CityXY(R.B), R.Steps, R.Salt);
	}
}

void AHolypawWorldBuilder::BuildTown(const FHolypawCity& City)
{
	const FVector2D Center = City.Pos;
	const int32 Cols = City.Cols;
	const int32 Rows = City.Rows;
	const float Zc = SampleHeight(Center.X, Center.Y);
	AddKit(RoadTiles, FVector(Center.X, Center.Y, Zc + 12.f), FRotator::ZeroRotator, FVector(Cols * 2.6f, Rows * 2.4f, 0.2f));
	const float SpireH = City.bTallSpire ? 18.f : 10.f;
	PlaceCube(FVector(Center.X - 120.f, Center.Y + 140.f, Zc + SpireH * 50.f), FVector(2.2f, 2.2f, SpireH), City.Accent, MakeName(TEXT("Spire")));
	PlaceCube(FVector(Center.X - 120.f, Center.Y + 140.f, Zc + SpireH * 100.f + 40.f), FVector(3.2f, 3.2f, 1.1f), FLinearColor(0.95f, 0.75f, 0.35f), MakeName(TEXT("SpireCap")));

	UInstancedStaticMeshComponent* Walls[3] = { WallRose.Get(), WallMint.Get(), WallGold.Get() };
	for (int32 Row = -Rows; Row <= Rows; ++Row)
	{
		for (int32 Col = -Cols; Col <= Cols; ++Col)
		{
			if (FMath::Abs(Row) < 2 && FMath::Abs(Col) < 2) { continue; }
			const float X = Center.X + Col * 920.f + HashRand(Col, Row, 2) * 70.f;
			const float Y = Center.Y + Row * 800.f;
			const float Z = SampleHeight(X, Y);
			const float W = 2.2f + HashRand(Col, Row, 5) * 1.6f;
			const float D = 2.0f + HashRand(Col, Row, 6) * 1.3f;
			const float H = 2.8f + HashRand(Col, Row, 8) * (City.bTallSpire ? 7.f : 4.2f);
			UInstancedStaticMeshComponent* Wall = Walls[(Col + Row + 16) % 3];
			AddKit(Wall, FVector(X, Y, Z + H * 50.f), FRotator::ZeroRotator, FVector(W, D, H));
			AddKit(Roofs, FVector(X, Y, Z + H * 100.f + 36.f), FRotator::ZeroRotator, FVector(W + 0.3f, D + 0.3f, 0.5f));
		}
	}
}

void AHolypawWorldBuilder::BuildAllSettlements()
{
	for (const FHolypawCity& C : HolypawCatalog::GetCities())
	{
		BuildTown(C);
		PlaceStall(C.Pos + FVector2D(280.f, -180.f));
		PlaceLantern(C.Pos + FVector2D(420.f, 360.f), C.Zone);
		PlaceSign(C.Pos + FVector2D(-700.f, 80.f),
			FText::FromString(FString::Printf(TEXT("%s  |  %s  |  %s"),
				*C.DisplayName.ToString(), *C.Continent.ToString(), *C.Flavor.ToString())));
	}

	for (int32 I = 0; I < 7; ++I)
	{
		const float X = Tidewell.X + 2800.f + I * 420.f;
		const float Y = Tidewell.Y + 900.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 8.f), FVector(3.8f, 7.f, 0.16f), FLinearColor(0.58f, 0.56f, 0.62f), MakeName(TEXT("Dock")));
	}

	PlaceCamp(FVector2D((CottageSpawn.X + RibbonCity.X) * 0.5f, 200.f), NSLOCTEXT("Holypaw", "CampNurs", "Nursery Camp"));
	PlaceCamp(FVector2D((RibbonCity.X + Hearthfold.X) * 0.5f, (RibbonCity.Y + Hearthfold.Y) * 0.5f), NSLOCTEXT("Holypaw", "CampFarm", "Homestead Camp"));
	PlaceCamp(FVector2D((RibbonCity.X + Emberfen.X) * 0.5f, (RibbonCity.Y + Emberfen.Y) * 0.5f), NSLOCTEXT("Holypaw", "CampMire", "Hollow Camp"));
	PlaceCamp(FVector2D((Hearthfold.X + Snowveil.X) * 0.5f, (Hearthfold.Y + Snowveil.Y) * 0.5f), NSLOCTEXT("Holypaw", "CampSnow", "Ridge Camp"));
	PlaceCamp((CityXY(EHolypawZone::LanternAngeles) + CityXY(EHolypawZone::Mossgate)) * 0.5f, NSLOCTEXT("Holypaw", "CampWest", "Pacific Camp"));
	PlaceCamp((CityXY(EHolypawZone::PalmaDusk) + CityXY(EHolypawZone::SandHymn)) * 0.5f, NSLOCTEXT("Holypaw", "CampSouth", "Tropic Camp"));
	PlaceCamp((Hearthfold + CityXY(EHolypawZone::AuroraBorough)) * 0.5f, NSLOCTEXT("Holypaw", "CampNorth", "Aurora Camp"));
	PlaceCamp(FVector2D(Tidewell.X + 6000.f, Tidewell.Y), NSLOCTEXT("Holypaw", "CampFerry", "Cherry Ferry Camp"));
	PlaceCamp(FVector2D((Tidewell.X + CityXY(EHolypawZone::Clockhaven).X) * 0.5f,
		(Tidewell.Y + CityXY(EHolypawZone::Clockhaven).Y) * 0.5f), NSLOCTEXT("Holypaw", "CampChannel", "Channel Ferry Camp"));
	PlaceCamp((CityXY(EHolypawZone::Clockhaven) + CityXY(EHolypawZone::VelvetSeine)) * 0.5f, NSLOCTEXT("Holypaw", "CampOld", "Old World Camp"));
	PlaceCamp((CityXY(EHolypawZone::SpiceHarbor) + CityXY(EHolypawZone::SilkDelta)) * 0.5f, NSLOCTEXT("Holypaw", "CampSilk", "Silk Road Camp"));
	PlaceCamp((CityXY(EHolypawZone::CapePlush) + CityXY(EHolypawZone::CoralChoir)) * 0.5f, NSLOCTEXT("Holypaw", "CampReef", "Reef Ferry Camp"));
	PlaceCamp((CityXY(EHolypawZone::AndesLoom) + CityXY(EHolypawZone::CarnivalBahia)) * 0.5f, NSLOCTEXT("Holypaw", "CampAndes", "Andes Camp"));
}

void AHolypawWorldBuilder::BuildRibbonDistricts()
{
	const FVector2D Plaza = RibbonCity;
	const FVector2D Market = RibbonCity + FVector2D(1800.f, -500.f);
	const FVector2D Cloth = RibbonCity + FVector2D(-2400.f, 700.f);
	const FVector2D Harbor = RibbonCity + FVector2D(4200.f, 1600.f);
	const FVector2D Quiet = RibbonCity + FVector2D(200.f, -3400.f);

	PlaceSign(Plaza + FVector2D(80.f, -80.f), NSLOCTEXT("Holypaw", "DistPlaza", "Ribbon Plaza  |  hug, rest, look up at the spire"));
	PlaceSign(Market + FVector2D(-200.f, 0.f), NSLOCTEXT("Holypaw", "DistMarket", "Market  |  stalls, faith for AP, no factory smiles"));
	PlaceSign(Cloth + FVector2D(0.f, 0.f), NSLOCTEXT("Holypaw", "DistCloth", "Cloth Quarter  |  handmade banners, not polyester"));
	PlaceSign(Harbor + FVector2D(0.f, 0.f), NSLOCTEXT("Holypaw", "DistHarbor", "Harbor Steps  |  east to Tidewell and the Plush Sea"));
	PlaceSign(Quiet + FVector2D(0.f, 0.f), NSLOCTEXT("Holypaw", "DistQuiet", "Quiet Rows  |  windows lit, few hostiles on the street"));

	PlaceStall(Market);
	PlaceStall(Market + FVector2D(260.f, 120.f));
	PlaceStall(Market + FVector2D(-180.f, 200.f));

	PlaceShrine(Plaza + FVector2D(-180.f, -220.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "Fountain", "Ribbon Fountain"));
	PlaceShrine(Plaza + FVector2D(520.f, 180.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "InnName", "Spire Inn"));
	PlaceShrine(Plaza + FVector2D(-480.f, 420.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "ChapelName", "Bear Chapel"));
	PlaceShrine(Cloth + FVector2D(180.f, 160.f), EHolypawShrineKind::Workshop, NSLOCTEXT("Holypaw", "LoftName", "Cloth Loft"));
	PlaceShrine(Harbor + FVector2D(-220.f, 80.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "CrateName", "Harbor Crate"));
	PlaceCube(FVector(Plaza.X + 40.f, Plaza.Y - 40.f, SampleHeight(Plaza.X, Plaza.Y) + 70.f), FVector(1.6f, 1.6f, 0.22f), FLinearColor(0.55f, 0.75f, 0.92f), MakeName(TEXT("FountainPool")));
	PlaceCube(FVector(Plaza.X + 40.f, Plaza.Y - 40.f, SampleHeight(Plaza.X, Plaza.Y) + 130.f), FVector(0.35f, 0.35f, 1.1f), FLinearColor(0.85f, 0.88f, 0.95f), MakeName(TEXT("FountainJet")));

	const FVector2D Inn = Plaza + FVector2D(520.f, 180.f);
	PlaceCube(FVector(Inn.X + 90.f, Inn.Y, SampleHeight(Inn.X, Inn.Y) + 90.f), FVector(0.35f, 0.12f, 0.55f), FLinearColor(1.f, 0.82f, 0.55f), MakeName(TEXT("InnLamp")));
	PlaceCube(FVector(Inn.X - 70.f, Inn.Y + 40.f, SampleHeight(Inn.X, Inn.Y) + 70.f), FVector(0.8f, 0.35f, 0.18f), FLinearColor(0.62f, 0.38f, 0.28f), MakeName(TEXT("InnBed")));

	const FVector2D Chapel = Plaza + FVector2D(-480.f, 420.f);
	PlaceCube(FVector(Chapel.X + 70.f, Chapel.Y - 40.f, SampleHeight(Chapel.X, Chapel.Y) + 28.f), FVector(0.9f, 0.28f, 0.22f), FLinearColor(0.82f, 0.72f, 0.48f), MakeName(TEXT("PewA")));
	PlaceCube(FVector(Chapel.X + 70.f, Chapel.Y + 40.f, SampleHeight(Chapel.X, Chapel.Y) + 28.f), FVector(0.9f, 0.28f, 0.22f), FLinearColor(0.82f, 0.72f, 0.48f), MakeName(TEXT("PewB")));

	const FVector2D Loft = Cloth + FVector2D(180.f, 160.f);
	PlaceCube(FVector(Loft.X + 80.f, Loft.Y, SampleHeight(Loft.X, Loft.Y) + 50.f), FVector(0.9f, 0.45f, 0.35f), FLinearColor(0.55f, 0.42f, 0.62f), MakeName(TEXT("Loom")));
	PlaceSign(RibbonCity + FVector2D(4700.f, -620.f), NSLOCTEXT("Holypaw", "MillProtest", "Handmade not polyester  |  clap, don't stamp"));

	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Cloth.X + I * 220.f;
		const float Y = Cloth.Y - 80.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 140.f), FVector(0.12f, 0.12f, 2.4f), FLinearColor(0.35f, 0.22f, 0.18f), MakeName(TEXT("BannerPole")));
		PlaceCube(FVector(X + 40.f, Y, Z + 200.f), FVector(0.9f, 0.08f, 1.1f), FLinearColor(0.85f, 0.42f, 0.58f), MakeName(TEXT("Banner"));
	}

	for (int32 I = 0; I < 6; ++I)
	{
		const float X = Harbor.X + I * 180.f;
		const float Y = Harbor.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 8.f + I * 12.f), FVector(2.4f, 6.5f, 0.14f), FLinearColor(0.62f, 0.58f, 0.52f), MakeName(TEXT("HarborStep")));
	}

	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Quiet.X + (I % 2) * 700.f - 200.f;
		const float Y = Quiet.Y + (I / 2) * 600.f;
		const float Z = SampleHeight(X, Y);
		AddKit(WallRose, FVector(X, Y, Z + 140.f), FRotator::ZeroRotator, FVector(2.0f, 1.8f, 2.6f));
		AddKit(Roofs, FVector(X, Y, Z + 280.f), FRotator::ZeroRotator, FVector(2.3f, 2.1f, 0.45f));
	}
}

void AHolypawWorldBuilder::BuildSkyRift()
{
	const float Z = SampleHeight(PeakCenter.X, PeakCenter.Y);
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 3200.f), FVector(14.f, 14.f, 0.35f), FLinearColor(0.72f, 0.55f, 1.f), MakeName(TEXT("SkyRiftRing")));
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 2800.f), FVector(1.1f, 1.1f, 12.f), FLinearColor(1.f, 0.88f, 0.55f), MakeName(TEXT("SkyRiftBeam")));
	PlaceCube(FVector(PeakCenter.X + 900.f, PeakCenter.Y - 400.f, Z + 3400.f), FVector(3.5f, 0.8f, 0.5f), FLinearColor(0.95f, 0.7f, 0.45f), MakeName(TEXT("SkyRiftCloud")));
	PlaceCube(FVector(PeakCenter.X - 800.f, PeakCenter.Y, Z + 3300.f), FVector(4.2f, 1.0f, 0.55f), FLinearColor(0.85f, 0.62f, 0.95f), MakeName(TEXT("SkyRiftCloudB")));
}

void AHolypawWorldBuilder::PlaceRangeMass(const FVector2D& Center, float ExtraH, const FLinearColor& Color, const TCHAR* Name)
{
	const float Z = SampleHeight(Center.X, Center.Y);
	PlaceCube(FVector(Center.X, Center.Y, Z + ExtraH * 0.35f), FVector(16.f, 12.f, ExtraH / 80.f), Color, MakeName(Name));
}

void AHolypawWorldBuilder::BuildMountain()
{
	const float Z = SampleHeight(PeakCenter.X, PeakCenter.Y);
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y, Z + 600.f), FVector(22.f, 18.f, 14.f), FLinearColor(0.58f, 0.55f, 0.62f), MakeName(TEXT("PeakMass")));
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y, Z + 1600.f), FVector(8.f, 7.f, 8.f), FLinearColor(0.88f, 0.90f, 0.95f), MakeName(TEXT("PeakSnow")));
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 2100.f), FVector(2.2f, 2.2f, 1.4f), FLinearColor(0.85f, 0.7f, 1.f), MakeName(TEXT("Shrine")));
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 2280.f), FVector(0.9f, 0.9f, 0.9f), FLinearColor(1.f, 0.9f, 0.45f), MakeName(TEXT("ShrineOrb")));
	PlaceRangeMass(CityXY(EHolypawZone::AndesLoom) + FVector2D(-1800.f, 400.f), 900.f, FLinearColor(0.52f, 0.42f, 0.34f), TEXT("AndesMass"));
	PlaceRangeMass(FVector2D(82000.f, 4000.f), 1200.f, FLinearColor(0.62f, 0.58f, 0.55f), TEXT("SilkRidge"));
}

void AHolypawWorldBuilder::BuildWater()
{
	PlaceWaterSheet(38000.f, -8000.f, 10, 10, 1800.f, 10.f);
	PlaceWaterSheet(-118000.f, -50000.f, 8, 12, 2400.f, 6.f);
	PlaceWaterSheet(118000.f, -20000.f, 6, 10, 2200.f, 8.f);
	PlaceWaterSheet(-8000.f, -118000.f, 10, 5, 2200.f, 4.f);
	PlaceWaterSheet(96000.f, 2000.f, 5, 6, 1800.f, 10.f);
	PlaceWaterSheet(43000.f, 8000.f, 6, 8, 1600.f, 9.f);
	PlaceWaterSheet(90000.f, -40000.f, 8, 6, 2000.f, 7.f);
}

void AHolypawWorldBuilder::PlaceWaterSheet(float OriginX, float OriginY, int32 NX, int32 NY, float Step, float Z)
{
	for (int32 I = 0; I < NX; ++I)
	{
		for (int32 J = 0; J < NY; ++J)
		{
			const float X = OriginX + I * Step;
			const float Y = OriginY + J * Step;
			if (IsInAnyTown(X, Y, 800.f))
			{
				continue;
			}
			if (HolypawCatalog::LandHeightBias(FVector2D(X, Y)) > 40.f)
			{
				continue;
			}
			AddKit(WaterTiles, FVector(X, Y, Z), FRotator::ZeroRotator, FVector(Step / 100.f, Step / 100.f, 0.18f));
		}
	}
}

void AHolypawWorldBuilder::PlaceCamp(const FVector2D& XY, const FText& Name)
{
	const float Z = SampleHeight(XY.X, XY.Y);
	PlaceCube(FVector(XY.X, XY.Y, Z + 70.f), FVector(2.4f, 2.4f, 1.4f), FLinearColor(0.62f, 0.42f, 0.28f), MakeName(TEXT("Tent")));
	PlaceCube(FVector(XY.X + 80.f, XY.Y, Z + 40.f), FVector(0.4f, 0.4f, 0.5f), FLinearColor(1.f, 0.55f, 0.2f), MakeName(TEXT("Fire")));
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ACampRest* C = GetWorld()->SpawnActor<ACampRest>(FVector(XY.X, XY.Y, Z + 30.f), FRotator::ZeroRotator, Sp))
	{
		C->CampName = Name;
		C->SetActorScale3D(FVector(1.2f, 1.2f, 0.2f));
	}
}

void AHolypawWorldBuilder::PlaceLantern(const FVector2D& XY, EHolypawZone Zone)
{
	const float Z = SampleHeight(XY.X, XY.Y);
	PlaceCube(FVector(XY.X, XY.Y, Z + 90.f), FVector(0.18f, 0.18f, 1.7f), FLinearColor(0.35f, 0.28f, 0.22f), MakeName(TEXT("TravelPole")));
	PlaceCube(FVector(XY.X, XY.Y, Z + 180.f), FVector(0.45f, 0.45f, 0.45f), FLinearColor(1.f, 0.86f, 0.42f), MakeName(TEXT("TravelGlow")));
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ATravelLantern* L = GetWorld()->SpawnActor<ATravelLantern>(FVector(XY.X, XY.Y, Z + 40.f), FRotator::ZeroRotator, Sp))
	{
		L->AnchorZone = Zone;
	}
}

void AHolypawWorldBuilder::PlaceShrine(const FVector2D& XY, EHolypawShrineKind Kind, const FText& Name)
{
	const float Z = SampleHeight(XY.X, XY.Y);
	FLinearColor Col(0.78f, 0.52f, 0.42f);
	FVector Scale(2.6f, 2.2f, 2.8f);
	if (Kind == EHolypawShrineKind::Chapel)
	{
		Col = FLinearColor(0.92f, 0.82f, 0.55f);
		Scale = FVector(2.2f, 2.2f, 4.2f);
	}
	else if (Kind == EHolypawShrineKind::Workshop)
	{
		Col = FLinearColor(0.72f, 0.48f, 0.68f);
		Scale = FVector(2.8f, 2.0f, 2.4f);
	}
	else if (Kind == EHolypawShrineKind::Wish)
	{
		Col = FLinearColor(0.42f, 0.68f, 0.88f);
		Scale = FVector(1.8f, 1.8f, 0.35f);
	}
	else if (Kind == EHolypawShrineKind::Crate)
	{
		Col = FLinearColor(0.55f, 0.45f, 0.32f);
		Scale = FVector(1.1f, 0.9f, 0.8f);
	}
	PlaceCube(FVector(XY.X, XY.Y, Z + Scale.Z * 50.f), Scale, Col, MakeName(TEXT("ShrineBody")));
	if (Kind == EHolypawShrineKind::Chapel)
	{
		PlaceCube(FVector(XY.X, XY.Y, Z + Scale.Z * 100.f + 40.f), FVector(1.4f, 1.4f, 0.5f), FLinearColor(0.95f, 0.75f, 0.35f), MakeName(TEXT("ChapelCap")));
	}
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (AHolypawShrine* S = GetWorld()->SpawnActor<AHolypawShrine>(FVector(XY.X, XY.Y, Z + 36.f), FRotator::ZeroRotator, Sp))
	{
		S->Kind = Kind;
		S->PlaceName = Name;
		S->SetActorScale3D(FVector(1.1f, 1.1f, 0.25f));
	}
}

void AHolypawWorldBuilder::PlacePickup(const FVector2D& XY, FName ItemId, const FText& Label)
{
	const float Z = SampleHeight(XY.X, XY.Y);
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (AHolypawPickup* P = GetWorld()->SpawnActor<AHolypawPickup>(FVector(XY.X, XY.Y, Z + 28.f), FRotator::ZeroRotator, Sp))
	{
		P->ItemId = ItemId;
		P->Label = Label;
		P->SetActorScale3D(FVector(0.55f, 0.7f, 0.12f));
	}
}

FVector AHolypawWorldBuilder::GetTravelLocation(EHolypawZone Zone) const
{
	if (Zone == EHolypawZone::ForestCottage)
	{
		return CottageSpawn + FVector(420.f, 0.f, 40.f);
	}
	const FHolypawCity City = HolypawCatalog::GetCity(Zone);
	const FVector2D XY = City.Pos.IsNearlyZero() ? CityXY(Zone) : City.Pos;
	return FVector(XY.X + 420.f, XY.Y + 360.f, SampleHeight(XY.X, XY.Y) + 80.f);
}

void AHolypawWorldBuilder::PlaceStall(const FVector2D& XY)
{
	const float Z = SampleHeight(XY.X, XY.Y);
	PlaceCube(FVector(XY.X, XY.Y, Z + 80.f), FVector(1.8f, 1.4f, 1.5f), FLinearColor(0.95f, 0.78f, 0.4f), MakeName(TEXT("Stall")));
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (AFaithStall* S = GetWorld()->SpawnActor<AFaithStall>(FVector(XY.X, XY.Y + 80.f, Z + 40.f), FRotator::ZeroRotator, Sp))
	{
		S->SetActorScale3D(FVector(0.9f, 0.9f, 0.5f));
	}
}

void AHolypawWorldBuilder::PlaceSign(const FVector2D& XY, const FText& Message)
{
	const float Z = SampleHeight(XY.X, XY.Y);
	PlaceCube(FVector(XY.X, XY.Y, Z + 70.f), FVector(0.16f, 0.16f, 1.4f), FLinearColor(0.4f, 0.3f, 0.2f), MakeName(TEXT("SignPole")));
	PlaceCube(FVector(XY.X, XY.Y, Z + 150.f), FVector(1.1f, 0.12f, 0.7f), FLinearColor(0.72f, 0.58f, 0.4f), MakeName(TEXT("SignBoard")));
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ASignpost* S = GetWorld()->SpawnActor<ASignpost>(FVector(XY.X, XY.Y, Z + 40.f), FRotator::ZeroRotator, Sp))
	{
		S->Message = Message;
		S->SetActorScale3D(FVector(0.5f, 0.5f, 1.2f));
	}
}

void AHolypawWorldBuilder::SpawnGameplayActors()
{
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const TArray<FFluffyTypeDef> Types = HolypawCatalog::MakeFluffyTypes();

	auto SpawnFluffy = [&](int32 TypeIndex, const FVector2D& XY)
	{
		const float Z = SampleHeight(XY.X, XY.Y) + 40.f;
		if (AWildFluffy* F = GetWorld()->SpawnActor<AWildFluffy>(FVector(XY.X, XY.Y, Z), FRotator::ZeroRotator, Sp))
		{
			F->Type = Types[FMath::Clamp(TypeIndex, 0, Types.Num() - 1)];
		}
	};
	SpawnFluffy(0, FVector2D(CottageSpawn.X + 700.f, CottageSpawn.Y + 400.f));
	SpawnFluffy(2, FVector2D(CottageSpawn.X + 500.f, CottageSpawn.Y - 600.f));
	SpawnFluffy(1, FVector2D(-18000.f, 1200.f));
	SpawnFluffy(0, FVector2D(-14000.f, -2000.f));
	SpawnFluffy(3, FVector2D(-8000.f, -4000.f));
	SpawnFluffy(1, FVector2D(-4000.f, 6000.f));
	SpawnFluffy(4, FVector2D(8000.f, 6000.f));
	SpawnFluffy(2, FVector2D(12000.f, -7000.f));
	SpawnFluffy(0, FVector2D(RibbonCity.X - 7000.f, -1800.f));
	SpawnFluffy(2, FVector2D(Tidewell.X - 2500.f, Tidewell.Y - 800.f));
	SpawnFluffy(1, FVector2D(Hearthfold.X + 1800.f, Hearthfold.Y - 900.f));
	SpawnFluffy(3, FVector2D(Emberfen.X - 1500.f, Emberfen.Y + 1200.f));
	SpawnFluffy(4, FVector2D(Snowveil.X + 1400.f, Snowveil.Y - 900.f));
	SpawnFluffy(5, FVector2D(PeakCenter.X + 1800.f, PeakCenter.Y - 2200.f));
	SpawnFluffy(5, FVector2D(Emberfen.X + 2200.f, Emberfen.Y - 800.f));

	auto SpawnHuman = [&](const TCHAR* Name, const FVector2D& XY, const FLinearColor& Shirt)
	{
		const float Z = SampleHeight(XY.X, XY.Y) + 50.f;
		if (AHugHuman* H = GetWorld()->SpawnActor<AHugHuman>(FVector(XY.X, XY.Y, Z), FRotator::ZeroRotator, Sp))
		{
			H->PersonName = FText::FromString(Name);
			H->ShirtColor = Shirt;
			H->SetSolidColor(Shirt);
		}
	};
	SpawnHuman(TEXT("Park Ranger"), FVector2D(CottageSpawn.X + 900.f, CottageSpawn.Y + 200.f), FLinearColor(0.45f, 0.75f, 0.55f));
	SpawnHuman(TEXT("Skeptic"), FVector2D(CottageSpawn.X + 1100.f, CottageSpawn.Y - 280.f), FLinearColor(0.62f, 0.58f, 0.7f));
	SpawnHuman(TEXT("Child"), FVector2D(-16000.f, 400.f), FLinearColor(0.55f, 0.78f, 0.95f));
	SpawnHuman(TEXT("Baker"), FVector2D(RibbonCity.X + 400.f, RibbonCity.Y - 500.f), FLinearColor(0.95f, 0.75f, 0.4f));
	SpawnHuman(TEXT("Acolyte"), FVector2D(RibbonCity.X - 300.f, RibbonCity.Y + 600.f), FLinearColor(0.72f, 0.62f, 0.9f));
	SpawnHuman(TEXT("Mayor"), FVector2D(RibbonCity.X + 200.f, RibbonCity.Y + 200.f), FLinearColor(0.85f, 0.55f, 0.7f));
	SpawnHuman(TEXT("Market Hawker"), FVector2D(RibbonCity.X + 1700.f, RibbonCity.Y - 480.f), FLinearColor(0.92f, 0.62f, 0.4f));
	SpawnHuman(TEXT("Cloth Dyer"), FVector2D(RibbonCity.X - 2300.f, RibbonCity.Y + 680.f), FLinearColor(0.78f, 0.45f, 0.62f));
	SpawnHuman(TEXT("Harbor Kid"), FVector2D(RibbonCity.X + 4000.f, RibbonCity.Y + 1500.f), FLinearColor(0.5f, 0.72f, 0.85f));
	SpawnHuman(TEXT("Quiet Neighbor"), FVector2D(RibbonCity.X + 180.f, RibbonCity.Y - 3300.f), FLinearColor(0.72f, 0.68f, 0.82f));
	SpawnHuman(TEXT("Innkeep"), FVector2D(RibbonCity.X + 520.f, RibbonCity.Y + 220.f), FLinearColor(0.78f, 0.48f, 0.38f));
	SpawnHuman(TEXT("Choir Bear"), FVector2D(RibbonCity.X - 500.f, RibbonCity.Y + 440.f), FLinearColor(0.92f, 0.78f, 0.55f));
	SpawnHuman(TEXT("Seamstress"), FVector2D(RibbonCity.X - 2200.f, RibbonCity.Y + 820.f), FLinearColor(0.82f, 0.52f, 0.72f));
	SpawnHuman(TEXT("Dock Poet"), FVector2D(RibbonCity.X + 4100.f, RibbonCity.Y + 1680.f), FLinearColor(0.48f, 0.62f, 0.78f));
	SpawnHuman(TEXT("Lamp Lighter"), FVector2D(RibbonCity.X + 80.f, RibbonCity.Y - 3100.f), FLinearColor(1.f, 0.82f, 0.42f));
	SpawnHuman(TEXT("Spire Guard"), FVector2D(RibbonCity.X - 80.f, RibbonCity.Y + 80.f), FLinearColor(0.62f, 0.55f, 0.72f));
	SpawnHuman(TEXT("Mill Whistleblower"), FVector2D(RibbonCity.X + 4800.f, RibbonCity.Y - 700.f), FLinearColor(0.7f, 0.68f, 0.6f));
	SpawnHuman(TEXT("Plaza Florist"), FVector2D(RibbonCity.X - 160.f, RibbonCity.Y - 280.f), FLinearColor(0.88f, 0.45f, 0.62f));
	SpawnHuman(TEXT("Night Watch"), FVector2D(RibbonCity.X + 240.f, RibbonCity.Y - 3600.f), FLinearColor(0.35f, 0.42f, 0.62f));
	SpawnHuman(TEXT("Harbor Hand"), FVector2D(Tidewell.X + 400.f, Tidewell.Y + 200.f), FLinearColor(0.45f, 0.55f, 0.7f));
	SpawnHuman(TEXT("Net Weaver"), FVector2D(Tidewell.X - 200.f, Tidewell.Y - 300.f), FLinearColor(0.4f, 0.7f, 0.75f));
	SpawnHuman(TEXT("Farmer"), FVector2D(Hearthfold.X + 250.f, Hearthfold.Y - 200.f), FLinearColor(0.7f, 0.6f, 0.3f));
	SpawnHuman(TEXT("Miller"), FVector2D(Hearthfold.X - 350.f, Hearthfold.Y + 150.f), FLinearColor(0.65f, 0.5f, 0.35f));
	SpawnHuman(TEXT("Fen Guide"), FVector2D(Emberfen.X + 180.f, Emberfen.Y + 80.f), FLinearColor(0.55f, 0.35f, 0.32f));
	SpawnHuman(TEXT("Mud Sculptor"), FVector2D(Emberfen.X - 220.f, Emberfen.Y - 120.f), FLinearColor(0.5f, 0.38f, 0.3f));
	SpawnHuman(TEXT("Snow Warden"), FVector2D(Snowveil.X + 160.f, Snowveil.Y - 80.f), FLinearColor(0.8f, 0.85f, 0.95f));
	SpawnHuman(TEXT("Peak Acolyte"), FVector2D(PeakCenter.X - 600.f, PeakCenter.Y - 900.f), FLinearColor(0.75f, 0.65f, 0.95f));
	SpawnHuman(TEXT("Studio Grip"), CityXY(EHolypawZone::LanternAngeles) + FVector2D(200.f, -150.f), FLinearColor(0.95f, 0.78f, 0.35f));
	SpawnHuman(TEXT("Fog Baker"), CityXY(EHolypawZone::Mossgate) + FVector2D(-120.f, 80.f), FLinearColor(0.55f, 0.72f, 0.68f));
	SpawnHuman(TEXT("Quilt Ranger"), CityXY(EHolypawZone::Quiltland) + FVector2D(80.f, -60.f), FLinearColor(0.42f, 0.58f, 0.4f));
	SpawnHuman(TEXT("Mesa Guide"), CityXY(EHolypawZone::DustMesa) + FVector2D(40.f, 90.f), FLinearColor(0.85f, 0.6f, 0.35f));
	SpawnHuman(TEXT("Palm Singer"), CityXY(EHolypawZone::PalmaDusk) + FVector2D(-80.f, 40.f), FLinearColor(0.35f, 0.78f, 0.55f));
	SpawnHuman(TEXT("Ivory Clerk"), CityXY(EHolypawZone::IvorySpire) + FVector2D(60.f, -40.f), FLinearColor(0.9f, 0.86f, 0.75f));
	SpawnHuman(TEXT("Sand Priest"), CityXY(EHolypawZone::SandHymn) + FVector2D(90.f, 20.f), FLinearColor(0.92f, 0.74f, 0.4f));
	SpawnHuman(TEXT("Cape Lookout"), CityXY(EHolypawZone::CapePlush) + FVector2D(-50.f, 70.f), FLinearColor(0.55f, 0.45f, 0.7f));
	SpawnHuman(TEXT("Loom Weaver"), CityXY(EHolypawZone::CherryLoom) + FVector2D(140.f, -90.f), FLinearColor(0.92f, 0.5f, 0.6f));
	SpawnHuman(TEXT("Aurora Child"), CityXY(EHolypawZone::AuroraBorough) + FVector2D(-70.f, 40.f), FLinearColor(0.55f, 0.8f, 0.95f));
	SpawnHuman(TEXT("Tundra Keeper"), CityXY(EHolypawZone::TundraParish) + FVector2D(30.f, -40.f), FLinearColor(0.8f, 0.85f, 0.95f));
	SpawnHuman(TEXT("Confetti Baker"), CityXY(EHolypawZone::CarnivalBahia) + FVector2D(120.f, -80.f), FLinearColor(0.95f, 0.5f, 0.4f));
	SpawnHuman(TEXT("Wool Climber"), CityXY(EHolypawZone::AndesLoom) + FVector2D(-90.f, 70.f), FLinearColor(0.72f, 0.5f, 0.35f));
	SpawnHuman(TEXT("Bell Warden"), CityXY(EHolypawZone::Clockhaven) + FVector2D(80.f, 40.f), FLinearColor(0.6f, 0.66f, 0.78f));
	SpawnHuman(TEXT("Ribbon Baker"), CityXY(EHolypawZone::VelvetSeine) + FVector2D(-60.f, -50.f), FLinearColor(0.82f, 0.55f, 0.74f));
	SpawnHuman(TEXT("Column Sitter"), CityXY(EHolypawZone::MarbleForum) + FVector2D(70.f, 90.f), FLinearColor(0.9f, 0.84f, 0.7f));
	SpawnHuman(TEXT("Grass Bell"), CityXY(EHolypawZone::SavannahBell) + FVector2D(-40.f, 60.f), FLinearColor(0.88f, 0.72f, 0.35f));
	SpawnHuman(TEXT("Delta Weaver"), CityXY(EHolypawZone::SilkDelta) + FVector2D(110.f, -70.f), FLinearColor(0.88f, 0.35f, 0.4f));
	SpawnHuman(TEXT("Pepper Clerk"), CityXY(EHolypawZone::SpiceHarbor) + FVector2D(-80.f, 50.f), FLinearColor(0.92f, 0.58f, 0.25f));
	SpawnHuman(TEXT("Reef Choir"), CityXY(EHolypawZone::CoralChoir) + FVector2D(50.f, -40.f), FLinearColor(0.35f, 0.78f, 0.82f));
	SpawnHuman(TEXT("Ice Scarf"), CityXY(EHolypawZone::FeltIceCamp) + FVector2D(-30.f, 40.f), FLinearColor(0.85f, 0.92f, 0.98f));

	SpawnFluffy(4, CityXY(EHolypawZone::LanternAngeles) + FVector2D(-900.f, 400.f));
	SpawnFluffy(2, CityXY(EHolypawZone::PalmaDusk) + FVector2D(700.f, -300.f));
	SpawnFluffy(0, CityXY(EHolypawZone::CherryLoom) + FVector2D(-600.f, 500.f));
	SpawnFluffy(5, CityXY(EHolypawZone::AuroraBorough) + FVector2D(800.f, -400.f));
	SpawnFluffy(3, CityXY(EHolypawZone::IvorySpire) + FVector2D(500.f, 600.f));
	SpawnFluffy(1, CityXY(EHolypawZone::CarnivalBahia) + FVector2D(-500.f, 400.f));
	SpawnFluffy(4, CityXY(EHolypawZone::Clockhaven) + FVector2D(600.f, -300.f));
	SpawnFluffy(0, CityXY(EHolypawZone::SilkDelta) + FVector2D(-700.f, 200.f));
	SpawnFluffy(2, CityXY(EHolypawZone::CoralChoir) + FVector2D(400.f, -500.f));
	SpawnFluffy(5, CityXY(EHolypawZone::FeltIceCamp) + FVector2D(300.f, 200.f));

	auto SpawnHostile = [&](EHolypawVillain Id, const FVector2D& XY)
	{
		SpawnVillainAt(Id, XY);
	};
	auto Ring = [&](EHolypawVillain Id, const FVector2D& C, int32 N, float R)
	{
		SpawnVillainRing(Id, C, N, R);
	};

	// Stuffed Park / cottage road — original Scrap Dogs plus forest pack.
	SpawnHostile(EHolypawVillain::ScrapDog, FVector2D(-12000.f, 2200.f));
	SpawnHostile(EHolypawVillain::ScrapDog, FVector2D(-24000.f, 1600.f));
	SpawnHostile(EHolypawVillain::ScrapDog, FVector2D(-20000.f, -1800.f));
	SpawnHostile(EHolypawVillain::ParkProwler, FVector2D(-28000.f, 900.f));
	SpawnHostile(EHolypawVillain::ParkProwler, FVector2D(-26000.f, -1400.f));
	SpawnHostile(EHolypawVillain::NightThread, FVector2D(-30000.f, -2200.f));
	SpawnHostile(EHolypawVillain::NightThread, FVector2D(-16000.f, 2800.f));
	SpawnHostile(EHolypawVillain::StitchedWolf, FVector2D(-21000.f, 3200.f));
	SpawnHostile(EHolypawVillain::Tatterfox, FVector2D(-8000.f, -2800.f));

	// Nursery hills roamers + original Corp Cat.
	SpawnHostile(EHolypawVillain::CorpCat, FVector2D(-2000.f, -3500.f));
	SpawnHostile(EHolypawVillain::CorpCat, FVector2D(2000.f, -6000.f));
	SpawnHostile(EHolypawVillain::ButtonThief, FVector2D(-6000.f, 500.f));
	SpawnHostile(EHolypawVillain::ButtonThief, FVector2D(4000.f, -2000.f));
	SpawnHostile(EHolypawVillain::MiracleEater, FVector2D(10000.f, 800.f));
	SpawnHostile(EHolypawVillain::MiracleEater, FVector2D(RibbonCity.X - 9000.f, 2500.f));
	SpawnHostile(EHolypawVillain::VoidRat, FVector2D(15000.f, -5000.f));

	// Ribbon City dens.
	Ring(EHolypawVillain::AlleyScrapDog, RibbonCity + FVector2D(2400.f, -2100.f), 3, 700.f);
	Ring(EHolypawVillain::PlazaCorpCat, RibbonCity + FVector2D(-1800.f, 1600.f), 2, 500.f);
	Ring(EHolypawVillain::SewerVoidRat, RibbonCity + FVector2D(400.f, -2800.f), 3, 420.f);
	SpawnHostile(EHolypawVillain::RazorPetbot, FVector2D(9000.f, 2500.f));
	SpawnHostile(EHolypawVillain::RazorPetbot, RibbonCity + FVector2D(3200.f, 900.f));
	SpawnHostile(EHolypawVillain::RibbonEnforcer, RibbonCity + FVector2D(-2400.f, -400.f));
	SpawnHostile(EHolypawVillain::RibbonEnforcer, RibbonCity + FVector2D(1800.f, 2200.f));
	SpawnHostile(EHolypawVillain::TinselGolem, RibbonCity + FVector2D(2800.f, 2800.f));
	SpawnHostile(EHolypawVillain::GoldSnipper, RibbonCity + FVector2D(600.f, -1600.f));
	SpawnHostile(EHolypawVillain::GoldSnipper, RibbonCity + FVector2D(-900.f, 900.f));
	SpawnHostile(EHolypawVillain::SilkMagistrate, RibbonCity + FVector2D(0.f, 3400.f));

	// Tidewell harbor.
	Ring(EHolypawVillain::SaltCrab, Tidewell + FVector2D(1800.f, -1400.f), 3, 650.f);
	SpawnHostile(EHolypawVillain::HarborHook, Tidewell + FVector2D(-1600.f, 900.f));
	SpawnHostile(EHolypawVillain::HarborHook, Tidewell + FVector2D(2200.f, 400.f));
	SpawnHostile(EHolypawVillain::BrineGull, Tidewell + FVector2D(800.f, 1800.f));
	SpawnHostile(EHolypawVillain::BrineGull, Tidewell + FVector2D(-400.f, -1800.f));
	Ring(EHolypawVillain::DockRat, Tidewell + FVector2D(-2000.f, -800.f), 2, 380.f);
	SpawnHostile(EHolypawVillain::BrineWarden, Tidewell + FVector2D(2800.f, 1600.f));

	// Hearthfold farms.
	Ring(EHolypawVillain::ScarecrowHound, Hearthfold + FVector2D(2200.f, 1600.f), 3, 700.f);
	SpawnHostile(EHolypawVillain::HaywireScarecrow, Hearthfold + FVector2D(-1800.f, 1400.f));
	SpawnHostile(EHolypawVillain::HaywireScarecrow, Hearthfold + FVector2D(2600.f, -900.f));
	SpawnHostile(EHolypawVillain::ThreshCat, Hearthfold + FVector2D(400.f, 2000.f));
	SpawnHostile(EHolypawVillain::ThreshCat, Hearthfold + FVector2D(-1200.f, -1600.f));
	Ring(EHolypawVillain::GrainMite, Hearthfold + FVector2D(1600.f, -1400.f), 3, 360.f);
	SpawnHostile(EHolypawVillain::HarvestOverseer, Hearthfold + FVector2D(0.f, 3200.f));

	// Emberfen mire.
	Ring(EHolypawVillain::MireLurker, Emberfen + FVector2D(2000.f, -1600.f), 3, 720.f);
	SpawnHostile(EHolypawVillain::FenWitchPet, Emberfen + FVector2D(-1400.f, 1200.f));
	SpawnHostile(EHolypawVillain::FenWitchPet, Emberfen + FVector2D(2400.f, 800.f));
	SpawnHostile(EHolypawVillain::BogLeech, Emberfen + FVector2D(-2000.f, -900.f));
	SpawnHostile(EHolypawVillain::BogLeech, Emberfen + FVector2D(900.f, -2200.f));
	SpawnHostile(EHolypawVillain::EmberToad, Emberfen + FVector2D(400.f, 1800.f));
	SpawnHostile(EHolypawVillain::EmberToad, Emberfen + FVector2D(-800.f, -1800.f));
	SpawnHostile(EHolypawVillain::BogKing, Emberfen + FVector2D(0.f, -3000.f));

	// Snowveil + ridge.
	Ring(EHolypawVillain::FrostMoth, Snowveil + FVector2D(-1600.f, 1100.f), 3, 600.f);
	SpawnHostile(EHolypawVillain::IceShardCat, Snowveil + FVector2D(1800.f, 900.f));
	SpawnHostile(EHolypawVillain::IceShardCat, Snowveil + FVector2D(-900.f, -1400.f));
	SpawnHostile(EHolypawVillain::DriftWolf, Snowveil + FVector2D(400.f, 2400.f));
	SpawnHostile(EHolypawVillain::DriftWolf, FVector2D(PeakCenter.X - 2400.f, PeakCenter.Y - 800.f));
	SpawnHostile(EHolypawVillain::AuroraWisp, Snowveil + FVector2D(2200.f, -600.f));
	SpawnHostile(EHolypawVillain::AuroraWarden, Snowveil + FVector2D(-200.f, 2800.f));

	// Velvet Peak world bosses and shades.
	SpawnHostile(EHolypawVillain::VelvetTyrant, FVector2D(PeakCenter.X + 900.f, PeakCenter.Y - 1600.f));
	SpawnHostile(EHolypawVillain::Unmaker, FVector2D(PeakCenter.X - 1400.f, PeakCenter.Y + 2200.f));
	SpawnHostile(EHolypawVillain::UnstuffedShade, FVector2D(PeakCenter.X + 1800.f, PeakCenter.Y + 400.f));
	SpawnHostile(EHolypawVillain::UnstuffedShade, FVector2D(PeakCenter.X - 2200.f, PeakCenter.Y - 2400.f));
	SpawnHostile(EHolypawVillain::PlazaCorpCat, CityXY(EHolypawZone::LanternAngeles) + FVector2D(1600.f, -900.f));
	SpawnHostile(EHolypawVillain::RazorPetbot, CityXY(EHolypawZone::LanternAngeles) + FVector2D(-1200.f, 800.f));
	SpawnHostile(EHolypawVillain::NightThread, CityXY(EHolypawZone::Mossgate) + FVector2D(900.f, -700.f));
	SpawnHostile(EHolypawVillain::StitchedWolf, CityXY(EHolypawZone::Quiltland) + FVector2D(1100.f, 600.f));
	SpawnHostile(EHolypawVillain::GoldSnipper, CityXY(EHolypawZone::DustMesa) + FVector2D(-800.f, 900.f));
	SpawnHostile(EHolypawVillain::SaltCrab, CityXY(EHolypawZone::PalmaDusk) + FVector2D(1400.f, -600.f));
	SpawnHostile(EHolypawVillain::RibbonEnforcer, CityXY(EHolypawZone::IvorySpire) + FVector2D(-1000.f, 700.f));
	SpawnHostile(EHolypawVillain::MiracleEater, CityXY(EHolypawZone::SandHymn) + FVector2D(800.f, -500.f));
	SpawnHostile(EHolypawVillain::HarborHook, CityXY(EHolypawZone::CapePlush) + FVector2D(900.f, 400.f));
	SpawnHostile(EHolypawVillain::VoidRat, CityXY(EHolypawZone::CherryLoom) + FVector2D(-1400.f, 700.f));
	SpawnHostile(EHolypawVillain::FrostMoth, CityXY(EHolypawZone::AuroraBorough) + FVector2D(1200.f, 500.f));
	SpawnHostile(EHolypawVillain::DriftWolf, CityXY(EHolypawZone::TundraParish) + FVector2D(-700.f, 600.f));
	SpawnHostile(EHolypawVillain::PlazaCorpCat, CityXY(EHolypawZone::CarnivalBahia) + FVector2D(1500.f, -800.f));
	SpawnHostile(EHolypawVillain::StitchedWolf, CityXY(EHolypawZone::AndesLoom) + FVector2D(-900.f, 700.f));
	SpawnHostile(EHolypawVillain::RibbonEnforcer, CityXY(EHolypawZone::Clockhaven) + FVector2D(1100.f, 400.f));
	SpawnHostile(EHolypawVillain::GoldSnipper, CityXY(EHolypawZone::VelvetSeine) + FVector2D(-800.f, 600.f));
	SpawnHostile(EHolypawVillain::TinselGolem, CityXY(EHolypawZone::MarbleForum) + FVector2D(900.f, -500.f));
	SpawnHostile(EHolypawVillain::ScarecrowHound, CityXY(EHolypawZone::SavannahBell) + FVector2D(-1000.f, 500.f));
	SpawnHostile(EHolypawVillain::MiracleEater, CityXY(EHolypawZone::SilkDelta) + FVector2D(1300.f, -700.f));
	SpawnHostile(EHolypawVillain::HarborHook, CityXY(EHolypawZone::SpiceHarbor) + FVector2D(-1100.f, 600.f));
	SpawnHostile(EHolypawVillain::SaltCrab, CityXY(EHolypawZone::CoralChoir) + FVector2D(800.f, 400.f));
	SpawnHostile(EHolypawVillain::IceShardCat, CityXY(EHolypawZone::FeltIceCamp) + FVector2D(-500.f, 300.f));

	PlaceSign(FVector2D(-22000.f, 400.f), NSLOCTEXT("Holypaw", "VSignPark", "Park notice  |  Scrap Dogs and Night Thread after dusk. Codex: V"));
	PlaceSign(RibbonCity + FVector2D(-500.f, 800.f), NSLOCTEXT("Holypaw", "VSignCity", "Watch  |  Silk Magistrate holds court north of the plaza"));
	PlaceSign(Tidewell + FVector2D(200.f, 400.f), NSLOCTEXT("Holypaw", "VSignTide", "Harbor watch  |  Brine Warden walks the outer dock"));
	PlaceSign(Hearthfold + FVector2D(-200.f, 400.f), NSLOCTEXT("Holypaw", "VSignFarm", "Field warning  |  Harvest Overseer north of the mill"));
	PlaceSign(Emberfen + FVector2D(100.f, 200.f), NSLOCTEXT("Holypaw", "VSignFen", "Fen warning  |  Bog King south in the deepest peat"));
	PlaceSign(Snowveil + FVector2D(-300.f, 200.f), NSLOCTEXT("Holypaw", "VSignSnow", "Ridge warning  |  Aurora Warden, then Velvet Tyrant, then The Unmaker"));
	BuildPolyMill();
}

void AHolypawWorldBuilder::SpawnVillainAt(EHolypawVillain Id, const FVector2D& XY)
{
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const float Z = SampleHeight(XY.X, XY.Y) + 40.f;
	if (AHostilePet* H = GetWorld()->SpawnActor<AHostilePet>(FVector(XY.X, XY.Y, Z), FRotator::ZeroRotator, Sp))
	{
		H->Configure(Id);
	}
}

void AHolypawWorldBuilder::SpawnVillainRing(EHolypawVillain Id, const FVector2D& Center, int32 Count, float Radius)
{
	const int32 N = FMath::Max(1, Count);
	for (int32 I = 0; I < N; ++I)
	{
		const float Ang = (2.f * PI * I) / static_cast<float>(N) + HashRand(I, static_cast<int32>(Id), 17) * 0.4f;
		const FVector2D XY = Center + FVector2D(FMath::Cos(Ang) * Radius, FMath::Sin(Ang) * Radius);
		SpawnVillainAt(Id, XY);
	}
}

void AHolypawWorldBuilder::BuildPolyMill()
{
	const FVector2D Mill = RibbonCity + FVector2D(5200.f, -800.f);
	const float Z = SampleHeight(Mill.X, Mill.Y);
	for (int32 I = 0; I < 5; ++I)
	{
		PlaceCube(FVector(Mill.X + I * 280.f, Mill.Y, Z + 220.f + I * 40.f),
			FVector(2.2f, 1.6f, 4.4f + I * 0.4f),
			FLinearColor(0.55f, 0.52f, 0.48f),
			MakeName(TEXT("PolyShed")));
	}
	PlaceCube(FVector(Mill.X + 400.f, Mill.Y + 200.f, Z + 380.f), FVector(1.2f, 1.2f, 7.5f),
		FLinearColor(0.72f, 0.28f, 0.38f), MakeName(TEXT("PolyStack")));
	PlaceSign(Mill + FVector2D(-200.f, 0.f),
		NSLOCTEXT("Holypaw", "PolyMill", "POLY MILL  |  cheap polyester, identical smiles, no handmade soul"));
	SpawnVillainAt(EHolypawVillain::RazorPetbot, Mill + FVector2D(600.f, 400.f));
	SpawnVillainAt(EHolypawVillain::RibbonEnforcer, Mill + FVector2D(200.f, -500.f));
	SpawnVillainAt(EHolypawVillain::GoldSnipper, Mill + FVector2D(-300.f, 300.f));
}

void AHolypawWorldBuilder::SpawnPlayerStart()
{
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FVector Loc = CottageSpawn + FVector(420.f, 0.f, 40.f);
	if (APlayerStart* PS = GetWorld()->SpawnActor<APlayerStart>(Loc, FRotator::ZeroRotator, Sp))
	{
		PS->PlayerStartTag = FName(TEXT("Cottage"));
	}
}

EHolypawZone AHolypawWorldBuilder::ResolveZone(const FVector& WorldPos) const
{
	const FVector2D P(WorldPos.X, WorldPos.Y);
	for (const FHolypawCity& C : HolypawCatalog::GetCities())
	{
		if (FVector2D::Distance(P, C.Pos) < C.Radius)
		{
			return C.Zone;
		}
	}

	const float Md = FVector2D::Distance(P, PeakCenter);
	if (Md < 7000.f && WorldPos.Z > 900.f)
	{
		return WorldPos.Z > 1800.f ? EHolypawZone::Snow : EHolypawZone::Highlands;
	}

	return HolypawCatalog::ResolveWilderness(P);
}

FString AHolypawWorldBuilder::GetCompassLine(const FVector& From) const
{
	TArray<FHolypawLandmark> Marks;
	Marks.Add({TEXT("Cottage"), FVector2D(CottageSpawn.X, CottageSpawn.Y)});
	Marks.Add({TEXT("Velvet Peak"), PeakCenter});
	Marks.Add({TEXT("Poly Mill"), RibbonCity + FVector2D(5200.f, -800.f)});
	for (const FHolypawCity& C : HolypawCatalog::GetCities())
	{
		Marks.Add({C.DisplayName.ToString(), C.Pos});
	}
	FString BestName = TEXT("Ribbon City");
	float BestD = TNumericLimits<float>::Max();
	FVector2D BestPos = RibbonCity;
	const FVector2D P(From.X, From.Y);
	for (const FHolypawLandmark& M : Marks)
	{
		const float D = FVector2D::Distance(P, M.Pos);
		if (D < BestD && D > 900.f)
		{
			BestD = D;
			BestName = M.Name;
			BestPos = M.Pos;
		}
	}
	const FVector2D Dir = (BestPos - P).GetSafeNormal();
	FString Cardinal = TEXT("east");
	if (FMath::Abs(Dir.Y) > FMath::Abs(Dir.X))
	{
		Cardinal = Dir.Y > 0.f ? TEXT("north") : TEXT("south");
	}
	else
	{
		Cardinal = Dir.X > 0.f ? TEXT("east") : TEXT("west");
	}
	const int32 Meters = FMath::Max(1, FMath::RoundToInt(BestD / 100.f));
	return FString::Printf(TEXT("%s  %dm  %s"), *BestName, Meters, *Cardinal);
}

TArray<FString> AHolypawWorldBuilder::GetMapLines(const FVector& From) const
{
	TArray<FString> Lines;
	Lines.Add(FString::Printf(TEXT("Plush Earth  —  %d cities  (N close)"), HolypawCatalog::GetCities().Num()));
	TArray<FHolypawLandmark> Marks;
	Marks.Add({TEXT("Cottage (home)"), FVector2D(CottageSpawn.X, CottageSpawn.Y)});
	Marks.Add({TEXT("Velvet Peak shrine"), PeakCenter});
	for (const FHolypawCity& C : HolypawCatalog::GetCities())
	{
		Marks.Add({FString::Printf(TEXT("%s  [%s]"), *C.DisplayName.ToString(), *C.Continent.ToString()), C.Pos});
	}
	const FVector2D P(From.X, From.Y);
	Marks.Sort([&](const FHolypawLandmark& A, const FHolypawLandmark& B)
	{
		return FVector2D::Distance(P, A.Pos) < FVector2D::Distance(P, B.Pos);
	});
	const int32 Show = FMath::Min(16, Marks.Num());
	for (int32 I = 0; I < Show; ++I)
	{
		const int32 Meters = FMath::RoundToInt(FVector2D::Distance(P, Marks[I].Pos) / 100.f);
		Lines.Add(FString::Printf(TEXT("%s   %dm"), *Marks[I].Name, Meters));
	}
	Lines.Add(TEXT("Continents like Earth. Follow lanterns. J for the Globe Trek."));
	return Lines;
}
