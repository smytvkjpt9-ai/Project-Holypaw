#include "Components/SkillTreeComponent.h"

USkillTreeComponent::USkillTreeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Catalog = HolypawCatalog::MakeSkills();
	for (const FSkillDef& S : Catalog)
	{
		Owned.Add(S.Id, false);
	}
}

bool USkillTreeComponent::HasSkill(FName Id) const
{
	const bool* Found = Owned.Find(Id);
	return Found && *Found;
}

const FSkillDef* USkillTreeComponent::FindDef(FName Id) const
{
	return Catalog.FindByPredicate([&](const FSkillDef& S) { return S.Id == Id; });
}

bool USkillTreeComponent::CanBuy(FName Id, int32 CurrentAP) const
{
	if (HasSkill(Id))
	{
		return false;
	}
	const FSkillDef* Def = FindDef(Id);
	if (!Def)
	{
		return false;
	}
	if (!Def->Prerequisite.IsNone() && !HasSkill(Def->Prerequisite))
	{
		return false;
	}
	return CurrentAP >= Def->Cost;
}

bool USkillTreeComponent::Buy(FName Id, int32& InOutAP)
{
	if (!CanBuy(Id, InOutAP))
	{
		return false;
	}
	const FSkillDef* Def = FindDef(Id);
	InOutAP -= Def->Cost;
	Owned.Add(Id, true);
	return true;
}
