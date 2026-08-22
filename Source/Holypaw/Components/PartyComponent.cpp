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

EPartyRole UPartyComponent::RoleFor(const EFluffyId Id)
{
	switch (Id)
	{
	case EFluffyId::Pup:
	case EFluffyId::Panda:
		return EPartyRole::Brace;
	case EFluffyId::Fox:
	case EFluffyId::Dragon:
		return EPartyRole::Ripper;
	case EFluffyId::Duck:
		return EPartyRole::Scout;
	default:
		return EPartyRole::Choir;
	}
}

const TCHAR* UPartyComponent::RoleLabel(const EPartyRole Role)
{
	switch (Role)
	{
	case EPartyRole::Brace:
		return TEXT("Brace");
	case EPartyRole::Ripper:
		return TEXT("Ripper");
	case EPartyRole::Scout:
		return TEXT("Scout");
	default:
		return TEXT("Choir");
	}
}

int32 UPartyComponent::CountRole(const EPartyRole Role) const
{
	int32 N = 0;
	for (const FPartyMember& M : Members)
	{
		if (M.Role == Role)
		{
			++N;
		}
	}
	return N;
}

int32 UPartyComponent::AssaultBonus(const bool bVsMill) const
{
	int32 Bonus = CountRole(EPartyRole::Ripper) * (bVsMill ? 5 : 2);
	Bonus += CountRole(EPartyRole::Choir) * 2;
	Bonus += CountRole(EPartyRole::Scout);
	return Bonus;
}

FString UPartyComponent::DescribeRoles() const
{
	if (Members.Num() == 0)
	{
		return TEXT("No fluffies.");
	}
	return FString::Printf(TEXT("Choir %d  Brace %d  Ripper %d  Scout %d"),
		CountRole(EPartyRole::Choir),
		CountRole(EPartyRole::Brace),
		CountRole(EPartyRole::Ripper),
		CountRole(EPartyRole::Scout));
}
