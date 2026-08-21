#include "Actors/FaithStall.h"
#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"

AFaithStall::AFaithStall()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFaithStall::BeginPlay()
{
	Super::BeginPlay();
	SetSolidColor(FLinearColor(0.95f, 0.78f, 0.35f));
}

FText AFaithStall::GetPrompt() const
{
	return FText::FromString(TEXT("E  Faith stall (buns, ribbons, jars)"));
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
