#include "Actors/HolypawMillBanner.h"
#include "Faith/HolypawFaithSim.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

AHolypawMillBanner::AHolypawMillBanner()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Pole = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pole"));
	Pole->SetupAttachment(Root);
	Pole->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Pole->SetRelativeLocation(FVector(0.f, 0.f, 140.f));
	Pole->SetRelativeScale3D(FVector(0.12f, 0.12f, 2.8f));

	Cloth = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cloth"));
	Cloth->SetupAttachment(Root);
	Cloth->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Cloth->SetRelativeLocation(FVector(48.f, 0.f, 210.f));
	Cloth->SetRelativeScale3D(FVector(1.05f, 0.08f, 1.35f));

	HandmadeRibbon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandmadeRibbon"));
	HandmadeRibbon->SetupAttachment(Root);
	HandmadeRibbon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandmadeRibbon->SetRelativeLocation(FVector(18.f, 0.f, 240.f));
	HandmadeRibbon->SetRelativeScale3D(FVector(0.45f, 0.08f, 0.7f));
	HandmadeRibbon->SetHiddenInGame(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
		Pole->SetStaticMesh(CubeMesh);
		Cloth->SetStaticMesh(CubeMesh);
		HandmadeRibbon->SetStaticMesh(CubeMesh);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MatFinder.Succeeded())
	{
		ShapeMat = MatFinder.Object;
	}
}

void AHolypawMillBanner::BeginPlay()
{
	Super::BeginPlay();
	ClothBase = Cloth ? Cloth->GetRelativeLocation() : FVector(48.f, 0.f, 210.f);
	Colorize(Pole, FLinearColor(0.38f, 0.34f, 0.32f), PoleMid);
	Colorize(Cloth, FLinearColor(0.72f, 0.28f, 0.38f), ClothMid);
	Colorize(HandmadeRibbon, FLinearColor(0.92f, 0.48f, 0.62f), RibbonMid);
	SnapToHearts(HolypawFaith::HeartsAt(this, HomeZone));
}

void AHolypawMillBanner::Colorize(UStaticMeshComponent* Mesh, const FLinearColor& Color, TObjectPtr<UMaterialInstanceDynamic>& Mid)
{
	if (!Mesh || !ShapeMat)
	{
		return;
	}
	if (!Mid)
	{
		Mid = Mesh->CreateDynamicMaterialInstance(0, ShapeMat);
	}
	if (Mid)
	{
		Mid->SetVectorParameterValue(TEXT("Color"), Color);
	}
}

void AHolypawMillBanner::ApplyPitch(const float Pitch)
{
	ClothPitch = Pitch;
	if (Cloth)
	{
		Cloth->SetRelativeRotation(FRotator(Pitch, 0.f, 0.f));
		const float Drop = Pitch / 82.f;
		Cloth->SetRelativeLocation(ClothBase + FVector(Drop * 40.f, 0.f, Drop * -90.f));
		const int32 Bucket = (Pitch > 50.f) ? 2 : ((Pitch > 12.f) ? 1 : 0);
		if (Bucket != ColorBucket)
		{
			ColorBucket = Bucket;
			if (Bucket == 2)
			{
				Colorize(Cloth, FLinearColor(0.55f, 0.42f, 0.4f), ClothMid);
			}
			else if (Bucket == 1)
			{
				Colorize(Cloth, FLinearColor(0.68f, 0.32f, 0.38f), ClothMid);
			}
			else
			{
				Colorize(Cloth, FLinearColor(0.72f, 0.28f, 0.38f), ClothMid);
			}
		}
	}
	if (HandmadeRibbon)
	{
		const bool bShow = Pitch >= 70.f;
		HandmadeRibbon->SetHiddenInGame(!bShow);
		float Wiggle = 0.f;
		if (bShow && GetWorld())
		{
			Wiggle = 8.f * FMath::Sin(GetWorld()->GetTimeSeconds() * 2.4f);
		}
		HandmadeRibbon->SetRelativeRotation(FRotator(Wiggle, 0.f, 0.f));
	}
}

void AHolypawMillBanner::SnapToHearts(const int32 Hearts)
{
	ApplyPitch(HolypawFaith::BannerPitchDegrees(Hearts));
}

void AHolypawMillBanner::BoostDrop(const float Seconds)
{
	DropBoost = FMath::Max(DropBoost, Seconds);
}

void AHolypawMillBanner::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (DropBoost > 0.f)
	{
		DropBoost = FMath::Max(0.f, DropBoost - DeltaSeconds);
	}
	const int32 Hearts = HolypawFaith::HeartsAt(this, HomeZone);
	const float Want = HolypawFaith::BannerPitchDegrees(Hearts);
	const float Rate = DropBoost > 0.f ? 7.5f : 2.4f;
	ApplyPitch(FMath::FInterpTo(ClothPitch, Want, DeltaSeconds, Rate));
}
