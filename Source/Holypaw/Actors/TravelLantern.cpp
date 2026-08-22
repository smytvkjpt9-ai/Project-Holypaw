#include "Actors/TravelLantern.h"
#include "Character/HolypawCharacter.h"
#include "Look/HolypawLook.h"
#include "HolypawCatalog.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"

ATravelLantern::ATravelLantern()
{
	PrimaryActorTick.bCanEverTick = true;
	Globe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Globe"));
	Globe->SetupAttachment(Root);
	Globe->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	Globe->SetRelativeScale3D(FVector(0.55f, 0.55f, 0.55f));
	Hood = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hood"));
	Hood->SetupAttachment(Globe);
	Hood->SetRelativeLocation(FVector(0.f, 0.f, 18.f));
	Hood->SetRelativeRotation(FRotator(180.f, 0.f, 0.f));
	Hood->SetRelativeScale3D(FVector(0.85f, 0.85f, 0.35f));
	Glow = CreateDefaultSubobject<UPointLightComponent>(TEXT("Glow"));
	Glow->SetupAttachment(Globe);
}

void ATravelLantern::BeginPlay()
{
	Super::BeginPlay();
	if (CylMesh)
	{
		Mesh->SetStaticMesh(CylMesh);
	}
	Mesh->SetRelativeScale3D(FVector(0.18f, 0.18f, 1.35f));
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	SetSolidColor(HolypawLook::Wood);
	HolypawLook::PrepPart(Globe, SphereMesh);
	HolypawLook::PrepPart(Hood, ConeMesh ? ConeMesh : SphereMesh);
	if (Globe)
	{
		Globe->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
		Globe->SetRelativeScale3D(FVector(0.42f, 0.42f, 0.42f));
	}
	if (ShapeMat && Globe)
	{
		Globe->SetMaterial(0, ShapeMat);
	}
	if (ShapeMat && Hood)
	{
		Hood->SetMaterial(0, ShapeMat);
	}
	HolypawLook::Paint(Globe, HolypawLook::Lantern);
	HolypawLook::Paint(Hood, HolypawLook::Wood);
	HolypawLook::DressLanternLight(Glow, HolypawLook::Lantern);
}

void ATravelLantern::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!Glow)
	{
		return;
	}
	const float Pulse = 0.72f + 0.28f * FMath::Sin(GetWorld()->GetTimeSeconds() * 2.6f);
	Glow->SetIntensity(2600.f * Pulse);
	if (Globe)
	{
		Globe->SetRelativeScale3D(FVector(0.40f + 0.08f * Pulse));
	}
}

FText ATravelLantern::GetPrompt() const
{
	return FText::Format(NSLOCTEXT("Holypaw", "Lantern", "E  Lantern — open map travel  ({0})"),
		FText::FromString(HolypawCatalog::ZoneDisplayName(AnchorZone)));
}

bool ATravelLantern::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn)
	{
		return false;
	}
	InstigatorPawn->UnlockTravel(AnchorZone);
	InstigatorPawn->OpenFastTravel(AnchorZone);
	return true;
}
