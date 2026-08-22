#include "Components/PartyComponent.h"

UPartyComponent::UPartyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UPartyComponent::TryAdd(const FPartyMember& Member)
{
	if (IsFull())
	{
		return false;
	}
	Members.Add(Member);
	return true;
}

int32 UPartyComponent::TotalAttack() const
{
	int32 Sum = 0;
	for (const FPartyMember& M : Members)
	{
		Sum += M.Attack;
	}
	return Sum;
}
