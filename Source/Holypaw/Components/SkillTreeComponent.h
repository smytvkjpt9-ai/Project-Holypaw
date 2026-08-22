#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HolypawTypes.h"
#include "SkillTreeComponent.generated.h"

UCLASS(ClassGroup = (Holypaw), meta = (BlueprintSpawnableComponent))
class HOLYPAW_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillTreeComponent();

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	TMap<FName, bool> Owned;

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	bool HasSkill(FName Id) const;

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	bool CanBuy(FName Id, int32 CurrentAP) const;

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	bool Buy(FName Id, int32& InOutAP);

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	const TArray<FSkillDef>& GetCatalog() const { return Catalog; }

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	TArray<FSkillDef> GetTreeSkills(EHolypawSkillTree Tree) const;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	EHolypawSkillTree ActiveTree = EHolypawSkillTree::Hug;

	void CycleTree();

	const FSkillDef* FindDef(FName Id) const;

	TArray<FName> GetOwnedIds() const;
	void ReplaceOwned(const TArray<FName>& Ids);

private:
	TArray<FSkillDef> Catalog;
};
