#include "Components/AffectionComponent.h"

UAffectionComponent::UAffectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAffectionComponent::Broadcast() const
{
	OnStatsChanged.Broadcast();
}

void UAffectionComponent::AddAP(int32 Amount, bool bFillMiracle)
{
	AP += FMath::Max(0, Amount);
	if (bFillMiracle)
	{
		AddMiracle(Amount * 0.85f);
	}
	Broadcast();
}

void UAffectionComponent::AddFP(int32 Amount)
{
	FP += FMath::Max(0, Amount);
	Broadcast();
}

bool UAffectionComponent::SpendAP(int32 Amount)
{
	if (AP < Amount)
	{
		return false;
	}
	AP -= Amount;
	Broadcast();
	return true;
}

bool UAffectionComponent::SpendFP(int32 Amount)
{
	if (FP < Amount)
	{
		return false;
	}
	FP -= Amount;
	Broadcast();
	return true;
}

void UAffectionComponent::ResetMiracleCharge()
{
	MiracleCharge = 0.f;
	Broadcast();
}

void UAffectionComponent::AddMiracle(float Amount)
{
	MiracleCharge = FMath::Clamp(MiracleCharge + Amount, 0.f, MiracleMax);
	Broadcast();
}
