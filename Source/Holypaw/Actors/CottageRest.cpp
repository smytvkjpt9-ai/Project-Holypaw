#include "Actors/CottageRest.h"
#include "Character/HolypawCharacter.h"

ACottageRest::ACottageRest()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACottageRest::BeginPlay()
{
	Super::BeginPlay();
	SetSolidColor(FLinearColor(0.72f, 0.58f, 0.42f));
}

FText ACottageRest::GetPrompt() const
{
	return NSLOCTEXT("Holypaw", "Rest", "E  Rest (heal). The humans will wait. They always wait.");
}

bool ACottageRest::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn)
	{
		return false;
	}
	InstigatorPawn->RestFully();
	return true;
}
