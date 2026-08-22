#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HolypawTypes.h"
#include "PartyComponent.generated.h"

UCLASS(ClassGroup = (Holypaw), meta = (BlueprintSpawnableComponent))
class HOLYPAW_API UPartyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPartyComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 MaxParty = 4;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	TArray<FPartyMember> Members;

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	bool IsFull() const { return Members.Num() >= MaxParty; }

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	bool TryAdd(const FPartyMember& Member);

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	int32 TotalAttack() const;

	int32 CountRole(EPartyRole Role) const;
	int32 AssaultBonus(bool bVsMill) const;
	FString DescribeRoles() const;
	static EPartyRole RoleFor(EFluffyId Id);
	static const TCHAR* RoleLabel(EPartyRole Role);
};
