#pragma once

#include "CoreMinimal.h"
#include "HolypawTypes.generated.h"

UENUM(BlueprintType)
enum class EHolypawZone : uint8
{
	ForestCottage UMETA(DisplayName = "Stuffed Park"),
	NurseryHills UMETA(DisplayName = "Nursery Hills"),
	RibbonCity UMETA(DisplayName = "Ribbon City"),
	Homestead UMETA(DisplayName = "Cozy Homestead"),
	Coast UMETA(DisplayName = "Ribbon Coast"),
	Mire UMETA(DisplayName = "Crimson Hollow"),
	Highlands UMETA(DisplayName = "Velvet Peak"),
	Snow UMETA(DisplayName = "Alabaster Ridge")
};

UENUM(BlueprintType)
enum class EFluffyId : uint8
{
	Bunny,
	Pup,
	Duck,
	Panda,
	Fox,
	Dragon
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FFluffyTypeDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EFluffyId Id = EFluffyId::Bunny;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor Color = FLinearColor(1.f, 0.8f, 0.9f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Attack = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Rarity = TEXT("common");
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FPartyMember
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly)
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(BlueprintReadOnly)
	int32 Attack = 5;

	UPROPERTY(BlueprintReadOnly)
	FString Rarity = TEXT("common");
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FSkillDef
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName Id;

	UPROPERTY(BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly)
	FText Description;

	UPROPERTY(BlueprintReadOnly)
	int32 Cost = 15;

	UPROPERTY(BlueprintReadOnly)
	FName Prerequisite;
};

namespace HolypawCatalog
{
	inline TArray<FFluffyTypeDef> MakeFluffyTypes()
	{
		TArray<FFluffyTypeDef> Out;
		auto Add = [&](EFluffyId Id, const TCHAR* Name, FLinearColor Color, int32 Atk, const TCHAR* Rarity)
		{
			FFluffyTypeDef D;
			D.Id = Id;
			D.DisplayName = FText::FromString(Name);
			D.Color = Color;
			D.Attack = Atk;
			D.Rarity = Rarity;
			Out.Add(D);
		};
		Add(EFluffyId::Bunny, TEXT("Cotton Bunny"), FLinearColor(0.99f, 0.85f, 0.93f), 5, TEXT("common"));
		Add(EFluffyId::Pup, TEXT("Plush Pup"), FLinearColor(0.99f, 0.78f, 0.55f), 7, TEXT("common"));
		Add(EFluffyId::Duck, TEXT("Bath Duck"), FLinearColor(0.99f, 0.92f, 0.45f), 4, TEXT("common"));
		Add(EFluffyId::Panda, TEXT("Ink Panda"), FLinearColor(0.88f, 0.89f, 0.91f), 9, TEXT("rare"));
		Add(EFluffyId::Fox, TEXT("Ember Fox"), FLinearColor(0.98f, 0.68f, 0.38f), 10, TEXT("rare"));
		Add(EFluffyId::Dragon, TEXT("Velvet Dragon"), FLinearColor(0.72f, 0.65f, 0.95f), 14, TEXT("legend"));
		return Out;
	}

	inline TArray<FSkillDef> MakeSkills()
	{
		TArray<FSkillDef> Out;
		auto Add = [&](const TCHAR* Id, const TCHAR* Name, int32 Cost, const TCHAR* Desc, const TCHAR* Req)
		{
			FSkillDef S;
			S.Id = Id;
			S.DisplayName = FText::FromString(Name);
			S.Description = FText::FromString(Desc);
			S.Cost = Cost;
			S.Prerequisite = Req;
			Out.Add(S);
		};
		Add(TEXT("softFur"), TEXT("Soft Fur"), 15, TEXT("+3 Attack. Your hugs feel irresistible."), TEXT(""));
		Add(TEXT("buttonEyes"), TEXT("Button Eyes"), 25, TEXT("+20% Affection from hugs."), TEXT("softFur"));
		Add(TEXT("haloStep"), TEXT("Halo Step"), 40, TEXT("+1.2 move speed and a slight glow."), TEXT("softFur"));
		Add(TEXT("partyBond"), TEXT("Party Bond"), 35, TEXT("Party Assault deals +40% damage."), TEXT("buttonEyes"));
		Add(TEXT("miracleEcho"), TEXT("Miracle Echo"), 50, TEXT("Miracles grant +50% Faith."), TEXT("haloStep"));
		Add(TEXT("fluffShield"), TEXT("Fluff Shield"), 45, TEXT("+20 Max HP and start battles shielded."), TEXT("partyBond"));
		return Out;
	}

	inline const TCHAR* ZoneDisplayName(EHolypawZone Zone)
	{
		switch (Zone)
		{
		case EHolypawZone::ForestCottage: return TEXT("Stuffed Park");
		case EHolypawZone::NurseryHills: return TEXT("Nursery Hills");
		case EHolypawZone::RibbonCity: return TEXT("Ribbon City");
		case EHolypawZone::Homestead: return TEXT("Cozy Homestead");
		case EHolypawZone::Coast: return TEXT("Ribbon Coast");
		case EHolypawZone::Mire: return TEXT("Crimson Hollow");
		case EHolypawZone::Highlands: return TEXT("Velvet Peak");
		case EHolypawZone::Snow: return TEXT("Alabaster Ridge");
		default: return TEXT("The Living World");
		}
	}
}
