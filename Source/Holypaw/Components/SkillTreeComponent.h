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

	const FSkillDef* FindDef(FName Id) const;

private:
	TArray<FSkillDef> Catalog;
};
