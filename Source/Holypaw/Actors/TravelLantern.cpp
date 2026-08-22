#include "Actors/TravelLantern.h"
#include "Character/HolypawCharacter.h"
#include "HolypawTypes.h"

ATravelLantern::ATravelLantern()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATravelLantern::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(0.35f, 0.35f, 1.4f));
	SetSolidColor(FLinearColor(1.f, 0.82f, 0.35f));
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
