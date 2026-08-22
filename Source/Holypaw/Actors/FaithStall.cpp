#include "Actors/FaithStall.h"
#include "Character/HolypawCharacter.h"
#include "Look/HolypawLook.h"
#include "Components/AffectionComponent.h"

AFaithStall::AFaithStall()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFaithStall::BeginPlay()
{
	Super::BeginPlay();
	SetSolidColor(HolypawLook::Gold);
}

FText AFaithStall::GetPrompt() const
{
	return FText::FromString(TEXT("E  Walk in · faith stall (buns, ribbons, jars)"));
}

bool AFaithStall::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn || !InstigatorPawn->Affection)
	{
		return false;
	}
	InstigatorPawn->OpenShop();
	return true;
}
