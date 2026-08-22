#include "Actors/FaithStall.h"
#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"
#include "Faith/HolypawFaithSim.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

AFaithStall::AFaithStall()
{
	PrimaryActorTick.bCanEverTick = true;

	Shutter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shutter"));
	Shutter->SetupAttachment(Root);
	Shutter->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Shutter->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	Shutter->SetRelativeLocation(FVector(0.f, -90.f, 95.f));
	Shutter->SetRelativeScale3D(FVector(2.05f, 0.12f, 1.85f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded() && Shutter)
	{
		Shutter->SetStaticMesh(CubeFinder.Object);
	}
}

void AFaithStall::BeginPlay()
{
	Super::BeginPlay();
	SetSolidColor(FLinearColor(0.95f, 0.78f, 0.35f));
	if (Shutter && ShapeMat)
	{
		if (UMaterialInstanceDynamic* Mid = Shutter->CreateDynamicMaterialInstance(0, ShapeMat))
		{
			Mid->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.42f, 0.34f, 0.3f));
		}
	}
	ApplyShutter(HolypawFaith::ShutterOpenAlpha(HolypawFaith::HeartsHere(this, GetActorLocation())));
}

void AFaithStall::ApplyShutter(const float OpenAlpha)
{
	ShutterAlpha = FMath::Clamp(OpenAlpha, 0.f, 1.f);
	if (!Shutter)
	{
		return;
	}
	const float Z = FMath::Lerp(95.f, 230.f, ShutterAlpha);
	Shutter->SetRelativeLocation(FVector(0.f, -90.f, Z));
	Shutter->SetCollisionEnabled(ShutterAlpha > 0.6f ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
	if (ShapeMat)
	{
		if (UMaterialInstanceDynamic* Mid = Shutter->CreateDynamicMaterialInstance(0, ShapeMat))
		{
			const FLinearColor Closed(0.42f, 0.34f, 0.3f);
			const FLinearColor Open(0.92f, 0.62f, 0.48f);
			Mid->SetVectorParameterValue(TEXT("Color"), FMath::Lerp(Closed, Open, ShutterAlpha));
		}
	}
}

void AFaithStall::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const float Want = HolypawFaith::ShutterOpenAlpha(HolypawFaith::HeartsHere(this, GetActorLocation()));
	ApplyShutter(FMath::FInterpTo(ShutterAlpha, Want, DeltaSeconds, 2.8f));
}

bool AFaithStall::IsUnlatched() const
{
	return HolypawFaith::ShopsOpen(HolypawFaith::HeartsHere(this, GetActorLocation()));
}

FText AFaithStall::GetPrompt() const
{
	if (!IsUnlatched())
	{
		return FText::FromString(HolypawFaith::ShopClosedLine());
	}
	return FText::FromString(TEXT("E  Walk in · faith stall (buns, ribbons, jars)"));
}

bool AFaithStall::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn || !InstigatorPawn->Affection)
	{
		return false;
	}
	if (!IsUnlatched())
	{
		InstigatorPawn->Toast(HolypawFaith::ShopClosedLine());
		return true;
	}
	InstigatorPawn->OpenShop();
	return true;
}
