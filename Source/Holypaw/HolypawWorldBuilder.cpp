#include "HolypawWorldBuilder.h"
#include "Holypaw.h"
#include "HolypawGameInstance.h"
#include "Faith/HolypawFaithSim.h"
#include "Actors/WildFluffy.h"
#include "Actors/HostilePet.h"
#include "Actors/HugHuman.h"
#include "Actors/CottageRest.h"
#include "Actors/CampRest.h"
#include "Actors/FaithStall.h"
#include "Actors/Signpost.h"
#include "Actors/TravelLantern.h"
#include "Actors/HolypawPickup.h"
#include "Character/HolypawCharacter.h"
#include "Look/HolypawLook.h"
#include "ProceduralMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/VolumetricCloudComponent.h"
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

	CanopyPuffs = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CanopyPuffs"));
	CanopyPuffs->SetupAttachment(RootComponent);
	CanopyPuffs->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Flowers = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Flowers"));
	Flowers->SetupAttachment(RootComponent);
	Flowers->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Windows = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Windows"));
	Windows->SetupAttachment(RootComponent);
	Windows->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Chimneys = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Chimneys"));
	Chimneys->SetupAttachment(RootComponent);
	Chimneys->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Chimneys->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	Doors = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Doors"));
	Doors->SetupAttachment(RootComponent);
	Doors->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WindowWarm = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WindowWarm"));
	WindowWarm->SetupAttachment(RootComponent);
	WindowWarm->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Foam = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Foam"));
	Foam->SetupAttachment(RootComponent);
	Foam->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	TickWorldStream();
	TickConversionPulse(DeltaSeconds);
}

void AHolypawWorldBuilder::TickClockLighting(float DeltaSeconds)
{
	UHolypawGameInstance* GI = UHolypawGameInstance::Get(this);
	if (!GI)
	{
		return;
	}
	const float Hour = GI->GetWorldHour();
	const HolypawLook::FHourLook Look = HolypawLook::SampleHour(Hour);
	float SunInt = Look.SunInt;
	FLinearColor SunCol = Look.SunCol;
	float SkyInt = Look.SkyInt;
	FLinearColor FogCol = Look.FogCol;
	float FogDensity = Look.FogDensity;
	const float Pitch = Look.Pitch;
	float MoonInt = Look.MoonInt;
	bool bIndoors = false;
	float MillW = 0.f;

	if (APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		if (IsPlayerIndoors(Pawn->GetActorLocation()))
		{
			bIndoors = true;
			SunInt *= 0.42f;
			SkyInt *= 0.55f;
			FogDensity += 0.004f;
			SunCol = FMath::Lerp(SunCol, FLinearColor(1.f, 0.82f, 0.58f), 0.45f);
			FogCol = FMath::Lerp(FogCol, FLinearColor(0.72f, 0.52f, 0.4f), 0.35f);
		}
		const FVector2D Mill = RibbonCity + FVector2D(5200.f, -800.f);
		const FVector2D P(Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y);
		const float Dist = FVector2D::Distance(P, Mill);
		if (Dist < 6200.f)
		{
			const float W = 1.f - Dist / 6200.f;
			int32 RibbonHearts = 0;
			if (const AHolypawCharacter* Teddy = Cast<AHolypawCharacter>(Pawn))
			{
				RibbonHearts = Teddy->GetCityHearts(EHolypawZone::RibbonCity);
			}
			const float Smog = HolypawFaith::MillSmogScale(RibbonHearts);
			MillW = W * Smog;
			FogDensity += 0.034f * MillW;
			FogCol = FMath::Lerp(FogCol, FLinearColor(0.58f, 0.54f, 0.48f), MillW);
			SunCol = FMath::Lerp(SunCol, FLinearColor(0.78f, 0.72f, 0.62f), MillW * 0.45f);
			if (Hour >= 17.f && Hour < 20.f && HolypawFaith::DuskHymnUnlocked(RibbonHearts))
			{
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.95f, 0.72f, 0.48f), 0.35f);
				SunCol = FMath::Lerp(SunCol, FLinearColor(1.f, 0.62f, 0.42f), 0.22f);
			}
		}
		if (const AHolypawCharacter* Teddy = Cast<AHolypawCharacter>(Pawn))
		{
			switch (Teddy->CurrentZone)
			{
			case EHolypawZone::Tidewell:
			case EHolypawZone::Coast:
			case EHolypawZone::Ocean:
			case EHolypawZone::CapePlush:
			case EHolypawZone::CoralChoir:
				FogDensity += 0.004f;
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.55f, 0.72f, 0.82f), 0.35f);
				break;
			case EHolypawZone::Snowveil:
			case EHolypawZone::Snow:
			case EHolypawZone::Ice:
			case EHolypawZone::FeltIceCamp:
			case EHolypawZone::AuroraBorough:
			case EHolypawZone::TundraParish:
				FogDensity += 0.003f;
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.86f, 0.9f, 0.98f), 0.4f);
				SunCol = FMath::Lerp(SunCol, FLinearColor(0.82f, 0.88f, 1.f), 0.25f);
				break;
			case EHolypawZone::Emberfen:
			case EHolypawZone::Mire:
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.62f, 0.32f, 0.28f), 0.4f);
				break;
			case EHolypawZone::LanternAngeles:
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.92f, 0.74f, 0.42f), 0.32f);
				SunCol = FMath::Lerp(SunCol, FLinearColor(1.f, 0.82f, 0.45f), 0.2f);
				break;
			case EHolypawZone::Mossgate:
			case EHolypawZone::Quiltland:
				FogDensity += 0.006f;
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.48f, 0.68f, 0.62f), 0.4f);
				break;
			case EHolypawZone::PalmaDusk:
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.35f, 0.72f, 0.58f), 0.3f);
				SunCol = FMath::Lerp(SunCol, FLinearColor(1.f, 0.72f, 0.42f), 0.18f);
				break;
			case EHolypawZone::CherryLoom:
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.92f, 0.55f, 0.68f), 0.32f);
				break;
			case EHolypawZone::DustMesa:
			case EHolypawZone::Desert:
			case EHolypawZone::SandHymn:
			case EHolypawZone::IvorySpire:
			case EHolypawZone::SavannahBell:
				FogDensity = FMath::Max(0.01f, FogDensity - 0.006f);
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.92f, 0.78f, 0.5f), 0.35f);
				break;
			case EHolypawZone::Clockhaven:
			case EHolypawZone::VelvetSeine:
				FogDensity += 0.005f;
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.62f, 0.68f, 0.78f), 0.38f);
				break;
			case EHolypawZone::MarbleForum:
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.88f, 0.82f, 0.7f), 0.28f);
				break;
			case EHolypawZone::CarnivalBahia:
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.95f, 0.48f, 0.4f), 0.3f);
				SunCol = FMath::Lerp(SunCol, FLinearColor(1.f, 0.7f, 0.45f), 0.18f);
				break;
			case EHolypawZone::AndesLoom:
				FogDensity += 0.006f;
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.62f, 0.5f, 0.4f), 0.28f);
				break;
			case EHolypawZone::SilkDelta:
			case EHolypawZone::SpiceHarbor:
				FogCol = FMath::Lerp(FogCol, FLinearColor(0.86f, 0.45f, 0.32f), 0.28f);
				break;
			default:
				break;
			}
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
	if (FillLight)
	{
		FillLight->SetActorRotation(FRotator(-28.f, 215.f, 0.f));
		if (UDirectionalLightComponent* C = FillLight->FindComponentByClass<UDirectionalLightComponent>())
		{
			C->SetIntensity(FMath::FInterpTo(C->Intensity, bIndoors ? 0.28f : 0.40f, DeltaSeconds, 1.2f));
		}
	}
	if (MoonLight)
	{
		MoonLight->SetActorRotation(FRotator(-48.f, 200.f, 0.f));
		if (UDirectionalLightComponent* C = MoonLight->FindComponentByClass<UDirectionalLightComponent>())
		{
			C->SetIntensity(FMath::FInterpTo(C->Intensity, MoonInt, DeltaSeconds, 1.2f));
			C->SetAtmosphereSunLight(MoonInt > 0.35f);
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
	HolypawLook::TickGrade(GradeVolume, Hour, bIndoors, MillW);
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
	RequestDress(EHolypawZone::RibbonCity);
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
		HolypawLook::DressSun(Sun->FindComponentByClass<UDirectionalLightComponent>());
	}
	if (ADirectionalLight* Fill = GetWorld()->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-28.f, 215.f, 0.f), Sp))
	{
		FillLight = Fill;
		HolypawLook::DressFill(Fill->FindComponentByClass<UDirectionalLightComponent>());
	}
	if (ADirectionalLight* Moon = GetWorld()->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-48.f, 200.f, 0.f), Sp))
	{
		MoonLight = Moon;
		HolypawLook::DressMoon(Moon->FindComponentByClass<UDirectionalLightComponent>());
	}
	if (ASkyLight* Sky = GetWorld()->SpawnActor<ASkyLight>(FVector(0.f, 0.f, 400.f), FRotator::ZeroRotator, Sp))
	{
		SkyLight = Sky;
		HolypawLook::DressSky(Sky->FindComponentByClass<USkyLightComponent>());
	}
	if (AExponentialHeightFog* Fog = GetWorld()->SpawnActor<AExponentialHeightFog>(FVector(0.f, 0.f, 200.f), FRotator::ZeroRotator, Sp))
	{
		HeightFog = Fog;
		HolypawLook::DressFog(Fog->FindComponentByClass<UExponentialHeightFogComponent>());
	}
	if (ASkyAtmosphere* Atmo = GetWorld()->SpawnActor<ASkyAtmosphere>(FVector::ZeroVector, FRotator::ZeroRotator, Sp))
	{
		SkyAtmo = Atmo;
		HolypawLook::DressAtmosphere(Atmo->FindComponentByClass<USkyAtmosphereComponent>());
	}
	if (AVolumetricCloud* Cloud = GetWorld()->SpawnActor<AVolumetricCloud>(FVector::ZeroVector, FRotator::ZeroRotator, Sp))
	{
		Clouds = Cloud;
		HolypawLook::DressClouds(Cloud->FindComponentByClass<UVolumetricCloudComponent>());
	}
	if (APostProcessVolume* PP = GetWorld()->SpawnActor<APostProcessVolume>(FVector::ZeroVector, FRotator::ZeroRotator, Sp))
	{
		GradeVolume = PP;
		HolypawLook::GradeVolume(PP);
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
	LanternAngeles = CityXY(EHolypawZone::LanternAngeles);
	Mossgate = CityXY(EHolypawZone::Mossgate);
	PalmaDusk = CityXY(EHolypawZone::PalmaDusk);
	CherryLoom = CityXY(EHolypawZone::CherryLoom);
	Quiltland = CityXY(EHolypawZone::Quiltland);
	DustMesa = CityXY(EHolypawZone::DustMesa);
	Clockhaven = CityXY(EHolypawZone::Clockhaven);
	VelvetSeine = CityXY(EHolypawZone::VelvetSeine);
	MarbleForum = CityXY(EHolypawZone::MarbleForum);
	IvorySpire = CityXY(EHolypawZone::IvorySpire);
	SandHymn = CityXY(EHolypawZone::SandHymn);
	CapePlush = CityXY(EHolypawZone::CapePlush);
	SavannahBell = CityXY(EHolypawZone::SavannahBell);
	CarnivalBahia = CityXY(EHolypawZone::CarnivalBahia);
	AndesLoom = CityXY(EHolypawZone::AndesLoom);
	SilkDelta = CityXY(EHolypawZone::SilkDelta);
	SpiceHarbor = CityXY(EHolypawZone::SpiceHarbor);
	CoralChoir = CityXY(EHolypawZone::CoralChoir);
	AuroraBorough = CityXY(EHolypawZone::AuroraBorough);
	TundraParish = CityXY(EHolypawZone::TundraParish);
	FeltIceCamp = CityXY(EHolypawZone::FeltIceCamp);
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
	HolypawLook::Paint(Mesh, Color);
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
	Bind(Trees, CylMesh ? CylMesh : CubeMesh, HolypawLook::Wood);
	Bind(Canopies, ConeMesh ? ConeMesh : SphereMesh, HolypawLook::MintDeep);
	Bind(CanopyPuffs, SphereMesh ? SphereMesh : CubeMesh, HolypawLook::Mint);
	Bind(Grass, CubeMesh, HolypawLook::GrassTuft);
	Bind(Rocks, SphereMesh ? SphereMesh : CubeMesh, FLinearColor(0.58f, 0.54f, 0.50f));
	Bind(Reeds, CubeMesh, FLinearColor(0.35f, 0.42f, 0.28f));
	Bind(WallRose, CubeMesh, HolypawLook::Rose);
	Bind(WallMint, CubeMesh, HolypawLook::Mint);
	Bind(WallGold, CubeMesh, HolypawLook::Gold);
	Bind(Roofs, CubeMesh, HolypawLook::Roof);
	Bind(RoadTiles, CubeMesh, FLinearColor(0.72f, 0.62f, 0.46f));
	Bind(WaterTiles, CubeMesh, HolypawLook::Water);
	Bind(Cacti, ConeMesh ? ConeMesh : CubeMesh, FLinearColor(0.42f, 0.62f, 0.32f));
	Bind(Flowers, CubeMesh, HolypawLook::BloomPink);
	Bind(Windows, CubeMesh, HolypawLook::Glass);
	Bind(WindowWarm, CubeMesh, HolypawLook::GoldWarm);
	Bind(Doors, CubeMesh, HolypawLook::Wood);
	Bind(Chimneys, CubeMesh, FLinearColor(0.48f, 0.28f, 0.24f));
	Bind(Foam, CubeMesh, HolypawLook::Foam);
}

void AHolypawWorldBuilder::AddKit(UInstancedStaticMeshComponent* ISM, const FVector& Loc, const FRotator& Rot, const FVector& Scale)
{
	if (!ISM) { return; }
	ISM->AddInstance(FTransform(Rot, Loc, Scale), true);
}

UStaticMeshComponent* AHolypawWorldBuilder::PlaceCube(const FVector& Loc, const FVector& Scale, const FLinearColor& Color, const FName& Name, const FRotator& Rot, const bool bBlock)
{
	return PlacePrimitive(CubeMesh, Loc, Scale, Color, Name, Rot, bBlock);
}

UStaticMeshComponent* AHolypawWorldBuilder::PlacePrimitive(UStaticMesh* Mesh, const FVector& Loc, const FVector& Scale, const FLinearColor& Color, const FName& Name, const FRotator& Rot, const bool bBlock)
{
	UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(this, Name);
	if (!Comp || !Mesh) { return nullptr; }
	Comp->SetStaticMesh(Mesh);
	Comp->SetMobility(EComponentMobility::Movable);
	Comp->CreationMethod = EComponentCreationMethod::Instance;
	if (ShapeMat) { Comp->SetMaterial(0, ShapeMat); }
	Comp->SetWorldLocation(Loc);
	Comp->SetWorldRotation(Rot);
	Comp->SetWorldScale3D(Scale);
	Comp->SetCollisionEnabled(bBlock ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	if (bBlock)
	{
		Comp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
	Comp->SetCastShadow(true);
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
		AddKit(Trees, FVector(X, Y, Zs + 22.f), FRotator::ZeroRotator, FVector(0.82f, 0.82f, 0.32f));
		const bool bSnowTree = (Z == EHolypawZone::Snow || Z == EHolypawZone::Snowveil || Z == EHolypawZone::TundraParish || Z == EHolypawZone::AuroraBorough || Z == EHolypawZone::FeltIceCamp);
		const bool bJungleTree = (Z == EHolypawZone::Jungle || Z == EHolypawZone::PalmaDusk || Z == EHolypawZone::Quiltland || Z == EHolypawZone::CarnivalBahia);
		if (bSnowTree)
		{
			AddKit(Canopies, FVector(X, Y, Zs + H + 70.f), FRotator::ZeroRotator, FVector(1.55f, 1.55f, 2.2f));
			AddKit(CanopyPuffs, FVector(X, Y, Zs + H + 20.f), FRotator::ZeroRotator, FVector(1.15f, 1.15f, 1.05f));
		}
		else
		{
			const float Canopy = bJungleTree ? 2.85f : 2.4f;
			AddKit(Canopies, FVector(X, Y, Zs + H + 80.f), FRotator::ZeroRotator, FVector(Canopy, Canopy, bJungleTree ? 2.4f : 2.1f));
			AddKit(CanopyPuffs, FVector(X + 55.f, Y - 30.f, Zs + H + 40.f), FRotator::ZeroRotator, FVector(1.7f, 1.55f, 1.35f));
			if (HashRand(int32(X), int32(Y), 9) > 0.45f || bJungleTree)
			{
				AddKit(CanopyPuffs, FVector(X - 40.f, Y + 35.f, Zs + H + 20.f), FRotator::ZeroRotator, FVector(1.4f, 1.3f, 1.15f));
			}
		}
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
		if (HashRand(int32(X), int32(Y), 17) > 0.72f)
		{
			AddKit(Flowers, FVector(X + 18.f, Y - 10.f, Zs + 14.f), FRotator::ZeroRotator, FVector(0.10f, 0.10f, 0.16f + HashRand(int32(X), int32(Y), 23) * 0.12f));
		}
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
	DressCottageRooms(CottageSpawn, Z); // CottageDoor + knob on the east wall gap
	PlaceCube(FVector(CottageSpawn.X, CottageSpawn.Y - 130.f, Z + 390.f), FVector(7.6f, 3.4f, 0.22f), HolypawLook::Roof, MakeName(TEXT("CottageRoof")), FRotator(22.f, 0.f, 0.f));
	PlaceCube(FVector(CottageSpawn.X, CottageSpawn.Y + 130.f, Z + 390.f), FVector(7.6f, 3.4f, 0.22f), HolypawLook::Roof, MakeName(TEXT("CottageRoof")), FRotator(-22.f, 0.f, 0.f));
	PlaceCube(FVector(CottageSpawn.X + 280.f, CottageSpawn.Y, Z + 90.f), FVector(3.2f, 3.8f, 0.25f), FLinearColor(0.62f, 0.5f, 0.38f), MakeName(TEXT("Porch")));
	PlaceCube(FVector(CottageSpawn.X + 40.f, CottageSpawn.Y - 90.f, Z + 175.f), FVector(0.9f, 0.12f, 0.9f), FLinearColor(0.55f, 0.82f, 0.95f), MakeName(TEXT("Window")));
	PlaceCube(FVector(CottageSpawn.X + 40.f, CottageSpawn.Y + 90.f, Z + 175.f), FVector(0.9f, 0.12f, 0.9f), HolypawLook::Glass, MakeName(TEXT("WindowBack")));
	PlaceCube(FVector(CottageSpawn.X + 40.f, CottageSpawn.Y - 96.f, Z + 175.f), FVector(0.55f, 0.05f, 0.55f), HolypawLook::GoldWarm, MakeName(TEXT("WindowPane")));
	PlaceCube(FVector(CottageSpawn.X - 80.f, CottageSpawn.Y, Z + 28.f), FVector(5.4f, 4.2f, 0.12f), FLinearColor(0.62f, 0.44f, 0.32f), MakeName(TEXT("CottageFloor")));
	PlaceBed(FVector(CottageSpawn.X - 140.f, CottageSpawn.Y + 40.f, Z + 8.f), 0.f, HolypawLook::Rose, TEXT("CottageBed"));
	PlaceTable(FVector(CottageSpawn.X + 40.f, CottageSpawn.Y + 80.f, Z + 8.f), 0.f);
	PlaceChair(FVector(CottageSpawn.X + 40.f, CottageSpawn.Y + 20.f, Z + 8.f), 180.f);
	PlaceChair(FVector(CottageSpawn.X - 20.f, CottageSpawn.Y + 80.f, Z + 8.f), 90.f);
	PlaceCube(FVector(CottageSpawn.X + 1400.f, CottageSpawn.Y - 200.f, Z + 40.f), FVector(4.2f, 0.7f, 0.55f), FLinearColor(0.42f, 0.32f, 0.22f), MakeName(TEXT("FallenTree")));
	PlaceCube(FVector(CottageSpawn.X - 40.f, CottageSpawn.Y, Z + 280.f), FVector(4.2f, 3.4f, 0.16f), FLinearColor(0.68f, 0.5f, 0.4f), MakeName(TEXT("CottageLoft")));
	PlaceCrate(FVector(CottageSpawn.X - 90.f, CottageSpawn.Y - 40.f, Z + 280.f));
	PlaceCube(FVector(CottageSpawn.X + 20.f, CottageSpawn.Y + 60.f, Z - 20.f), FVector(3.2f, 2.6f, 0.9f), FLinearColor(0.32f, 0.24f, 0.2f), MakeName(TEXT("CottageCellar")));
	PlaceChair(FVector(CottageSpawn.X + 250.f, CottageSpawn.Y + 70.f, Z + 90.f), -20.f);
	PlaceChair(FVector(CottageSpawn.X + 250.f, CottageSpawn.Y - 70.f, Z + 90.f), 20.f);
	PlaceCrate(FVector(CottageSpawn.X + 300.f, CottageSpawn.Y, Z + 90.f));
	PlaceCube(FVector(CottageSpawn.X - 200.f, CottageSpawn.Y - 40.f, Z + 420.f), FVector(0.55f, 0.55f, 1.6f), FLinearColor(0.48f, 0.28f, 0.22f), MakeName(TEXT("CottageChimney")));
	PlaceCube(FVector(CottageSpawn.X + 280.f, CottageSpawn.Y - 140.f, Z + 40.f), FVector(0.22f, 0.22f, 0.9f), HolypawLook::Wood, MakeName(TEXT("PorchPostA")));
	PlaceCube(FVector(CottageSpawn.X + 280.f, CottageSpawn.Y + 140.f, Z + 40.f), FVector(0.22f, 0.22f, 0.9f), HolypawLook::Wood, MakeName(TEXT("PorchPostB")));
	PlacePlanter(FVector(CottageSpawn.X + 80.f, CottageSpawn.Y - 200.f, Z));
	PlacePlanter(FVector(CottageSpawn.X + 160.f, CottageSpawn.Y - 200.f, Z));
	UStaticMesh* LogMesh = CylMesh ? CylMesh : CubeMesh;
	PlacePrimitive(LogMesh, FVector(CottageSpawn.X - 180.f, CottageSpawn.Y + 220.f, Z + 22.f), FVector(0.22f, 0.22f, 0.85f), HolypawLook::Wood, MakeName(TEXT("Woodpile")), FRotator(0.f, 20.f, 88.f));
	PlacePrimitive(LogMesh, FVector(CottageSpawn.X - 180.f, CottageSpawn.Y + 238.f, Z + 22.f), FVector(0.22f, 0.22f, 0.85f), HolypawLook::Wood, MakeName(TEXT("Woodpile")), FRotator(0.f, -10.f, 88.f));
	PlacePrimitive(LogMesh, FVector(CottageSpawn.X - 180.f, CottageSpawn.Y + 228.f, Z + 42.f), FVector(0.20f, 0.20f, 0.75f), HolypawLook::Wood, MakeName(TEXT("Woodpile")), FRotator(0.f, 8.f, 88.f));
	PlaceYardFence(FVector(CottageSpawn.X, CottageSpawn.Y, Z));
	PlaceClothesline(FVector(CottageSpawn.X - 160.f, CottageSpawn.Y + 140.f, Z));
	PlaceWell(FVector(CottageSpawn.X + 420.f, CottageSpawn.Y + 220.f, Z));
	HolypawLook::SpawnGlow(GetWorld(), this, FVector(CottageSpawn.X + 280.f, CottageSpawn.Y, Z + 160.f), HolypawLook::Lantern, 1800.f, 640.f);
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
	DressCityPlaza(Center, Zc);

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
			AddKit(Roofs, FVector(X, Y, Z + H * 100.f + 52.f), FRotator(14.f, 0.f, 0.f), FVector(W + 0.72f, D + 0.28f, 0.38f));
			const float FaceY = D * 50.f;
			const float FaceX = W * 50.f;
			AddKit(Windows, FVector(X - 22.f, Y - FaceY, Z + H * 46.f), FRotator::ZeroRotator, FVector(0.36f, 0.07f, 0.42f));
			AddKit(Windows, FVector(X + 22.f, Y - FaceY, Z + H * 46.f), FRotator::ZeroRotator, FVector(0.36f, 0.07f, 0.42f));
			AddKit(Windows, FVector(X - 22.f, Y + FaceY, Z + H * 46.f), FRotator::ZeroRotator, FVector(0.36f, 0.07f, 0.42f));
			AddKit(Windows, FVector(X + 22.f, Y + FaceY, Z + H * 46.f), FRotator::ZeroRotator, FVector(0.36f, 0.07f, 0.42f));
			AddKit(Windows, FVector(X + FaceX, Y, Z + H * 58.f), FRotator(0.f, 90.f, 0.f), FVector(0.36f, 0.07f, 0.40f));
			AddKit(Windows, FVector(X - FaceX, Y, Z + H * 58.f), FRotator(0.f, 90.f, 0.f), FVector(0.36f, 0.07f, 0.40f));
			AddKit(WindowWarm, FVector(X - 22.f, Y - FaceY - 4.f, Z + H * 46.f), FRotator::ZeroRotator, FVector(0.22f, 0.04f, 0.28f));
			AddKit(WindowWarm, FVector(X + 22.f, Y - FaceY - 4.f, Z + H * 46.f), FRotator::ZeroRotator, FVector(0.22f, 0.04f, 0.28f));
			if (H > 5.2f)
			{
				AddKit(Windows, FVector(X - 18.f, Y - FaceY, Z + H * 78.f), FRotator::ZeroRotator, FVector(0.30f, 0.06f, 0.34f));
				AddKit(Windows, FVector(X + 18.f, Y - FaceY, Z + H * 78.f), FRotator::ZeroRotator, FVector(0.30f, 0.06f, 0.34f));
			}
			AddKit(Doors, FVector(X, Y - FaceY, Z + 42.f), FRotator::ZeroRotator, FVector(0.28f, 0.08f, 0.72f));
			AddKit(Chimneys, FVector(X + W * 28.f, Y + D * 16.f, Z + H * 100.f + 88.f), FRotator::ZeroRotator, FVector(0.32f, 0.32f, 0.72f));
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

	PlaceSign(Plaza + FVector2D(80.f, -80.f), NSLOCTEXT("Holypaw", "DistPlaza", "Ribbon Plaza  |  mill ads still up. hug someone — Hearts change this"), TEXT("ribbonPlaza"));
	PlaceSign(Market + FVector2D(-200.f, 0.f), NSLOCTEXT("Holypaw", "DistMarket", "Market  |  shutters down until a Heart sticks"), TEXT("ribbonMarket"));
	PlaceSign(Cloth + FVector2D(0.f, 0.f), NSLOCTEXT("Holypaw", "DistCloth", "Cloth Quarter  |  handmade banners, not polyester"));
	PlaceSign(Harbor + FVector2D(0.f, 0.f), NSLOCTEXT("Holypaw", "DistHarbor", "Harbor Steps  |  east to Tidewell and the Plush Sea"));
	PlaceSign(Quiet + FVector2D(0.f, 0.f), NSLOCTEXT("Holypaw", "DistQuiet", "Quiet Rows  |  windows lit, few hostiles on the street"));

	PlaceStall(Plaza + FVector2D(320.f, -280.f), true, TEXT("Fountain Shopkeep"));
	PlaceStall(Market, false, TEXT("Market Shopkeep"));
	PlaceStall(Market + FVector2D(260.f, 120.f), false, TEXT("Bun Shopkeep"));
	PlaceStall(Market + FVector2D(-180.f, 200.f), false, TEXT("Ribbon Shopkeep"));

	PlaceShrine(Plaza + FVector2D(-180.f, -220.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "Fountain", "Ribbon Fountain"));
	PlaceShrine(Plaza + FVector2D(520.f, 180.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "InnName", "Spire Inn"));
	PlaceShrine(Plaza + FVector2D(-480.f, 420.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "ChapelName", "Bear Chapel"));
	PlaceShrine(Cloth + FVector2D(180.f, 160.f), EHolypawShrineKind::Workshop, NSLOCTEXT("Holypaw", "LoftName", "Cloth Loft"));
	PlaceShrine(Harbor + FVector2D(-220.f, 80.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "CrateName", "Harbor Crate"));
	FountainPool = PlaceCube(FVector(Plaza.X + 40.f, Plaza.Y - 40.f, SampleHeight(Plaza.X, Plaza.Y) + 70.f), FVector(1.6f, 1.6f, 0.22f), FLinearColor(0.55f, 0.75f, 0.92f), MakeName(TEXT("FountainPool")));
	FountainJet = PlaceCube(FVector(Plaza.X + 40.f, Plaza.Y - 40.f, SampleHeight(Plaza.X, Plaza.Y) + 130.f), FVector(0.35f, 0.35f, 1.1f), FLinearColor(0.85f, 0.88f, 0.95f), MakeName(TEXT("FountainJet")));

	const FVector2D Inn = Plaza + FVector2D(520.f, 180.f);
	PlaceCube(FVector(Inn.X + 90.f, Inn.Y, SampleHeight(Inn.X, Inn.Y) + 90.f), FVector(0.35f, 0.12f, 0.55f), FLinearColor(1.f, 0.82f, 0.55f), MakeName(TEXT("InnLamp")));
	PlaceCube(FVector(Inn.X - 70.f, Inn.Y + 40.f, SampleHeight(Inn.X, Inn.Y) + 70.f), FVector(0.8f, 0.35f, 0.18f), FLinearColor(0.62f, 0.38f, 0.28f), MakeName(TEXT("InnBed")));

	const FVector2D Chapel = Plaza + FVector2D(-480.f, 420.f);
	PlaceCube(FVector(Chapel.X + 70.f, Chapel.Y - 40.f, SampleHeight(Chapel.X, Chapel.Y) + 28.f), FVector(0.9f, 0.28f, 0.22f), FLinearColor(0.82f, 0.72f, 0.48f), MakeName(TEXT("PewA")));
	PlaceCube(FVector(Chapel.X + 70.f, Chapel.Y + 40.f, SampleHeight(Chapel.X, Chapel.Y) + 28.f), FVector(0.9f, 0.28f, 0.22f), FLinearColor(0.82f, 0.72f, 0.48f), MakeName(TEXT("PewB")));

	const FVector2D Loft = Cloth + FVector2D(180.f, 160.f);
	PlaceCube(FVector(Loft.X + 80.f, Loft.Y, SampleHeight(Loft.X, Loft.Y) + 50.f), FVector(0.9f, 0.45f, 0.35f), FLinearColor(0.55f, 0.42f, 0.62f), MakeName(TEXT("Loom")));
	PlaceSign(RibbonCity + FVector2D(4700.f, -620.f), NSLOCTEXT("Holypaw", "MillProtest", "POLY ads  |  beige theology, identical smiles. three Hearts drop them"), TEXT("millProtest"));
	PlaceRibbonMillBanners();

	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Cloth.X + I * 220.f;
		const float Y = Cloth.Y - 80.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 140.f), FVector(0.12f, 0.12f, 2.4f), FLinearColor(0.35f, 0.22f, 0.18f), MakeName(TEXT("BannerPole")));
		if (UStaticMeshComponent* ClothBanner = PlaceCube(FVector(X + 40.f, Y, Z + 200.f), FVector(0.9f, 0.08f, 1.1f), FLinearColor(0.85f, 0.42f, 0.58f), MakeName(TEXT("Banner"))))
		{
			HandmadeBanners.Add(ClothBanner);
		}
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

void AHolypawWorldBuilder::BuildTidewellDistricts()
{
	const FVector2D Dock = Tidewell + FVector2D(400.f, 200.f);
	const FVector2D Nets = Tidewell + FVector2D(-900.f, -400.f);
	const FVector2D Stair = Tidewell + FVector2D(1400.f, 800.f);
	const FVector2D Ferry = Tidewell + FVector2D(2200.f, 200.f);
	const FVector2D Choir = Tidewell + FVector2D(-400.f, 1100.f);

	PlaceSign(Dock + FVector2D(80.f, -80.f), NSLOCTEXT("Holypaw", "TideDock", "Outer Dock  |  salt, hooks, brine hymns"));
	PlaceSign(Nets, NSLOCTEXT("Holypaw", "TideNets", "Net Walk  |  pearls under the mesh, not polyester"));
	PlaceSign(Stair, NSLOCTEXT("Holypaw", "TideStair", "Brine Stair  |  steps down to the Plush Sea"));
	PlaceSign(Ferry, NSLOCTEXT("Holypaw", "TideFerry", "Ferry Slip  |  east rumors toward Cherry Loom"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "TideChoir", "Salt Choir  |  off-key on purpose"));

	PlaceShrine(Dock + FVector2D(260.f, 80.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "TideInn", "Tide Inn"));
	PlaceShrine(Choir + FVector2D(80.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "BrineFont", "Brine Font"));
	PlaceShrine(Ferry + FVector2D(-120.f, 60.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "FerryCrate", "Ferry Crate"));
	PlaceShrine(Nets + FVector2D(120.f, 40.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "NetChapel", "Net Chapel"));

	PlacePickup(Nets + FVector2D(-80.f, 90.f), TEXT("saltPearl"), NSLOCTEXT("Holypaw", "PearlPick", "salt pearl"));

	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Nets.X + I * 160.f;
		const float Y = Nets.Y + 40.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 90.f), FVector(0.12f, 1.8f, 0.08f), FLinearColor(0.42f, 0.55f, 0.58f), MakeName(TEXT("NetRack")));
	}
	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Stair.X + I * 90.f;
		const float Y = Stair.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 6.f + I * 10.f), FVector(1.6f, 4.2f, 0.12f), FLinearColor(0.5f, 0.58f, 0.62f), MakeName(TEXT("BrineStep")));
	}
	PlaceCube(FVector(Choir.X, Choir.Y, SampleHeight(Choir.X, Choir.Y) + 40.f), FVector(1.4f, 1.4f, 0.2f), FLinearColor(0.45f, 0.7f, 0.78f), MakeName(TEXT("BrinePool")));
}

void AHolypawWorldBuilder::BuildHearthfoldDistricts()
{
	const FVector2D Square = Hearthfold + FVector2D(80.f, -40.f);
	const FVector2D Barn = Hearthfold + FVector2D(-700.f, 400.f);
	const FVector2D Crows = Hearthfold + FVector2D(900.f, 200.f);
	const FVector2D Path = Hearthfold + FVector2D(200.f, 900.f);
	const FVector2D Choir = Hearthfold + FVector2D(-200.f, -800.f);

	PlaceSign(Square, NSLOCTEXT("Holypaw", "HearthSq", "Grain Square  |  lumpy bread, no identical flour"));
	PlaceSign(Barn, NSLOCTEXT("Holypaw", "HearthBarn", "Hymn Barn  |  wheat that remembers fingers"));
	PlaceSign(Crows, NSLOCTEXT("Holypaw", "HearthCrows", "Scarecrow Row  |  they forgot their job on purpose"));
	PlaceSign(Path, NSLOCTEXT("Holypaw", "HearthPath", "Mill Path  |  Harvest Overseer north, bring a party"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "HearthChoir", "Wheat Choir  |  grain that hums at noon"));

	PlaceShrine(Square + FVector2D(220.f, 80.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "HearthInn", "Loaf Inn"));
	PlaceShrine(Barn + FVector2D(80.f, 40.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "HearthChap", "Barn Chapel"));
	PlaceShrine(Crows + FVector2D(-60.f, 40.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "HearthCrate", "Grain Crate"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "WheatFont", "Wheat Font"));

	PlacePickup(Choir + FVector2D(-90.f, 50.f), TEXT("wheatEar"), NSLOCTEXT("Holypaw", "WheatPick", "wheat ear"));

	PlaceCube(FVector(Barn.X, Barn.Y, SampleHeight(Barn.X, Barn.Y) + 140.f), FVector(3.4f, 2.4f, 2.6f), FLinearColor(0.62f, 0.42f, 0.28f), MakeName(TEXT("HymnBarn")));
	PlaceCube(FVector(Barn.X, Barn.Y, SampleHeight(Barn.X, Barn.Y) + 280.f), FVector(3.8f, 2.8f, 0.55f), FLinearColor(0.72f, 0.38f, 0.28f), MakeName(TEXT("BarnRoof")));
	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Crows.X + I * 140.f;
		const float Y = Crows.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 90.f), FVector(0.12f, 0.12f, 1.7f), FLinearColor(0.42f, 0.32f, 0.2f), MakeName(TEXT("CrowPole")));
		PlaceCube(FVector(X, Y, Z + 160.f), FVector(0.7f, 0.18f, 0.7f), FLinearColor(0.78f, 0.62f, 0.35f), MakeName(TEXT("CrowHat")));
	}
	for (int32 I = 0; I < 6; ++I)
	{
		const float X = Choir.X + (I % 3) * 120.f - 80.f;
		const float Y = Choir.Y + (I / 3) * 90.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 40.f), FVector(0.18f, 0.18f, 0.7f), FLinearColor(0.88f, 0.74f, 0.32f), MakeName(TEXT("Wheat")));
	}
}

void AHolypawWorldBuilder::BuildEmberfenDistricts()
{
	const FVector2D Walk = Emberfen + FVector2D(120.f, 40.f);
	const FVector2D Pool = Emberfen + FVector2D(-600.f, 500.f);
	const FVector2D Stoop = Emberfen + FVector2D(800.f, -200.f);
	const FVector2D Choir = Emberfen + FVector2D(-200.f, -700.f);
	const FVector2D Board = Emberfen + FVector2D(400.f, 900.f);

	PlaceSign(Walk, NSLOCTEXT("Holypaw", "FenWalk", "Peat Walk  |  boots remember, paws improve"));
	PlaceSign(Pool, NSLOCTEXT("Holypaw", "FenPool", "Jam Pool  |  frogs the color of cooling jam"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "FenStoop", "Witch Stoop  |  F-sharp hiss, hymn in G"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "FenChoir", "Hollow Choir  |  peat that claps wetly"));
	PlaceSign(Board, NSLOCTEXT("Holypaw", "FenBoard", "Leech Board  |  Bog King south in the deepest jam"));

	PlaceShrine(Walk + FVector2D(180.f, 60.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "FenInn", "Peat Inn"));
	PlaceShrine(Pool + FVector2D(40.f, 20.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "JamFont", "Jam Font"));
	PlaceShrine(Stoop + FVector2D(-40.f, 50.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "FenChap", "Stoop Chapel"));
	PlaceShrine(Board + FVector2D(-80.f, 30.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "FenCrate", "Peat Crate"));

	PlacePickup(Stoop + FVector2D(70.f, -40.f), TEXT("peatBead"), NSLOCTEXT("Holypaw", "PeatPick", "peat bead"));

	PlaceCube(FVector(Pool.X, Pool.Y, SampleHeight(Pool.X, Pool.Y) + 18.f), FVector(2.8f, 2.2f, 0.16f), FLinearColor(0.62f, 0.28f, 0.32f), MakeName(TEXT("JamSheet")));
	PlaceCube(FVector(Stoop.X, Stoop.Y, SampleHeight(Stoop.X, Stoop.Y) + 50.f), FVector(1.6f, 1.2f, 0.22f), FLinearColor(0.38f, 0.24f, 0.2f), MakeName(TEXT("WitchStoop")));
	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Walk.X + I * 150.f;
		const float Y = Walk.Y - 80.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 35.f), FVector(0.7f, 0.5f, 0.45f), FLinearColor(0.42f, 0.28f, 0.22f), MakeName(TEXT("PeatStack")));
	}
}

void AHolypawWorldBuilder::BuildSnowveilDistricts()
{
	const FVector2D Gate = Snowveil + FVector2D(40.f, 40.f);
	const FVector2D Scarf = Snowveil + FVector2D(-800.f, 300.f);
	const FVector2D Stoop = Snowveil + FVector2D(700.f, -200.f);
	const FVector2D Choir = Snowveil + FVector2D(100.f, 900.f);
	const FVector2D Notch = Snowveil + FVector2D(-300.f, -700.f);

	PlaceSign(Gate, NSLOCTEXT("Holypaw", "SnowGate", "Ridge Gate  |  Snowveil, then peak, then the Unmaker"));
	PlaceSign(Scarf, NSLOCTEXT("Holypaw", "SnowScarf", "Scarf Walk  |  aurora thread, not polyester yarn"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "SnowStoop", "Aurora Stoop  |  lights copy you if you glow"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "SnowChoir", "Ice Choir  |  hymns that freeze mid-clap"));
	PlaceSign(Notch, NSLOCTEXT("Holypaw", "SnowNotch", "Lamp Notch  |  last warm cube before Velvet Peak"));

	PlaceShrine(Gate + FVector2D(200.f, 60.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "SnowInn", "Ridge Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "IceChap", "Ice Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "AuroraFont", "Aurora Font"));
	PlaceShrine(Notch + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "SnowCrate", "Ridge Crate"));

	PlacePickup(Scarf + FVector2D(60.f, -50.f), TEXT("auroraThread"), NSLOCTEXT("Holypaw", "ThreadPick", "aurora thread"));

	PlaceCube(FVector(Gate.X - 80.f, Gate.Y, SampleHeight(Gate.X, Gate.Y) + 160.f), FVector(0.35f, 2.4f, 3.2f), FLinearColor(0.72f, 0.78f, 0.88f), MakeName(TEXT("RidgePost")));
	PlaceCube(FVector(Gate.X + 80.f, Gate.Y, SampleHeight(Gate.X, Gate.Y) + 160.f), FVector(0.35f, 2.4f, 3.2f), FLinearColor(0.72f, 0.78f, 0.88f), MakeName(TEXT("RidgePostB")));
	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Scarf.X + I * 140.f;
		const float Y = Scarf.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 110.f), FVector(0.1f, 0.1f, 2.0f), FLinearColor(0.85f, 0.88f, 0.95f), MakeName(TEXT("ScarfPole")));
		PlaceCube(FVector(X + 30.f, Y, Z + 150.f), FVector(0.7f, 0.08f, 0.9f), FLinearColor(0.55f, 0.72f, 0.95f), MakeName(TEXT("Scarf")));
	}
	PlaceCube(FVector(Stoop.X, Stoop.Y, SampleHeight(Stoop.X, Stoop.Y) + 220.f), FVector(0.8f, 0.8f, 0.35f), FLinearColor(0.65f, 0.82f, 1.f), MakeName(TEXT("AuroraLamp")));
}

void AHolypawWorldBuilder::BuildLanternAngelesDistricts()
{
	const FVector2D Walk = LanternAngeles + FVector2D(80.f, -40.f);
	const FVector2D Lot = LanternAngeles + FVector2D(-800.f, 500.f);
	const FVector2D Alley = LanternAngeles + FVector2D(200.f, -900.f);
	const FVector2D Grid = LanternAngeles + FVector2D(1100.f, 200.f);
	const FVector2D Choir = LanternAngeles + FVector2D(-200.f, 1100.f);

	PlaceSign(Walk, NSLOCTEXT("Holypaw", "LAWalk", "Marquee Walk  |  gold posters lie, fur does not"));
	PlaceSign(Lot, NSLOCTEXT("Holypaw", "LALot", "Lot Nine  |  commercials for identical pets, lights dropped on purpose"));
	PlaceSign(Alley, NSLOCTEXT("Holypaw", "LAAlley", "Extra Alley  |  clap off-script, crate of leftover takes"));
	PlaceSign(Grid, NSLOCTEXT("Holypaw", "LAGrid", "Lamp Grid  |  cables that used to power identical smiles"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "LAChoir", "Poster Choir  |  popcorn theology, toss a thought"));

	PlaceShrine(Walk + FVector2D(220.f, 80.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "LAInn", "Lot Inn"));
	PlaceShrine(Lot + FVector2D(60.f, 40.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "LAChap", "Marquee Chapel"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "PosterFont", "Poster Font"));
	PlaceShrine(Alley + FVector2D(-40.f, 50.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "LACrate", "Take Crate"));
	PlaceStall(Walk + FVector2D(160.f, -80.f));

	PlacePickup(Walk + FVector2D(-70.f, 60.f), TEXT("goldPoster"), NSLOCTEXT("Holypaw", "PosterPick", "gold poster"));

	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Walk.X + I * 160.f;
		const float Y = Walk.Y - 50.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 140.f), FVector(1.1f, 0.12f, 1.8f), FLinearColor(0.95f, 0.72f, 0.28f), MakeName(TEXT("Marquee")));
		PlaceCube(FVector(X, Y, Z + 40.f), FVector(0.18f, 0.18f, 0.8f), FLinearColor(0.35f, 0.28f, 0.22f), MakeName(TEXT("MarqueePole")));
	}
	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Grid.X + (I % 3) * 140.f;
		const float Y = Grid.Y + (I / 3) * 160.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 160.f), FVector(0.1f, 0.1f, 3.0f), FLinearColor(0.92f, 0.82f, 0.45f), MakeName(TEXT("LotLamp")));
	}
	PlaceCube(FVector(Lot.X, Lot.Y, SampleHeight(Lot.X, Lot.Y) + 18.f), FVector(4.2f, 3.4f, 0.12f), FLinearColor(0.42f, 0.4f, 0.38f), MakeName(TEXT("LotFloor")));
}

void AHolypawWorldBuilder::BuildMossgateDistricts()
{
	const FVector2D Gate = Mossgate + FVector2D(40.f, 40.f);
	const FVector2D Tea = Mossgate + FVector2D(-700.f, 200.f);
	const FVector2D Choir = Mossgate + FVector2D(100.f, 900.f);
	const FVector2D Stoop = Mossgate + FVector2D(800.f, -200.f);
	const FVector2D Path = Mossgate + FVector2D(-200.f, -800.f);

	PlaceSign(Gate, NSLOCTEXT("Holypaw", "MossGate", "Fog Gate  |  damp on purpose, dry thoughts mill faster"));
	PlaceSign(Tea, NSLOCTEXT("Holypaw", "MossTea", "Tea Walk  |  fog that tastes like a bear if you believe"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "MossChoir", "Moss Choir  |  hymns dunked in humidity"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "MossStoop", "Rain Stoop  |  wishes that come back steamed"));
	PlaceSign(Path, NSLOCTEXT("Holypaw", "MossPath", "Quilt Path  |  north woods, Night Thread in the rain"));

	PlaceShrine(Gate + FVector2D(200.f, 60.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "FogInn", "Fog Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "MossChap", "Moss Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "RainFont", "Rain Font"));
	PlaceShrine(Path + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "MossCrate", "Moss Crate"));

	PlacePickup(Tea + FVector2D(50.f, -40.f), TEXT("fogTea"), NSLOCTEXT("Holypaw", "TeaPick", "fog tea"));

	PlaceCube(FVector(Gate.X - 70.f, Gate.Y, SampleHeight(Gate.X, Gate.Y) + 140.f), FVector(0.28f, 2.0f, 2.8f), FLinearColor(0.42f, 0.58f, 0.52f), MakeName(TEXT("FogPost")));
	PlaceCube(FVector(Gate.X + 70.f, Gate.Y, SampleHeight(Gate.X, Gate.Y) + 140.f), FVector(0.28f, 2.0f, 2.8f), FLinearColor(0.42f, 0.58f, 0.52f), MakeName(TEXT("FogPostB")));
	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Tea.X + I * 130.f;
		const float Y = Tea.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 55.f), FVector(0.45f, 0.45f, 0.7f), FLinearColor(0.72f, 0.62f, 0.42f), MakeName(TEXT("TeaCart")));
		PlaceCube(FVector(X, Y, Z + 95.f), FVector(0.22f, 0.22f, 0.28f), FLinearColor(0.55f, 0.72f, 0.62f), MakeName(TEXT("TeaSteam")));
	}
	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Path.X + I * 110.f;
		const float Y = Path.Y + (I % 2) * 40.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 28.f), FVector(0.55f, 0.4f, 0.35f), FLinearColor(0.38f, 0.52f, 0.42f), MakeName(TEXT("MossStack")));
	}
}

void AHolypawWorldBuilder::BuildPalmaDuskDistricts()
{
	const FVector2D Walk = PalmaDusk + FVector2D(-600.f, 80.f);
	const FVector2D Pier = PalmaDusk + FVector2D(400.f, 200.f);
	const FVector2D Choir = PalmaDusk + FVector2D(80.f, 1000.f);
	const FVector2D Stoop = PalmaDusk + FVector2D(800.f, -180.f);
	const FVector2D Notch = PalmaDusk + FVector2D(-100.f, -750.f);

	PlaceSign(Walk, NSLOCTEXT("Holypaw", "PalmWalk", "Palm Walk  |  trunks that refuse to be a brochure"));
	PlaceSign(Pier, NSLOCTEXT("Holypaw", "PalmPier", "Clap Pier  |  palms copy you if you glow"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "PalmChoir", "Dusk Choir  |  off-key on purpose, salt optional"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "PalmStoop", "Usher Stoop  |  better plot, same chairs"));
	PlaceSign(Notch, NSLOCTEXT("Holypaw", "PalmNotch", "Sand Notch  |  last warm cube before the dunes"));

	PlaceShrine(Pier + FVector2D(180.f, 60.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "PalmInn", "Palm Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "DuskChap", "Dusk Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "DuskFont", "Dusk Font"));
	PlaceShrine(Notch + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "PalmCrate", "Pier Crate"));

	PlacePickup(Pier + FVector2D(-80.f, 70.f), TEXT("palmClap"), NSLOCTEXT("Holypaw", "ClapPick", "palm clap"));

	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Walk.X + I * 150.f;
		const float Y = Walk.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 120.f), FVector(0.18f, 0.18f, 2.4f), FLinearColor(0.42f, 0.32f, 0.22f), MakeName(TEXT("PalmTrunk")));
		PlaceCube(FVector(X, Y, Z + 250.f), FVector(1.1f, 1.1f, 0.7f), FLinearColor(0.28f, 0.72f, 0.48f), MakeName(TEXT("PalmCrown")));
	}
	for (int32 I = 0; I < 6; ++I)
	{
		const float X = Pier.X + I * 90.f;
		const float Y = Pier.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 8.f + I * 6.f), FVector(1.5f, 4.0f, 0.12f), FLinearColor(0.62f, 0.52f, 0.38f), MakeName(TEXT("ClapPlank")));
	}
	PlaceCube(FVector(Choir.X, Choir.Y, SampleHeight(Choir.X, Choir.Y) + 36.f), FVector(1.6f, 1.6f, 0.18f), FLinearColor(0.32f, 0.68f, 0.62f), MakeName(TEXT("DuskPool")));
}

void AHolypawWorldBuilder::BuildCherryLoomDistricts()
{
	const FVector2D Gate = CherryLoom + FVector2D(40.f, 40.f);
	const FVector2D Loom = CherryLoom + FVector2D(-750.f, 280.f);
	const FVector2D Choir = CherryLoom + FVector2D(120.f, 950.f);
	const FVector2D Stoop = CherryLoom + FVector2D(780.f, -160.f);
	const FVector2D Notch = CherryLoom + FVector2D(-80.f, -720.f);

	PlaceSign(Gate, NSLOCTEXT("Holypaw", "CherryGate", "Blossom Gate  |  no logos, only pink"));
	PlaceSign(Loom, NSLOCTEXT("Holypaw", "CherryLoomWalk", "Loom Walk  |  thread that remembers fingers"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "CherryChoir", "Pink Choir  |  hymns dunked in sap"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "CherryStoop", "Thread Stoop  |  wishes that come back wrapped"));
	PlaceSign(Notch, NSLOCTEXT("Holypaw", "CherryNotch", "Ferry Notch  |  hop off the island when the wrap holds"));

	PlaceShrine(Gate + FVector2D(210.f, 70.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "BlossomInn", "Blossom Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "PinkChap", "Pink Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "ThreadFont", "Thread Font"));
	PlaceShrine(Notch + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "CherryCrate", "Isle Crate"));

	PlacePickup(Loom + FVector2D(60.f, -50.f), TEXT("cherryThread"), NSLOCTEXT("Holypaw", "CherryPick", "cherry thread"));

	PlaceCube(FVector(Gate.X - 80.f, Gate.Y, SampleHeight(Gate.X, Gate.Y) + 150.f), FVector(0.32f, 2.2f, 3.0f), FLinearColor(0.92f, 0.55f, 0.65f), MakeName(TEXT("BlossomPost")));
	PlaceCube(FVector(Gate.X + 80.f, Gate.Y, SampleHeight(Gate.X, Gate.Y) + 150.f), FVector(0.32f, 2.2f, 3.0f), FLinearColor(0.92f, 0.55f, 0.65f), MakeName(TEXT("BlossomPostB")));
	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Loom.X + I * 140.f;
		const float Y = Loom.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 70.f), FVector(1.2f, 0.35f, 0.9f), FLinearColor(0.72f, 0.42f, 0.48f), MakeName(TEXT("LoomFrame")));
		PlaceCube(FVector(X + 20.f, Y, Z + 160.f), FVector(0.55f, 0.55f, 0.45f), FLinearColor(0.95f, 0.62f, 0.72f), MakeName(TEXT("Blossom")));
	}
	for (int32 I = 0; I < 6; ++I)
	{
		const float X = Choir.X + (I % 3) * 110.f - 80.f;
		const float Y = Choir.Y + (I / 3) * 90.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 50.f), FVector(0.2f, 0.2f, 0.85f), FLinearColor(0.88f, 0.45f, 0.58f), MakeName(TEXT("PinkStem")));
	}
}

void AHolypawWorldBuilder::BuildSkyRift()
{
	const float Z = SampleHeight(PeakCenter.X, PeakCenter.Y);
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 3200.f), FVector(14.f, 14.f, 0.35f), FLinearColor(0.72f, 0.55f, 1.f), MakeName(TEXT("SkyRiftRing")));
	PlaceCube(FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 2800.f), FVector(1.1f, 1.1f, 12.f), FLinearColor(1.f, 0.88f, 0.55f), MakeName(TEXT("SkyRiftBeam")));
	PlaceCube(FVector(PeakCenter.X + 900.f, PeakCenter.Y - 400.f, Z + 3400.f), FVector(3.5f, 0.8f, 0.5f), FLinearColor(0.95f, 0.7f, 0.45f), MakeName(TEXT("SkyRiftCloud")));
	PlaceCube(FVector(PeakCenter.X - 800.f, PeakCenter.Y, Z + 3300.f), FVector(4.2f, 1.0f, 0.55f), FLinearColor(0.85f, 0.62f, 0.95f), MakeName(TEXT("SkyRiftCloudB")));
	PlacePrimitive(SphereMesh, FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 2500.f), FVector(2.2f, 2.2f, 2.2f), HolypawLook::GoldWarm, MakeName(TEXT("SkyRiftCore")));
	HolypawLook::SpawnGlow(GetWorld(), this, FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 2500.f), HolypawLook::GoldWarm, 8000.f, 4200.f);
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
	HolypawLook::SpawnGlow(GetWorld(), this, FVector(PeakCenter.X, PeakCenter.Y - 400.f, Z + 2280.f), HolypawLook::GoldWarm, 3600.f, 1400.f);
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
			if (I == 0)
			{
				AddKit(Foam, FVector(X - Step * 0.42f, Y, Z + 8.f), FRotator::ZeroRotator, FVector(0.18f, Step / 100.f, 0.05f));
			}
			if (I == NX - 1)
			{
				AddKit(Foam, FVector(X + Step * 0.42f, Y, Z + 8.f), FRotator::ZeroRotator, FVector(0.18f, Step / 100.f, 0.05f));
			}
			if (J == 0)
			{
				AddKit(Foam, FVector(X, Y - Step * 0.42f, Z + 8.f), FRotator::ZeroRotator, FVector(Step / 100.f, 0.18f, 0.05f));
			}
			if (J == NY - 1)
			{
				AddKit(Foam, FVector(X, Y + Step * 0.42f, Z + 8.f), FRotator::ZeroRotator, FVector(Step / 100.f, 0.18f, 0.05f));
			}
		}
	}
}

void AHolypawWorldBuilder::PlaceCamp(const FVector2D& XY, const FText& Name)
{
	const float Z = SampleHeight(XY.X, XY.Y);
	PlaceCampKit(FVector(XY.X, XY.Y, Z));
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
	FVector Pad = FVector(XY.X, XY.Y, Z + 36.f);
	if (Kind == EHolypawShrineKind::Inn || Kind == EHolypawShrineKind::Chapel || Kind == EHolypawShrineKind::Workshop)
	{
		DressInterior(FVector(XY.X, XY.Y, Z), Kind);
		Pad = FVector(XY.X, XY.Y + 40.f, Z + 18.f);
	}
	else
	{
		FLinearColor Col(0.78f, 0.52f, 0.42f);
		FVector Scale(2.6f, 2.2f, 2.8f);
		if (Kind == EHolypawShrineKind::Wish)
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
	}
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (AHolypawShrine* S = GetWorld()->SpawnActor<AHolypawShrine>(Pad, FRotator::ZeroRotator, Sp))
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

void AHolypawWorldBuilder::PlaceStall(const FVector2D& XY, const bool bOpenAir, const TCHAR* KeepName)
{
	const float Z = SampleHeight(XY.X, XY.Y);
	if (bOpenAir)
	{
		DressOpenStall(FVector(XY.X, XY.Y, Z));
	}
	else
	{
		DressShopRoom(FVector(XY.X, XY.Y, Z));
	}
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (AFaithStall* S = GetWorld()->SpawnActor<AFaithStall>(FVector(XY.X, XY.Y + 40.f, Z + 18.f), FRotator::ZeroRotator, Sp))
	{
		S->bOpenAir = bOpenAir;
		S->SetActorScale3D(FVector(1.1f, 1.1f, 0.25f));
	}
	if (AHugHuman* Keep = GetWorld()->SpawnActor<AHugHuman>(FVector(XY.X + 80.f, XY.Y + 90.f, Z + 50.f), FRotator::ZeroRotator, Sp))
	{
		FString Name = KeepName ? FString(KeepName) : FString();
		if (Name.IsEmpty())
		{
			Name = FString::Printf(TEXT("Shopkeep %d"),
				FMath::Abs(FMath::RoundToInt(XY.X * 0.01f) * 31 + FMath::RoundToInt(XY.Y * 0.01f)));
		}
		Keep->PersonName = FText::FromString(Name);
		Keep->ShirtColor = FLinearColor(0.95f, 0.72f, 0.35f);
		Keep->SetSolidColor(Keep->ShirtColor);
		Keep->bTendsStall = true;
	}
}

void AHolypawWorldBuilder::PlaceSign(const FVector2D& XY, const FText& Message, const FName LivingId)
{
	const float Z = SampleHeight(XY.X, XY.Y);
	PlaceCube(FVector(XY.X, XY.Y, Z + 70.f), FVector(0.16f, 0.16f, 1.4f), FLinearColor(0.4f, 0.3f, 0.2f), MakeName(TEXT("SignPole")));
	PlaceCube(FVector(XY.X, XY.Y, Z + 150.f), FVector(1.1f, 0.12f, 0.7f), FLinearColor(0.72f, 0.58f, 0.4f), MakeName(TEXT("SignBoard")));
	PlaceCube(FVector(XY.X, XY.Y, Z + 8.f), FVector(0.42f, 0.42f, 0.12f), HolypawLook::Wood, MakeName(TEXT("SignBase")));
	PlaceCube(FVector(XY.X, XY.Y + 8.f, Z + 150.f), FVector(0.95f, 0.04f, 0.48f), HolypawLook::Cream, MakeName(TEXT("SignFace")), FRotator::ZeroRotator, false);
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ASignpost* S = GetWorld()->SpawnActor<ASignpost>(FVector(XY.X, XY.Y, Z + 40.f), FRotator::ZeroRotator, Sp))
	{
		S->Message = Message;
		S->LivingId = LivingId;
		if (!LivingId.IsNone())
		{
			LivingSigns.Add(S);
			S->RefreshFromHearts(0);
		}
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
	SpawnHuman(TEXT("Floor Foreman"), FVector2D(RibbonCity.X + 5100.f, RibbonCity.Y - 800.f), FLinearColor(0.62f, 0.58f, 0.5f));
	SpawnHuman(TEXT("Plaza Florist"), FVector2D(RibbonCity.X - 160.f, RibbonCity.Y - 280.f), FLinearColor(0.88f, 0.45f, 0.62f));
	SpawnHuman(TEXT("Night Watch"), FVector2D(RibbonCity.X + 240.f, RibbonCity.Y - 3600.f), FLinearColor(0.35f, 0.42f, 0.62f));
	SpawnHuman(TEXT("Harbor Hand"), FVector2D(Tidewell.X + 400.f, Tidewell.Y + 200.f), FLinearColor(0.45f, 0.55f, 0.7f));
	SpawnHuman(TEXT("Net Weaver"), FVector2D(Tidewell.X - 200.f, Tidewell.Y - 300.f), FLinearColor(0.4f, 0.7f, 0.75f));
	SpawnHuman(TEXT("Ferry Clerk"), FVector2D(Tidewell.X + 2200.f, Tidewell.Y + 240.f), FLinearColor(0.55f, 0.6f, 0.72f));
	SpawnHuman(TEXT("Salt Priest"), FVector2D(Tidewell.X - 380.f, Tidewell.Y + 1120.f), FLinearColor(0.7f, 0.82f, 0.88f));
	SpawnHuman(TEXT("Pearl Diver"), FVector2D(Tidewell.X - 980.f, Tidewell.Y - 360.f), FLinearColor(0.35f, 0.62f, 0.7f));
	SpawnHuman(TEXT("Hook Cook"), FVector2D(Tidewell.X + 480.f, Tidewell.Y + 80.f), FLinearColor(0.72f, 0.48f, 0.4f));
	SpawnHuman(TEXT("Farmer"), FVector2D(Hearthfold.X + 250.f, Hearthfold.Y - 200.f), FLinearColor(0.7f, 0.6f, 0.3f));
	SpawnHuman(TEXT("Miller"), FVector2D(Hearthfold.X - 350.f, Hearthfold.Y + 150.f), FLinearColor(0.65f, 0.5f, 0.35f));
	SpawnHuman(TEXT("Thresh Singer"), FVector2D(Hearthfold.X - 180.f, Hearthfold.Y - 780.f), FLinearColor(0.88f, 0.72f, 0.35f));
	SpawnHuman(TEXT("Grain Widow"), FVector2D(Hearthfold.X - 680.f, Hearthfold.Y + 420.f), FLinearColor(0.72f, 0.48f, 0.38f));
	SpawnHuman(TEXT("Scarecrow Mender"), FVector2D(Hearthfold.X + 920.f, Hearthfold.Y + 220.f), FLinearColor(0.78f, 0.62f, 0.4f));
	SpawnHuman(TEXT("Fen Guide"), FVector2D(Emberfen.X + 180.f, Emberfen.Y + 80.f), FLinearColor(0.55f, 0.35f, 0.32f));
	SpawnHuman(TEXT("Mud Sculptor"), FVector2D(Emberfen.X - 220.f, Emberfen.Y - 120.f), FLinearColor(0.5f, 0.38f, 0.3f));
	SpawnHuman(TEXT("Frog Bell"), FVector2D(Emberfen.X - 580.f, Emberfen.Y + 520.f), FLinearColor(0.42f, 0.62f, 0.38f));
	SpawnHuman(TEXT("Peat Baker"), FVector2D(Emberfen.X + 160.f, Emberfen.Y + 60.f), FLinearColor(0.62f, 0.32f, 0.28f));
	SpawnHuman(TEXT("Jam Priest"), FVector2D(Emberfen.X - 180.f, Emberfen.Y - 680.f), FLinearColor(0.72f, 0.28f, 0.38f));
	SpawnHuman(TEXT("Snow Warden"), FVector2D(Snowveil.X + 160.f, Snowveil.Y - 80.f), FLinearColor(0.8f, 0.85f, 0.95f));
	SpawnHuman(TEXT("Scarf Knitter"), FVector2D(Snowveil.X - 780.f, Snowveil.Y + 320.f), FLinearColor(0.7f, 0.82f, 0.95f));
	SpawnHuman(TEXT("Drift Priest"), FVector2D(Snowveil.X + 120.f, Snowveil.Y + 920.f), FLinearColor(0.85f, 0.88f, 0.98f));
	SpawnHuman(TEXT("Ridge Child"), FVector2D(Snowveil.X + 680.f, Snowveil.Y - 180.f), FLinearColor(0.75f, 0.85f, 1.f));
	SpawnHuman(TEXT("Peak Acolyte"), FVector2D(PeakCenter.X - 600.f, PeakCenter.Y - 900.f), FLinearColor(0.75f, 0.65f, 0.95f));
	SpawnHuman(TEXT("Studio Grip"), CityXY(EHolypawZone::LanternAngeles) + FVector2D(200.f, -150.f), FLinearColor(0.95f, 0.78f, 0.35f));
	SpawnHuman(TEXT("Best Boy"), LanternAngeles + FVector2D(1080.f, 180.f), FLinearColor(0.88f, 0.62f, 0.32f));
	SpawnHuman(TEXT("Extra"), LanternAngeles + FVector2D(180.f, -860.f), FLinearColor(0.78f, 0.55f, 0.62f));
	SpawnHuman(TEXT("Marquee Bear"), LanternAngeles + FVector2D(-180.f, 1080.f), FLinearColor(0.95f, 0.82f, 0.42f));
	SpawnHuman(TEXT("Fog Baker"), CityXY(EHolypawZone::Mossgate) + FVector2D(-120.f, 80.f), FLinearColor(0.55f, 0.72f, 0.68f));
	SpawnHuman(TEXT("Moss Warden"), Mossgate + FVector2D(80.f, 60.f), FLinearColor(0.42f, 0.62f, 0.55f));
	SpawnHuman(TEXT("Tea Cart"), Mossgate + FVector2D(-680.f, 220.f), FLinearColor(0.72f, 0.58f, 0.4f));
	SpawnHuman(TEXT("Gate Priest"), Mossgate + FVector2D(120.f, 920.f), FLinearColor(0.62f, 0.78f, 0.72f));
	SpawnHuman(TEXT("Quilt Ranger"), CityXY(EHolypawZone::Quiltland) + FVector2D(80.f, -60.f), FLinearColor(0.42f, 0.58f, 0.4f));
	SpawnHuman(TEXT("Patch Child"), Quiltland + FVector2D(-700.f, 260.f), FLinearColor(0.72f, 0.48f, 0.55f));
	SpawnHuman(TEXT("Pine Priest"), Quiltland + FVector2D(100.f, 900.f), FLinearColor(0.48f, 0.62f, 0.42f));
	SpawnHuman(TEXT("Mesa Guide"), CityXY(EHolypawZone::DustMesa) + FVector2D(40.f, 90.f), FLinearColor(0.85f, 0.6f, 0.35f));
	SpawnHuman(TEXT("Bead Sifter"), DustMesa + FVector2D(-780.f, 180.f), FLinearColor(0.82f, 0.58f, 0.32f));
	SpawnHuman(TEXT("Canyon Priest"), DustMesa + FVector2D(120.f, 860.f), FLinearColor(0.78f, 0.62f, 0.4f));
	SpawnHuman(TEXT("Palm Singer"), CityXY(EHolypawZone::PalmaDusk) + FVector2D(-80.f, 40.f), FLinearColor(0.35f, 0.78f, 0.55f));
	SpawnHuman(TEXT("Dusk Usher"), PalmaDusk + FVector2D(780.f, -160.f), FLinearColor(0.92f, 0.62f, 0.4f));
	SpawnHuman(TEXT("Clap Kid"), PalmaDusk + FVector2D(-580.f, 100.f), FLinearColor(0.45f, 0.82f, 0.7f));
	SpawnHuman(TEXT("Pier Priest"), PalmaDusk + FVector2D(100.f, 980.f), FLinearColor(0.55f, 0.7f, 0.78f));
	SpawnHuman(TEXT("Ivory Clerk"), CityXY(EHolypawZone::IvorySpire) + FVector2D(60.f, -40.f), FLinearColor(0.9f, 0.86f, 0.75f));
	SpawnHuman(TEXT("Stamp Kid"), IvorySpire + FVector2D(-740.f, 260.f), FLinearColor(0.88f, 0.78f, 0.62f));
	SpawnHuman(TEXT("Spire Priest"), IvorySpire + FVector2D(110.f, 920.f), FLinearColor(0.92f, 0.88f, 0.8f));
	SpawnHuman(TEXT("Sand Priest"), CityXY(EHolypawZone::SandHymn) + FVector2D(90.f, 20.f), FLinearColor(0.92f, 0.74f, 0.4f));
	SpawnHuman(TEXT("Dune Child"), SandHymn + FVector2D(-700.f, 200.f), FLinearColor(0.95f, 0.78f, 0.48f));
	SpawnHuman(TEXT("Shade Priest"), SandHymn + FVector2D(90.f, 880.f), FLinearColor(0.88f, 0.7f, 0.38f));
	SpawnHuman(TEXT("Cape Lookout"), CityXY(EHolypawZone::CapePlush) + FVector2D(-50.f, 70.f), FLinearColor(0.55f, 0.45f, 0.7f));
	SpawnHuman(TEXT("Shell Kid"), CapePlush + FVector2D(-660.f, 160.f), FLinearColor(0.62f, 0.52f, 0.78f));
	SpawnHuman(TEXT("Spray Priest"), CapePlush + FVector2D(80.f, 840.f), FLinearColor(0.48f, 0.62f, 0.72f));
	SpawnHuman(TEXT("Loom Weaver"), CityXY(EHolypawZone::CherryLoom) + FVector2D(140.f, -90.f), FLinearColor(0.92f, 0.5f, 0.6f));
	SpawnHuman(TEXT("Cherry Priest"), CherryLoom + FVector2D(140.f, 940.f), FLinearColor(0.95f, 0.55f, 0.68f));
	SpawnHuman(TEXT("Silk Child"), CherryLoom + FVector2D(760.f, -140.f), FLinearColor(0.88f, 0.62f, 0.78f));
	SpawnHuman(TEXT("Gate Watch"), CherryLoom + FVector2D(60.f, 80.f), FLinearColor(0.72f, 0.42f, 0.52f));
	SpawnHuman(TEXT("Aurora Child"), CityXY(EHolypawZone::AuroraBorough) + FVector2D(-70.f, 40.f), FLinearColor(0.55f, 0.8f, 0.95f));
	SpawnHuman(TEXT("Mitten Pair"), AuroraBorough + FVector2D(-700.f, 220.f), FLinearColor(0.72f, 0.55f, 0.85f));
	SpawnHuman(TEXT("Light Priest"), AuroraBorough + FVector2D(100.f, 860.f), FLinearColor(0.55f, 0.78f, 0.98f));
	SpawnHuman(TEXT("Tundra Keeper"), CityXY(EHolypawZone::TundraParish) + FVector2D(30.f, -40.f), FLinearColor(0.8f, 0.85f, 0.95f));
	SpawnHuman(TEXT("Snow Page"), TundraParish + FVector2D(-620.f, 160.f), FLinearColor(0.88f, 0.9f, 0.98f));
	SpawnHuman(TEXT("Quiet Priest"), TundraParish + FVector2D(70.f, 780.f), FLinearColor(0.75f, 0.82f, 0.95f));
	SpawnHuman(TEXT("Confetti Baker"), CityXY(EHolypawZone::CarnivalBahia) + FVector2D(120.f, -80.f), FLinearColor(0.95f, 0.5f, 0.4f));
	SpawnHuman(TEXT("Drum Kid"), CarnivalBahia + FVector2D(-700.f, 260.f), FLinearColor(0.95f, 0.55f, 0.38f));
	SpawnHuman(TEXT("Parade Priest"), CarnivalBahia + FVector2D(110.f, 900.f), FLinearColor(0.92f, 0.42f, 0.5f));
	SpawnHuman(TEXT("Wool Climber"), CityXY(EHolypawZone::AndesLoom) + FVector2D(-90.f, 70.f), FLinearColor(0.72f, 0.5f, 0.35f));
	SpawnHuman(TEXT("Skein Mender"), AndesLoom + FVector2D(-660.f, 180.f), FLinearColor(0.68f, 0.45f, 0.32f));
	SpawnHuman(TEXT("Height Priest"), AndesLoom + FVector2D(90.f, 820.f), FLinearColor(0.78f, 0.55f, 0.4f));
	SpawnHuman(TEXT("Bell Warden"), CityXY(EHolypawZone::Clockhaven) + FVector2D(80.f, 40.f), FLinearColor(0.6f, 0.66f, 0.78f));
	SpawnHuman(TEXT("Cog Priest"), Clockhaven + FVector2D(-760.f, 300.f), FLinearColor(0.58f, 0.62f, 0.74f));
	SpawnHuman(TEXT("Fog Usher"), Clockhaven + FVector2D(140.f, 960.f), FLinearColor(0.7f, 0.74f, 0.82f));
	SpawnHuman(TEXT("Ribbon Baker"), CityXY(EHolypawZone::VelvetSeine) + FVector2D(-60.f, -50.f), FLinearColor(0.82f, 0.55f, 0.74f));
	SpawnHuman(TEXT("Pastry Kid"), VelvetSeine + FVector2D(-680.f, 220.f), FLinearColor(0.92f, 0.62f, 0.7f));
	SpawnHuman(TEXT("River Priest"), VelvetSeine + FVector2D(80.f, 920.f), FLinearColor(0.78f, 0.52f, 0.68f));
	SpawnHuman(TEXT("Column Sitter"), CityXY(EHolypawZone::MarbleForum) + FVector2D(70.f, 90.f), FLinearColor(0.9f, 0.84f, 0.7f));
	SpawnHuman(TEXT("Hat Mender"), MarbleForum + FVector2D(-720.f, 240.f), FLinearColor(0.82f, 0.48f, 0.58f));
	SpawnHuman(TEXT("Stone Priest"), MarbleForum + FVector2D(100.f, 880.f), FLinearColor(0.86f, 0.8f, 0.68f));
	SpawnHuman(TEXT("Grass Bell"), CityXY(EHolypawZone::SavannahBell) + FVector2D(-40.f, 60.f), FLinearColor(0.88f, 0.72f, 0.35f));
	SpawnHuman(TEXT("Acacia Kid"), SavannahBell + FVector2D(-720.f, 240.f), FLinearColor(0.9f, 0.68f, 0.32f));
	SpawnHuman(TEXT("Wind Priest"), SavannahBell + FVector2D(100.f, 860.f), FLinearColor(0.82f, 0.7f, 0.4f));
	SpawnHuman(TEXT("Delta Weaver"), CityXY(EHolypawZone::SilkDelta) + FVector2D(110.f, -70.f), FLinearColor(0.88f, 0.35f, 0.4f));
	SpawnHuman(TEXT("Rumor Kid"), SilkDelta + FVector2D(-740.f, 240.f), FLinearColor(0.9f, 0.4f, 0.48f));
	SpawnHuman(TEXT("Silt Priest"), SilkDelta + FVector2D(120.f, 920.f), FLinearColor(0.82f, 0.32f, 0.38f));
	SpawnHuman(TEXT("Pepper Clerk"), CityXY(EHolypawZone::SpiceHarbor) + FVector2D(-80.f, 50.f), FLinearColor(0.92f, 0.58f, 0.25f));
	SpawnHuman(TEXT("Sail Kid"), SpiceHarbor + FVector2D(-700.f, 200.f), FLinearColor(0.95f, 0.62f, 0.28f));
	SpawnHuman(TEXT("Saffron Priest"), SpiceHarbor + FVector2D(100.f, 880.f), FLinearColor(0.9f, 0.5f, 0.22f));
	SpawnHuman(TEXT("Reef Choir"), CityXY(EHolypawZone::CoralChoir) + FVector2D(50.f, -40.f), FLinearColor(0.35f, 0.78f, 0.82f));
	SpawnHuman(TEXT("Anemone Kid"), CoralChoir + FVector2D(-680.f, 180.f), FLinearColor(0.95f, 0.52f, 0.6f));
	SpawnHuman(TEXT("Lagoon Priest"), CoralChoir + FVector2D(90.f, 840.f), FLinearColor(0.32f, 0.72f, 0.78f));
	SpawnHuman(TEXT("Ice Scarf"), CityXY(EHolypawZone::FeltIceCamp) + FVector2D(-30.f, 40.f), FLinearColor(0.85f, 0.92f, 0.98f));
	SpawnHuman(TEXT("Penguin Usher"), FeltIceCamp + FVector2D(560.f, -120.f), FLinearColor(0.25f, 0.28f, 0.35f));
	SpawnHuman(TEXT("Echo Priest"), FeltIceCamp + FVector2D(60.f, 700.f), FLinearColor(0.82f, 0.9f, 0.98f));

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
	PlaceSign(LanternAngeles + FVector2D(80.f, 200.f), NSLOCTEXT("Holypaw", "VSignLA", "Lot warning  |  Plaza Corp Cats and Razor Petbots between takes"));
	PlaceSign(Mossgate + FVector2D(-40.f, 120.f), NSLOCTEXT("Holypaw", "VSignMoss", "Fog warning  |  Night Thread in the rain, Quiltland further north"));
	PlaceSign(PalmaDusk + FVector2D(60.f, 160.f), NSLOCTEXT("Holypaw", "VSignPalm", "Pier warning  |  Salt Crabs pinch. Cape Plush if the water argues"));
	PlaceSign(CherryLoom + FVector2D(-60.f, 140.f), NSLOCTEXT("Holypaw", "VSignCherry", "Isle warning  |  Void Rats on the west walk. Globe Trek ends when the wrap holds"));
	PlaceSign(Quiltland + FVector2D(40.f, 80.f), NSLOCTEXT("Holypaw", "VSignQuilt", "Woods warning  |  patch the pines, then Dust Mesa if you like beige inventory"));
	PlaceSign(DustMesa + FVector2D(20.f, 80.f), NSLOCTEXT("Holypaw", "VSignMesa", "Canyon warning  |  mill intern cacti. Andes Loom further south"));
	PlaceSign(Clockhaven + FVector2D(40.f, 100.f), NSLOCTEXT("Holypaw", "VSignClock", "Fog warning  |  Ribbon Enforcers on the cobbles. Tea Inn if they win"));
	PlaceSign(VelvetSeine + FVector2D(-20.f, 80.f), NSLOCTEXT("Holypaw", "VSignSeine", "River warning  |  Gold Snippers. Pastry is still a valid government"));
	PlaceSign(MarbleForum + FVector2D(40.f, 80.f), NSLOCTEXT("Holypaw", "VSignForum", "Stone warning  |  Tinsel Golems. Sit converted anyway"));
	PlaceSign(IvorySpire + FVector2D(40.f, 80.f), NSLOCTEXT("Holypaw", "VSignIvory", "Shore warning  |  mill wanted a franchise. The form is lost on purpose"));
	PlaceSign(SandHymn + FVector2D(20.f, 80.f), NSLOCTEXT("Holypaw", "VSignHymn", "Dune warning  |  hum at noon. Cape Plush if the water argues"));
	PlaceSign(CapePlush + FVector2D(20.f, 80.f), NSLOCTEXT("Holypaw", "VSignCape", "Cape warning  |  Harbor Hooks. Two oceans, one bear"));
	PlaceSign(SavannahBell + FVector2D(20.f, 80.f), NSLOCTEXT("Holypaw", "VSignSav", "Grass warning  |  Scarecrow Hounds. Bells hung without a permit"));
	PlaceSign(CarnivalBahia + FVector2D(40.f, 80.f), NSLOCTEXT("Holypaw", "VSignBahia", "Parade warning  |  Plaza Corp Cats. Confetti is still a valid government"));
	PlaceSign(AndesLoom + FVector2D(20.f, 80.f), NSLOCTEXT("Holypaw", "VSignAndes", "Spine warning  |  Stitched Wolves. Don't mill this mountain"));
	PlaceSign(SilkDelta + FVector2D(40.f, 80.f), NSLOCTEXT("Holypaw", "VSignDelta", "River warning  |  Miracle Eaters. Wrap the rumor, not the moon"));
	PlaceSign(SpiceHarbor + FVector2D(20.f, 80.f), NSLOCTEXT("Holypaw", "VSignSpice", "Dock warning  |  Harbor Hooks. Stamp handmade anyway"));
	PlaceSign(CoralChoir + FVector2D(20.f, 80.f), NSLOCTEXT("Holypaw", "VSignCoral", "Reef warning  |  Salt Crabs. Lyrics are just the word bear"));
	PlaceSign(AuroraBorough + FVector2D(20.f, 80.f), NSLOCTEXT("Holypaw", "VSignAurora", "Light warning  |  Frost Moths. Mittens are civic policy"));
	PlaceSign(TundraParish + FVector2D(20.f, 80.f), NSLOCTEXT("Holypaw", "VSignTundra", "Parish warning  |  Drift Wolves. Hymn louder"));
	PlaceSign(FeltIceCamp + FVector2D(20.f, 80.f), NSLOCTEXT("Holypaw", "VSignFelt", "Ice warning  |  Ice Shard Cats. Last scarf on the last ice"));
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
	DressMillHall(FVector(Mill.X, Mill.Y, Z));
	PlaceCube(FVector(Mill.X + 520.f, Mill.Y + 280.f, Z + 380.f), FVector(1.2f, 1.2f, 7.5f),
		FLinearColor(0.72f, 0.28f, 0.38f), MakeName(TEXT("PolyStack")));
	PlaceCube(FVector(Mill.X + 620.f, Mill.Y - 420.f, Z + 180.f), FVector(1.6f, 1.2f, 3.4f),
		FLinearColor(0.55f, 0.52f, 0.48f), MakeName(TEXT("PolyShed")));
	PlaceCube(FVector(Mill.X + 880.f, Mill.Y + 80.f, Z + 160.f), FVector(1.4f, 1.8f, 3.0f),
		FLinearColor(0.52f, 0.5f, 0.46f), MakeName(TEXT("PolyShed")));
	PlaceSign(Mill + FVector2D(-480.f, 0.f),
		NSLOCTEXT("Holypaw", "PolyMill", "POLY MILL  |  walk in  |  cheap polyester, identical smiles, no handmade soul"));
	PlaceRibbonMillBanners();
	PlaceShrine(Mill + FVector2D(40.f, -40.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "MillCrate", "Mill Scrap Crate"));
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
