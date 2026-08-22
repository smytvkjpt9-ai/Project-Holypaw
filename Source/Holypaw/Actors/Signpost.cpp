#include "Actors/Signpost.h"
#include "Character/HolypawCharacter.h"
#include "Faith/HolypawFaithSim.h"

ASignpost::ASignpost()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASignpost::BeginPlay()
{
	Super::BeginPlay();
	SetSolidColor(FLinearColor(0.55f, 0.42f, 0.28f));
}

void ASignpost::RefreshFromHearts(const int32 Hearts)
{
	if (LivingId.IsNone())
	{
		return;
	}
	const FText Next = HolypawFaith::LivingSign(LivingId, Hearts);
	if (!Next.IsEmpty())
	{
		Message = Next;
	}
}

FText ASignpost::GetPrompt() const
{
	return Message.IsEmpty() ? NSLOCTEXT("Holypaw", "Sign", "E  Read signpost") : Message;
}

bool ASignpost::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn)
	{
		return false;
	}
	InstigatorPawn->Toast(Message.ToString());
	return true;
}
