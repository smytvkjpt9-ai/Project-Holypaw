#include "HolypawWorldBuilder.h"
#include "Holypaw.h"
#include "Actors/WildFluffy.h"
#include "Actors/HostilePet.h"
#include "Actors/HugHuman.h"
#include "Actors/CottageRest.h"
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeFinder(TEXT("/Engine/BasicShapes/Cone.Cone"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
	}
	if (SphereFinder.Succeeded())
	{
		SphereMesh = SphereFinder.Object;
	}
	if (ConeFinder.Succeeded())
	{
		ConeMesh = ConeFinder.Object;
	}
	if (CylFinder.Succeeded())
	{
		CylMesh = CylFinder.Object;
	}
	if (MatFinder.Succeeded())
	{
		ShapeMat = MatFinder.Object;
	}
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
	UE_LOG(LogHolypaw, Log, TEXT("Holypaw world builder: generating cottage, path, city, peak."));
	HideTemplateFloor();
	SpawnAtmosphere();
	BuildTerrain();
	ScatterFlora();
	BuildMountain();
	BuildCottage();
	BuildPathAndCity();
	SpawnGameplayActors();
	SpawnPlayerStart();
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
			C->SetDynamicShadowDistanceMovableLight(40000.f);
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
			C->SetFogDensity(0.018f);
			C->SetFogHeightFalloff(0.12f);
			C->SetFogInscatteringColor(FLinearColor(0.78f, 0.84f, 0.95f));
			C->SetVolumetricFog(true);
			C->SetVolumetricFogScatteringDistribution(0.7f);
		}
	}

	GetWorld()->SpawnActor<ASkyAtmosphere>(FVector::ZeroVector, FRotator::ZeroRotator, Sp);
}

float AHolypawWorldBuilder::HashRand(int32 X, int32 Y, int32 Salt) const
{
	uint32 H = static_cast<uint32>(X * 374761393 + Y * 668265263 + Salt * 1274126177);
	H = (H ^ (H >> 13)) * 1274126177u;
	H ^= H >> 16;
	return (H & 0xFFFF) / 65535.f;
}

float AHolypawWorldBuilder::SampleHeight(float X, float Y) const
{
	const float Nx = X / 18000.f;
	const float Ny = Y / 18000.f;
	float H = 90.f;
	H += 140.f * FMath::Sin(Nx * 1.15f) * FMath::Cos(Ny * 0.95f);
	H += 65.f * FMath::Sin(Nx * 2.8f + Ny * 1.7f);
	H += 40.f * FMath::Sin(Nx * 6.1f) * FMath::Sin(Ny * 5.4f);

	const float Md = FVector2D(X - PeakCenter.X, Y - PeakCenter.Y).Size();
	H += 3200.f * FMath::Exp(-FMath::Square(Md / 8500.f));
	if (Md < 3500.f)
	{
		H += 900.f * (1.f - Md / 3500.f);
	}

	const float Cd = FVector2D(X - CityCenter.X, Y - CityCenter.Y).Size();
	if (Cd < 8000.f)
	{
		const float T = 1.f - Cd / 8000.f;
		H = FMath::Lerp(H, 55.f, T * T);
	}

	// Gentle basin for the cottage clearing
	const float Fd = FVector2D(X + 32000.f, Y - 800.f).Size();
	if (Fd < 3500.f)
	{
		H = FMath::Lerp(H, 70.f, 1.f - Fd / 3500.f);
	}

	// Coast drop east of city
	if (X > 34000.f)
	{
		H -= (X - 34000.f) * 0.04f;
	}
	return H;
}

void AHolypawWorldBuilder::BuildTerrain()
{
	TArray<FVector> Verts;
	TArray<int32> Tris;
	TArray<FVector> Normals;
	TArray<FVector2D> UV;
	TArray<FColor> Colors;
	TArray<FProcMeshTangent> Tangents;

	const int32 N = GridN;
	const float Half = (N - 1) * Cell * 0.5f;
	Verts.Reserve(N * N);
	UV.Reserve(N * N);
	Colors.Reserve(N * N);

	for (int32 J = 0; J < N; ++J)
	{
		for (int32 I = 0; I < N; ++I)
		{
			const float X = -Half + I * Cell;
			const float Y = -Half + J * Cell;
			const float Z = SampleHeight(X, Y);
			Verts.Add(FVector(X, Y, Z));
			UV.Add(FVector2D(I / float(N - 1) * 16.f, J / float(N - 1) * 16.f));

			const EHolypawZone Zid = ResolveZone(FVector(X, Y, Z));
			FLinearColor C(0.45f, 0.62f, 0.38f);
			switch (Zid)
			{
			case EHolypawZone::ForestCottage: C = FLinearColor(0.32f, 0.52f, 0.34f); break;
			case EHolypawZone::NurseryHills: C = FLinearColor(0.55f, 0.72f, 0.40f); break;
			case EHolypawZone::RibbonCity: C = FLinearColor(0.62f, 0.58f, 0.52f); break;
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
			const int32 C = A + N;
			const int32 D = C + 1;
			Tris.Add(A); Tris.Add(C); Tris.Add(B);
			Tris.Add(B); Tris.Add(C); Tris.Add(D);
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
	if (!Mesh || !ShapeMat)
	{
		return;
	}
	if (UMaterialInstanceDynamic* Mid = Mesh->CreateDynamicMaterialInstance(0, ShapeMat))
	{
		Mid->SetVectorParameterValue(TEXT("Color"), Color);
	}
}

UStaticMeshComponent* AHolypawWorldBuilder::PlaceCube(const FVector& Loc, const FVector& Scale, const FLinearColor& Color, const FName& Name)
{
	UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(this, Name);
	if (!Comp || !CubeMesh)
	{
		return nullptr;
	}
	Comp->SetStaticMesh(CubeMesh);
	Comp->SetMobility(EComponentMobility::Movable);
	Comp->CreationMethod = EComponentCreationMethod::Instance;
	if (ShapeMat)
	{
		Comp->SetMaterial(0, ShapeMat);
	}
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
	if (CylMesh)
	{
		Trees->SetStaticMesh(CylMesh);
	}
	else if (CubeMesh)
	{
		Trees->SetStaticMesh(CubeMesh);
	}
	if (ConeMesh)
	{
		Canopies->SetStaticMesh(ConeMesh);
	}
	else if (SphereMesh)
	{
		Canopies->SetStaticMesh(SphereMesh);
	}
	if (CubeMesh)
	{
		Grass->SetStaticMesh(CubeMesh);
	}
	if (ShapeMat)
	{
		if (UMaterialInstanceDynamic* Trunk = UMaterialInstanceDynamic::Create(ShapeMat, this))
		{
			Trunk->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.38f, 0.24f, 0.16f));
			Trees->SetMaterial(0, Trunk);
		}
		if (UMaterialInstanceDynamic* Leaf = UMaterialInstanceDynamic::Create(ShapeMat, this))
		{
			Leaf->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.28f, 0.52f, 0.32f));
			Canopies->SetMaterial(0, Leaf);
		}
		if (UMaterialInstanceDynamic* G = UMaterialInstanceDynamic::Create(ShapeMat, this))
		{
			G->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.40f, 0.68f, 0.36f));
			Grass->SetMaterial(0, G);
		}
	}

	const float Half = (GridN - 1) * Cell * 0.5f;
	int32 TreeCount = 0;
	int32 GrassCount = 0;
	for (int32 N = 0; N < 1400 && TreeCount < 520; ++N)
	{
		const float X = FMath::FRandRange(-Half * 0.9f, Half * 0.9f);
		const float Y = FMath::FRandRange(-Half * 0.9f, Half * 0.9f);
		const EHolypawZone Z = ResolveZone(FVector(X, Y, 0.f));
		if (Z == EHolypawZone::RibbonCity || Z == EHolypawZone::Coast)
		{
			continue;
		}
		if (Z != EHolypawZone::ForestCottage && Z != EHolypawZone::NurseryHills && HashRand(int32(X), int32(Y), 3) > 0.35f)
		{
			continue;
		}
		const float Zs = SampleHeight(X, Y);
		const float H = 180.f + HashRand(int32(X), int32(Y), 7) * 160.f;
		FTransform Trunk(FRotator::ZeroRotator, FVector(X, Y, Zs + H * 0.5f), FVector(0.45f, 0.45f, H / 100.f));
		Trees->AddInstance(Trunk, true);
		FTransform Cap(FRotator::ZeroRotator, FVector(X, Y, Zs + H + 80.f), FVector(2.4f, 2.4f, 2.1f));
		Canopies->AddInstance(Cap, true);
		++TreeCount;
	}

	for (int32 N = 0; N < 900 && GrassCount < 700; ++N)
	{
		const float X = FMath::FRandRange(-Half * 0.85f, CityCenter.X - 2000.f);
		const float Y = FMath::FRandRange(-12000.f, 12000.f);
		if (ResolveZone(FVector(X, Y, 0.f)) == EHolypawZone::RibbonCity)
		{
			continue;
		}
		const float Zs = SampleHeight(X, Y);
		FTransform T(FRotator::ZeroRotator, FVector(X, Y, Zs + 18.f), FVector(0.12f, 0.12f, 0.35f + HashRand(int32(X), int32(Y), 11) * 0.25f));
		Grass->AddInstance(T, true);
		++GrassCount;
	}
}

void AHolypawWorldBuilder::BuildCottage()
{
	const float Z = SampleHeight(CottageSpawn.X, CottageSpawn.Y);
	CottageSpawn.Z = Z + 120.f;

	PlaceCube(FVector(CottageSpawn.X, CottageSpawn.Y, Z + 160.f), FVector(6.5f, 5.2f, 3.2f), FLinearColor(0.72f, 0.48f, 0.36f), TEXT("CottageBody"));
	PlaceCube(FVector(CottageSpawn.X, CottageSpawn.Y, Z + 340.f), FVector(7.4f, 6.0f, 1.6f), FLinearColor(0.55f, 0.28f, 0.28f), TEXT("CottageRoof"));
	PlaceCube(FVector(CottageSpawn.X + 280.f, CottageSpawn.Y, Z + 90.f), FVector(3.2f, 3.8f, 0.25f), FLinearColor(0.62f, 0.5f, 0.38f), TEXT("Porch"));
	PlaceCube(FVector(CottageSpawn.X + 40.f, CottageSpawn.Y - 90.f, Z + 175.f), FVector(0.9f, 0.12f, 0.9f), FLinearColor(0.55f, 0.82f, 0.95f), TEXT("Window"));

	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ACottageRest* C = GetWorld()->SpawnActor<ACottageRest>(FVector(CottageSpawn.X + 420.f, CottageSpawn.Y, Z + 40.f), FRotator::ZeroRotator, Sp))
	{
		C->SetActorScale3D(FVector(1.4f, 1.4f, 0.15f));
	}
}

void AHolypawWorldBuilder::BuildPathAndCity()
{
	const float X0 = CottageSpawn.X + 400.f;
	const float X1 = CityCenter.X - 3500.f;
	for (int32 I = 0; I <= 28; ++I)
	{
		const float T = I / 28.f;
		const float X = FMath::Lerp(X0, X1, T);
		const float Y = FMath::Sin(T * PI * 1.2f) * 900.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 8.f), FVector(7.5f, 2.2f, 0.12f), FLinearColor(0.72f, 0.62f, 0.46f), FName(*FString::Printf(TEXT("Path%d"), I)));
		if (I % 3 == 0)
		{
			PlaceCube(FVector(X, Y + 160.f, Z + 90.f), FVector(0.18f, 0.18f, 1.7f), FLinearColor(0.35f, 0.28f, 0.22f), FName(*FString::Printf(TEXT("LampPole%d"), I)));
			PlaceCube(FVector(X, Y + 160.f, Z + 180.f), FVector(0.35f, 0.35f, 0.35f), FLinearColor(1.f, 0.85f, 0.45f), FName(*FString::Printf(TEXT("Lamp%d"), I)));
		}
	}

	const float Zc = SampleHeight(CityCenter.X, CityCenter.Y);
	// Plaza
	PlaceCube(FVector(CityCenter.X, CityCenter.Y, Zc + 12.f), FVector(28.f, 22.f, 0.2f), FLinearColor(0.70f, 0.64f, 0.56f), TEXT("Plaza"));
	// Spire
	PlaceCube(FVector(CityCenter.X - 200.f, CityCenter.Y + 200.f, Zc + 900.f), FVector(2.4f, 2.4f, 18.f), FLinearColor(0.82f, 0.42f, 0.55f), TEXT("Spire"));
	PlaceCube(FVector(CityCenter.X - 200.f, CityCenter.Y + 200.f, Zc + 1850.f), FVector(3.6f, 3.6f, 1.2f), FLinearColor(0.95f, 0.75f, 0.35f), TEXT("SpireCap"));

	const TArray<FLinearColor> WallColors = {
		FLinearColor(0.85f, 0.62f, 0.58f),
		FLinearColor(0.72f, 0.68f, 0.82f),
		FLinearColor(0.62f, 0.75f, 0.70f),
		FLinearColor(0.90f, 0.78f, 0.55f),
		FLinearColor(0.78f, 0.58f, 0.62f)
	};

	int32 B = 0;
	for (int32 Row = -3; Row <= 3; ++Row)
	{
		for (int32 Col = -4; Col <= 4; ++Col)
		{
			if (FMath::Abs(Row) < 2 && FMath::Abs(Col) < 2)
			{
				continue; // keep plaza open
			}
			const float X = CityCenter.X + Col * 980.f + HashRand(Col, Row, 2) * 80.f;
			const float Y = CityCenter.Y + Row * 860.f;
			const float Z = SampleHeight(X, Y);
			const float W = 2.4f + HashRand(Col, Row, 5) * 1.8f;
			const float D = 2.2f + HashRand(Col, Row, 6) * 1.4f;
			const float H = 3.5f + HashRand(Col, Row, 8) * 6.5f;
			const FLinearColor Wall = WallColors[(Col + Row + 8) % WallColors.Num()];
			PlaceCube(FVector(X, Y, Z + H * 50.f), FVector(W, D, H), Wall, FName(*FString::Printf(TEXT("Bldg%d"), B)));
			PlaceCube(FVector(X, Y, Z + H * 100.f + 40.f), FVector(W + 0.35f, D + 0.35f, 0.55f), FLinearColor(0.55f, 0.28f, 0.32f), FName(*FString::Printf(TEXT("Roof%d"), B)));
			++B;
		}
	}

	// Harbor steps toward the coast
	for (int32 I = 0; I < 6; ++I)
	{
		const float X = CityCenter.X + 4200.f + I * 500.f;
		const float Y = CityCenter.Y + 1800.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 10.f + I * 4.f), FVector(4.5f, 8.f, 0.18f), FLinearColor(0.60f, 0.58f, 0.62f), FName(*FString::Printf(TEXT("Harbor%d"), I)));
	}
}

void AHolypawWorldBuilder::BuildMountain()
{
	const float Z = SampleHeight(PeakCenter.X, PeakCenter.Y);
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y, Z + 600.f), FVector(22.f, 18.f, 14.f), FLinearColor(0.58f, 0.55f, 0.62f), TEXT("PeakMass"));
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y, Z + 1600.f), FVector(8.f, 7.f, 8.f), FLinearColor(0.88f, 0.90f, 0.95f), TEXT("PeakSnow"));
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 2100.f), FVector(2.2f, 2.2f, 1.4f), FLinearColor(0.85f, 0.7f, 1.f), TEXT("Shrine"));
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

	// Cottage clearing
	SpawnFluffy(0, FVector2D(CottageSpawn.X + 700.f, CottageSpawn.Y + 400.f));
	SpawnFluffy(2, FVector2D(CottageSpawn.X + 500.f, CottageSpawn.Y - 600.f));
	SpawnFluffy(1, FVector2D(-18000.f, 1200.f));
	SpawnFluffy(3, FVector2D(-8000.f, -4000.f));
	SpawnFluffy(4, FVector2D(8000.f, 6000.f));
	SpawnFluffy(5, FVector2D(PeakCenter.X + 1800.f, PeakCenter.Y - 2200.f));
	SpawnFluffy(0, FVector2D(12000.f, -7000.f));
	SpawnFluffy(2, FVector2D(CityCenter.X - 6000.f, -2500.f));

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
	SpawnHuman(TEXT("Shopkeep"), FVector2D(CityCenter.X + 400.f, CityCenter.Y - 500.f), FLinearColor(0.95f, 0.75f, 0.4f));
	SpawnHuman(TEXT("Acolyte"), FVector2D(CityCenter.X - 300.f, CityCenter.Y + 600.f), FLinearColor(0.72f, 0.62f, 0.9f));
	SpawnHuman(TEXT("Harbor Hand"), FVector2D(CityCenter.X + 3800.f, CityCenter.Y + 1600.f), FLinearColor(0.45f, 0.55f, 0.7f));

	auto SpawnHostile = [&](const TCHAR* Name, int32 Hp, int32 Atk, const FVector2D& XY)
	{
		const float Z = SampleHeight(XY.X, XY.Y) + 40.f;
		if (AHostilePet* H = GetWorld()->SpawnActor<AHostilePet>(FVector(XY.X, XY.Y, Z), FRotator::ZeroRotator, Sp))
		{
			H->DisplayName = FText::FromString(Name);
			H->HPMax = Hp;
			H->HP = Hp;
			H->Attack = Atk;
		}
	};
	SpawnHostile(TEXT("Scrap Dog"), 28, 7, FVector2D(-12000.f, 2200.f));
	SpawnHostile(TEXT("Corp Cat"), 32, 8, FVector2D(-2000.f, -3500.f));
	SpawnHostile(TEXT("Razor Petbot"), 40, 10, FVector2D(9000.f, 2500.f));
	SpawnHostile(TEXT("Void Rat"), 22, 9, FVector2D(15000.f, -5000.f));
	SpawnHostile(TEXT("Alley Scrap Dog"), 26, 7, FVector2D(CityCenter.X + 2400.f, CityCenter.Y - 2100.f));
}

void AHolypawWorldBuilder::SpawnPlayerStart()
{
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FVector Loc = CottageSpawn + FVector(420.f, 0.f, 40.f);
	if (APlayerStart* PS = GetWorld()->SpawnActor<APlayerStart>(Loc, FRotator(0.f, 0.f, 0.f), Sp))
	{
		PS->PlayerStartTag = FName(TEXT("Cottage"));
	}
}

EHolypawZone AHolypawWorldBuilder::ResolveZone(const FVector& WorldPos) const
{
	const float Md = FVector2D(WorldPos.X - PeakCenter.X, WorldPos.Y - PeakCenter.Y).Size();
	if (Md < 7000.f && WorldPos.Z > 900.f)
	{
		return WorldPos.Z > 1800.f ? EHolypawZone::Snow : EHolypawZone::Highlands;
	}
	const float Cd = FVector2D(WorldPos.X - CityCenter.X, WorldPos.Y - CityCenter.Y).Size();
	if (Cd < 6500.f)
	{
		return EHolypawZone::RibbonCity;
	}
	if (WorldPos.X > 33000.f)
	{
		return EHolypawZone::Coast;
	}
	if (WorldPos.Y < -14000.f)
	{
		return EHolypawZone::Mire;
	}
	if (WorldPos.Y > 8000.f && WorldPos.X > -5000.f && WorldPos.X < 18000.f)
	{
		return EHolypawZone::Homestead;
	}
	if (WorldPos.X < -20000.f)
	{
		return EHolypawZone::ForestCottage;
	}
	return EHolypawZone::NurseryHills;
}
