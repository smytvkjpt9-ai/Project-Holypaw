#include "HolypawWorldBuilder.h"
#include "Holypaw.h"
#include "Actors/WildFluffy.h"
#include "Actors/HostilePet.h"
#include "Actors/HugHuman.h"
#include "Actors/CottageRest.h"
#include "Actors/CampRest.h"
#include "Actors/FaithStall.h"
#include "Actors/Signpost.h"
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
#include "Engine/StaticMeshActor.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/PlayerStart.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

AHolypawWorldBuilder::AHolypawWorldBuilder()
{
	PrimaryActorTick.bCanEverTick = false;

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

void AHolypawWorldBuilder::GenerateWorld()
{
	if (bGenerated)
	{
		return;
	}
	bGenerated = true;
	UE_LOG(LogHolypaw, Log, TEXT("Holypaw world: cottage, five settlements, roads, water, biomes."));
	HideTemplateFloor();
	SpawnAtmosphere();
	BuildTerrain();
	ScatterFlora();
	BuildWater();
	BuildMountain();
	BuildCottage();
	BuildRoads();
	BuildAllSettlements();
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
		if (UDirectionalLightComponent* C = Sun->FindComponentByClass<UDirectionalLightComponent>())
		{
			C->SetIntensity(9.5f);
			C->SetLightColor(FLinearColor(1.f, 0.93f, 0.82f));
			C->SetAtmosphereSunLight(true);
			C->SetCastShadows(true);
			C->SetDynamicShadowDistanceMovableLight(50000.f);
		}
	}
	if (ASkyLight* Sky = GetWorld()->SpawnActor<ASkyLight>(FVector(0.f, 0.f, 400.f), FRotator::ZeroRotator, Sp))
	{
		if (USkyLightComponent* C = Sky->FindComponentByClass<USkyLightComponent>())
		{
			C->SetIntensity(1.15f);
			C->SetLightColor(FLinearColor(0.72f, 0.82f, 0.98f));
			C->bRealTimeCapture = true;
		}
	}
	if (AExponentialHeightFog* Fog = GetWorld()->SpawnActor<AExponentialHeightFog>(FVector(0.f, 0.f, 200.f), FRotator::ZeroRotator, Sp))
	{
		if (UExponentialHeightFogComponent* C = Fog->FindComponentByClass<UExponentialHeightFogComponent>())
		{
			C->SetFogDensity(0.016f);
			C->SetFogHeightFalloff(0.12f);
			C->SetFogInscatteringColor(FLinearColor(0.78f, 0.84f, 0.95f));
			C->SetVolumetricFog(true);
		}
	}
	GetWorld()->SpawnActor<ASkyAtmosphere>(FVector::ZeroVector, FRotator::ZeroRotator, Sp);

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
	if (FVector2D::Distance(P, RibbonCity) < 6500.f + Extra) { return true; }
	if (FVector2D::Distance(P, Tidewell) < 4200.f + Extra) { return true; }
	if (FVector2D::Distance(P, Hearthfold) < 3800.f + Extra) { return true; }
	if (FVector2D::Distance(P, Emberfen) < 3600.f + Extra) { return true; }
	if (FVector2D::Distance(P, Snowveil) < 3200.f + Extra) { return true; }
	return false;
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
	Flatten(RibbonCity, 8000.f, 55.f);
	Flatten(Tidewell, 5000.f, 28.f);
	Flatten(Hearthfold, 4500.f, 70.f);
	Flatten(Emberfen, 4200.f, 22.f);
	Flatten(Snowveil, 3800.f, 140.f);
	const float Fd = FVector2D::Distance(FVector2D(X, Y), FVector2D(CottageSpawn.X, CottageSpawn.Y));
	if (Fd < 3500.f)
	{
		InOutHeight = FMath::Lerp(InOutHeight, 70.f, 1.f - Fd / 3500.f);
	}
}

float AHolypawWorldBuilder::SampleHeight(float X, float Y) const
{
	const float Nx = X / 18000.f;
	const float Ny = Y / 18000.f;
	float H = 90.f;
	H += 140.f * FMath::Sin(Nx * 1.15f) * FMath::Cos(Ny * 0.95f);
	H += 65.f * FMath::Sin(Nx * 2.8f + Ny * 1.7f);
	H += 40.f * FMath::Sin(Nx * 6.1f) * FMath::Sin(Ny * 5.4f);

	const float Md = FVector2D::Distance(FVector2D(X, Y), PeakCenter);
	H += 3200.f * FMath::Exp(-FMath::Square(Md / 8500.f));
	if (Md < 3500.f)
	{
		H += 900.f * (1.f - Md / 3500.f);
	}

	FlattenNearTowns(X, Y, H);

	if (X > 36000.f)
	{
		H -= (X - 36000.f) * 0.045f;
	}
	if (Y < -16000.f)
	{
		H -= 25.f;
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
			FLinearColor C(0.45f, 0.62f, 0.38f);
			switch (Zid)
			{
			case EHolypawZone::ForestCottage: C = FLinearColor(0.32f, 0.52f, 0.34f); break;
			case EHolypawZone::NurseryHills: C = FLinearColor(0.55f, 0.72f, 0.40f); break;
			case EHolypawZone::RibbonCity: C = FLinearColor(0.62f, 0.58f, 0.52f); break;
			case EHolypawZone::Tidewell: C = FLinearColor(0.55f, 0.62f, 0.68f); break;
			case EHolypawZone::Hearthfold: C = FLinearColor(0.62f, 0.70f, 0.38f); break;
			case EHolypawZone::Emberfen: C = FLinearColor(0.52f, 0.34f, 0.30f); break;
			case EHolypawZone::Snowveil: C = FLinearColor(0.82f, 0.86f, 0.92f); break;
			case EHolypawZone::Homestead: C = FLinearColor(0.58f, 0.68f, 0.36f); break;
			case EHolypawZone::Coast: C = FLinearColor(0.78f, 0.72f, 0.52f); break;
			case EHolypawZone::Mire: C = FLinearColor(0.48f, 0.32f, 0.28f); break;
			case EHolypawZone::Highlands: C = FLinearColor(0.55f, 0.52f, 0.50f); break;
			case EHolypawZone::Snow: C = FLinearColor(0.90f, 0.93f, 0.97f); break;
			default: break;
			}
			Colors.Add(C.ToFColor(true));
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

	const float Half = (GridN - 1) * Cell * 0.5f;
	int32 TreeCount = 0;
	for (int32 N = 0; N < 2200 && TreeCount < 900; ++N)
	{
		const float X = FMath::FRandRange(-Half * 0.92f, Half * 0.92f);
		const float Y = FMath::FRandRange(-Half * 0.92f, Half * 0.92f);
		if (IsInAnyTown(X, Y, 400.f)) { continue; }
		const EHolypawZone Z = ResolveZone(FVector(X, Y, 0.f));
		if (Z == EHolypawZone::Coast) { continue; }
		const float Zs = SampleHeight(X, Y);
		float H = 160.f + HashRand(int32(X), int32(Y), 7) * 180.f;
		if (Z == EHolypawZone::Snow || Z == EHolypawZone::Snowveil) { H *= 0.7f; }
		Trees->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X, Y, Zs + H * 0.5f), FVector(0.45f, 0.45f, H / 100.f)), true);
		Canopies->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X, Y, Zs + H + 80.f), FVector(2.4f, 2.4f, 2.1f)), true);
		++TreeCount;
	}
	int32 GrassCount = 0;
	for (int32 N = 0; N < 1600 && GrassCount < 1100; ++N)
	{
		const float X = FMath::FRandRange(-Half * 0.88f, Half * 0.7f);
		const float Y = FMath::FRandRange(-Half * 0.7f, Half * 0.7f);
		if (IsInAnyTown(X, Y)) { continue; }
		const float Zs = SampleHeight(X, Y);
		Grass->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X, Y, Zs + 18.f), FVector(0.12f, 0.12f, 0.35f + HashRand(int32(X), int32(Y), 11) * 0.3f)), true);
		++GrassCount;
	}
	int32 RockCount = 0;
	for (int32 N = 0; N < 500 && RockCount < 220; ++N)
	{
		const float X = FMath::FRandRange(-Half * 0.85f, Half * 0.85f);
		const float Y = FMath::FRandRange(-Half * 0.85f, Half * 0.85f);
		if (IsInAnyTown(X, Y, 200.f)) { continue; }
		const float Zs = SampleHeight(X, Y);
		const float S = 0.8f + HashRand(int32(X), int32(Y), 19) * 1.6f;
		Rocks->AddInstance(FTransform(FRotator(HashRand(int32(X), int32(Y), 4) * 40.f, HashRand(int32(X), int32(Y), 5) * 90.f, 0.f), FVector(X, Y, Zs + 20.f), FVector(S)), true);
		++RockCount;
	}
	int32 ReedCount = 0;
	for (int32 N = 0; N < 400 && ReedCount < 280; ++N)
	{
		const float X = FMath::FRandRange(-8000.f, 18000.f);
		const float Y = FMath::FRandRange(-Half * 0.9f, -14000.f);
		if (IsInAnyTown(X, Y)) { continue; }
		const float Zs = SampleHeight(X, Y);
		Reeds->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X, Y, Zs + 40.f), FVector(0.08f, 0.08f, 0.9f + HashRand(int32(X), int32(Y), 21) * 0.6f)), true);
		++ReedCount;
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

	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ACottageRest* C = GetWorld()->SpawnActor<ACottageRest>(FVector(CottageSpawn.X + 420.f, CottageSpawn.Y, Z + 40.f), FRotator::ZeroRotator, Sp))
	{
		C->SetActorScale3D(FVector(1.4f, 1.4f, 0.15f));
	}
	PlaceSign(FVector2D(CottageSpawn.X + 900.f, CottageSpawn.Y + 80.f),
		NSLOCTEXT("Holypaw", "CottageSign", "Lantern road east -> Ribbon City. N for map."));
}

void AHolypawWorldBuilder::BuildRoad(const FVector2D& A, const FVector2D& B, int32 Steps, int32 Salt)
{
	for (int32 I = 0; I <= Steps; ++I)
	{
		const float T = I / float(Steps);
		const float X = FMath::Lerp(A.X, B.X, T);
		const float Y = FMath::Lerp(A.Y, B.Y, T) + FMath::Sin(T * PI * 1.1f + Salt) * 700.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 8.f), FVector(7.2f, 2.0f, 0.12f), FLinearColor(0.72f, 0.62f, 0.46f), MakeName(TEXT("Road")));
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
	BuildRoad(RibbonCity, Tidewell, 18, 1);
	BuildRoad(RibbonCity, Hearthfold, 22, 2);
	BuildRoad(RibbonCity, Emberfen, 22, 3);
	BuildRoad(Hearthfold, Snowveil, 12, 4);
	BuildRoad(Snowveil, PeakCenter, 10, 5);
}

void AHolypawWorldBuilder::BuildTown(const FVector2D& Center, const FName& Prefix, const FLinearColor& Accent, int32 Cols, int32 Rows, bool bTallSpire)
{
	const float Zc = SampleHeight(Center.X, Center.Y);
	PlaceCube(FVector(Center.X, Center.Y, Zc + 12.f), FVector(Cols * 2.6f, Rows * 2.4f, 0.2f), FLinearColor(0.70f, 0.64f, 0.56f), MakeName(*Prefix.ToString()));
	const float SpireH = bTallSpire ? 18.f : 10.f;
	PlaceCube(FVector(Center.X - 120.f, Center.Y + 140.f, Zc + SpireH * 50.f), FVector(2.2f, 2.2f, SpireH), Accent, MakeName(TEXT("Spire")));
	PlaceCube(FVector(Center.X - 120.f, Center.Y + 140.f, Zc + SpireH * 100.f + 40.f), FVector(3.2f, 3.2f, 1.1f), FLinearColor(0.95f, 0.75f, 0.35f), MakeName(TEXT("SpireCap")));

	const TArray<FLinearColor> Walls = {
		FLinearColor(0.85f, 0.62f, 0.58f), Accent,
		FLinearColor(0.72f, 0.68f, 0.82f), FLinearColor(0.62f, 0.75f, 0.70f),
		FLinearColor(0.90f, 0.78f, 0.55f)
	};
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
			const float H = 2.8f + HashRand(Col, Row, 8) * (bTallSpire ? 7.f : 4.2f);
			const FLinearColor Wall = Walls[(Col + Row + 16) % Walls.Num()];
			PlaceCube(FVector(X, Y, Z + H * 50.f), FVector(W, D, H), Wall, MakeName(TEXT("Bldg")));
			PlaceCube(FVector(X, Y, Z + H * 100.f + 36.f), FVector(W + 0.3f, D + 0.3f, 0.5f), FLinearColor(0.55f, 0.28f, 0.32f), MakeName(TEXT("Roof")));
		}
	}
}

void AHolypawWorldBuilder::BuildAllSettlements()
{
	BuildTown(RibbonCity, TEXT("RibbonPlaza"), FLinearColor(0.82f, 0.42f, 0.55f), 5, 4, true);
	BuildTown(Tidewell, TEXT("TidePlaza"), FLinearColor(0.40f, 0.62f, 0.78f), 3, 3, false);
	BuildTown(Hearthfold, TEXT("HearthPlaza"), FLinearColor(0.78f, 0.62f, 0.28f), 3, 2, false);
	BuildTown(Emberfen, TEXT("FenPlaza"), FLinearColor(0.62f, 0.32f, 0.28f), 3, 2, false);
	BuildTown(Snowveil, TEXT("SnowPlaza"), FLinearColor(0.75f, 0.82f, 0.95f), 2, 2, false);

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

	PlaceStall(RibbonCity + FVector2D(500.f, -400.f));
	PlaceStall(Tidewell + FVector2D(300.f, 200.f));
	PlaceStall(Hearthfold + FVector2D(-200.f, 250.f));
	PlaceStall(Emberfen + FVector2D(180.f, -160.f));
	PlaceStall(Snowveil + FVector2D(120.f, 80.f));

	PlaceSign(RibbonCity + FVector2D(-900.f, 0.f), NSLOCTEXT("Holypaw", "RSign", "Ribbon City  |  Tidewell east  Hearthfold north  Emberfen south"));
	PlaceSign(Tidewell + FVector2D(-600.f, 0.f), NSLOCTEXT("Holypaw", "TSign", "Tidewell Harbor  |  sea to the east"));
	PlaceSign(Hearthfold + FVector2D(0.f, -700.f), NSLOCTEXT("Holypaw", "HSign", "Hearthfold  |  Snowveil and Velvet Peak further north"));
	PlaceSign(Emberfen + FVector2D(0.f, 700.f), NSLOCTEXT("Holypaw", "ESign", "Emberfen  |  stay on the boards in the hollow"));
	PlaceSign(Snowveil + FVector2D(400.f, 0.f), NSLOCTEXT("Holypaw", "SSign", "Snowveil  |  shrine of Velvet Peak ahead"));
}

void AHolypawWorldBuilder::BuildMountain()
{
	const float Z = SampleHeight(PeakCenter.X, PeakCenter.Y);
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y, Z + 600.f), FVector(22.f, 18.f, 14.f), FLinearColor(0.58f, 0.55f, 0.62f), MakeName(TEXT("PeakMass")));
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y, Z + 1600.f), FVector(8.f, 7.f, 8.f), FLinearColor(0.88f, 0.90f, 0.95f), MakeName(TEXT("PeakSnow")));
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 2100.f), FVector(2.2f, 2.2f, 1.4f), FLinearColor(0.85f, 0.7f, 1.f), MakeName(TEXT("Shrine")));
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 2280.f), FVector(0.9f, 0.9f, 0.9f), FLinearColor(1.f, 0.9f, 0.45f), MakeName(TEXT("ShrineOrb")));
}

void AHolypawWorldBuilder::BuildWater()
{
	for (int32 I = 0; I < 18; ++I)
	{
		for (int32 J = 0; J < 8; ++J)
		{
			const float X = 38000.f + I * 1400.f;
			const float Y = -4000.f + J * 2200.f;
			PlaceCube(FVector(X, Y, 12.f), FVector(14.f, 22.f, 0.18f), FLinearColor(0.35f, 0.55f, 0.72f), MakeName(TEXT("Water")));
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
			H->SetSolidColor(Shirt);
		}
	};
	SpawnHuman(TEXT("Park Ranger"), FVector2D(CottageSpawn.X + 900.f, CottageSpawn.Y + 200.f), FLinearColor(0.45f, 0.75f, 0.55f));
	SpawnHuman(TEXT("Child"), FVector2D(-16000.f, 400.f), FLinearColor(0.55f, 0.78f, 0.95f));
	SpawnHuman(TEXT("Baker"), FVector2D(RibbonCity.X + 400.f, RibbonCity.Y - 500.f), FLinearColor(0.95f, 0.75f, 0.4f));
	SpawnHuman(TEXT("Acolyte"), FVector2D(RibbonCity.X - 300.f, RibbonCity.Y + 600.f), FLinearColor(0.72f, 0.62f, 0.9f));
	SpawnHuman(TEXT("Mayor"), FVector2D(RibbonCity.X + 200.f, RibbonCity.Y + 200.f), FLinearColor(0.85f, 0.55f, 0.7f));
	SpawnHuman(TEXT("Harbor Hand"), FVector2D(Tidewell.X + 400.f, Tidewell.Y + 200.f), FLinearColor(0.45f, 0.55f, 0.7f));
	SpawnHuman(TEXT("Net Weaver"), FVector2D(Tidewell.X - 200.f, Tidewell.Y - 300.f), FLinearColor(0.4f, 0.7f, 0.75f));
	SpawnHuman(TEXT("Farmer"), FVector2D(Hearthfold.X + 250.f, Hearthfold.Y - 200.f), FLinearColor(0.7f, 0.6f, 0.3f));
	SpawnHuman(TEXT("Miller"), FVector2D(Hearthfold.X - 350.f, Hearthfold.Y + 150.f), FLinearColor(0.65f, 0.5f, 0.35f));
	SpawnHuman(TEXT("Fen Guide"), FVector2D(Emberfen.X + 180.f, Emberfen.Y + 80.f), FLinearColor(0.55f, 0.35f, 0.32f));
	SpawnHuman(TEXT("Mud Sculptor"), FVector2D(Emberfen.X - 220.f, Emberfen.Y - 120.f), FLinearColor(0.5f, 0.38f, 0.3f));
	SpawnHuman(TEXT("Snow Warden"), FVector2D(Snowveil.X + 160.f, Snowveil.Y - 80.f), FLinearColor(0.8f, 0.85f, 0.95f));
	SpawnHuman(TEXT("Peak Acolyte"), FVector2D(PeakCenter.X - 600.f, PeakCenter.Y - 900.f), FLinearColor(0.75f, 0.65f, 0.95f));

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
	if (FVector2D::Distance(P, Snowveil) < 3200.f) { return EHolypawZone::Snowveil; }
	if (FVector2D::Distance(P, RibbonCity) < 6500.f) { return EHolypawZone::RibbonCity; }
	if (FVector2D::Distance(P, Tidewell) < 4200.f) { return EHolypawZone::Tidewell; }
	if (FVector2D::Distance(P, Hearthfold) < 3800.f) { return EHolypawZone::Hearthfold; }
	if (FVector2D::Distance(P, Emberfen) < 3600.f) { return EHolypawZone::Emberfen; }

	const float Md = FVector2D::Distance(P, PeakCenter);
	if (Md < 7000.f && WorldPos.Z > 900.f)
	{
		return WorldPos.Z > 1800.f ? EHolypawZone::Snow : EHolypawZone::Highlands;
	}
	if (WorldPos.X > 36000.f) { return EHolypawZone::Coast; }
	if (WorldPos.Y < -14000.f) { return EHolypawZone::Mire; }
	if (WorldPos.Y > 10000.f && WorldPos.X > -8000.f && WorldPos.X < 16000.f) { return EHolypawZone::Homestead; }
	if (WorldPos.X < -20000.f) { return EHolypawZone::ForestCottage; }
	return EHolypawZone::NurseryHills;
}

FString AHolypawWorldBuilder::GetCompassLine(const FVector& From) const
{
	const TArray<FHolypawLandmark> Marks = {
		{TEXT("Ribbon City"), RibbonCity},
		{TEXT("Tidewell"), Tidewell},
		{TEXT("Hearthfold"), Hearthfold},
		{TEXT("Emberfen"), Emberfen},
		{TEXT("Snowveil"), Snowveil},
		{TEXT("Velvet Peak"), PeakCenter},
		{TEXT("Cottage"), FVector2D(CottageSpawn.X, CottageSpawn.Y)},
		{TEXT("Poly Mill"), RibbonCity + FVector2D(5200.f, -800.f)}
	};
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
	Lines.Add(TEXT("Living World  (N close)"));
	const TArray<FHolypawLandmark> Marks = {
		{TEXT("Cottage (home)"), FVector2D(CottageSpawn.X, CottageSpawn.Y)},
		{TEXT("Ribbon City"), RibbonCity},
		{TEXT("Tidewell Harbor"), Tidewell},
		{TEXT("Hearthfold"), Hearthfold},
		{TEXT("Emberfen"), Emberfen},
		{TEXT("Snowveil"), Snowveil},
		{TEXT("Velvet Peak shrine"), PeakCenter},
		{TEXT("Poly Mill"), RibbonCity + FVector2D(5200.f, -800.f)}
	};
	const FVector2D P(From.X, From.Y);
	for (const FHolypawLandmark& M : Marks)
	{
		const int32 Meters = FMath::RoundToInt(FVector2D::Distance(P, M.Pos) / 100.f);
		Lines.Add(FString::Printf(TEXT("%s   %dm"), *M.Name, Meters));
	}
	return Lines;
}
