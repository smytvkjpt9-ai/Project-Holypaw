#include "Actors/HolypawPickup.h"
#include "Character/HolypawCharacter.h"

AHolypawPickup::AHolypawPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	Label = NSLOCTEXT("Holypaw", "Pickup", "Keepsake");
}

void AHolypawPickup::BeginPlay()
{
	Super::BeginPlay();
	if (ItemId == TEXT("hymnSheet"))
	{
		SetSolidColor(FLinearColor(0.95f, 0.86f, 0.55f));
	}
	else if (ItemId == TEXT("saltPearl"))
	{
		SetSolidColor(FLinearColor(0.72f, 0.9f, 0.95f));
	}
	else
	{
		SetSolidColor(FLinearColor(0.85f, 0.62f, 0.72f));
	}
}

FText AHolypawPickup::GetPrompt() const
{
	return FText::Format(NSLOCTEXT("Holypaw", "PickPrompt", "E  Pocket {0}"), Label);
}

bool AHolypawPickup::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn || ItemId.IsNone())
	{
		return false;
	}
	if (InstigatorPawn->GetItemCount(ItemId) > 0)
	{
		InstigatorPawn->Toast(TEXT("Pockets already hold that. Leave it for the next you."));
		return true;
	}
	InstigatorPawn->AddItem(ItemId, 1);
	InstigatorPawn->Toast(FString::Printf(TEXT("Pocketed %s."), *Label.ToString()));
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	return true;
}
