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
	return FText::Format(NSLOCTEXT("Holypaw", "Stall", "E  Faith stall  ({0} AP -> {1} FP)"),
		FText::AsNumber(ApCost), FText::AsNumber(FpGain));
}

bool AFaithStall::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn || !InstigatorPawn->Affection)
	{
		return false;
	}
	return InstigatorPawn->BuyFaith(ApCost, FpGain);
}
