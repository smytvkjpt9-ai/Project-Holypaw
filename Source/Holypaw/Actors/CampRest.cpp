#include "Actors/CampRest.h"
#include "Character/HolypawCharacter.h"

ACampRest::ACampRest()
{
	PrimaryActorTick.bCanEverTick = false;
	CampName = NSLOCTEXT("Holypaw", "Camp", "Survey Camp");
}

void ACampRest::BeginPlay()
{
	Super::BeginPlay();
	SetSolidColor(FLinearColor(0.55f, 0.38f, 0.28f));
}

FText ACampRest::GetPrompt() const
{
	return FText::Format(NSLOCTEXT("Holypaw", "CampRest", "E  Rest at {0} (heal)"), CampName);
}

bool ACampRest::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn)
	{
		return false;
	}
	InstigatorPawn->RestFully();
	return true;
}
