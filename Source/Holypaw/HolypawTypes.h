#pragma once

#include "CoreMinimal.h"
#include "HolypawTypes.generated.h"

UENUM(BlueprintType)
enum class EHolypawZone : uint8
{
	ForestCottage UMETA(DisplayName = "Stuffed Park"),
	NurseryHills UMETA(DisplayName = "Nursery Hills"),
	RibbonCity UMETA(DisplayName = "Ribbon City"),
	Tidewell UMETA(DisplayName = "Tidewell"),
	Hearthfold UMETA(DisplayName = "Hearthfold"),
	Emberfen UMETA(DisplayName = "Emberfen"),
	Snowveil UMETA(DisplayName = "Snowveil"),
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

UENUM(BlueprintType)
enum class EHolypawVillain : uint8
{
	ScrapDog UMETA(DisplayName = "Scrap Dog"),
	CorpCat UMETA(DisplayName = "Corp Cat"),
	RazorPetbot UMETA(DisplayName = "Razor Petbot"),
	VoidRat UMETA(DisplayName = "Void Rat"),
	NightThread UMETA(DisplayName = "Night Thread"),
	StitchedWolf UMETA(DisplayName = "Stitched Wolf"),
	ParkProwler UMETA(DisplayName = "Park Prowler"),
	Tatterfox UMETA(DisplayName = "Tatterfox"),
	AlleyScrapDog UMETA(DisplayName = "Alley Scrap Dog"),
	RibbonEnforcer UMETA(DisplayName = "Ribbon Enforcer"),
	TinselGolem UMETA(DisplayName = "Tinsel Golem"),
	GoldSnipper UMETA(DisplayName = "Gold Snipper"),
	PlazaCorpCat UMETA(DisplayName = "Plaza Corp Cat"),
	SewerVoidRat UMETA(DisplayName = "Sewer Void Rat"),
	SilkMagistrate UMETA(DisplayName = "Silk Magistrate"),
	SaltCrab UMETA(DisplayName = "Salt Crab"),
	HarborHook UMETA(DisplayName = "Harbor Hook"),
	BrineGull UMETA(DisplayName = "Brine Gull"),
	DockRat UMETA(DisplayName = "Dock Rat"),
	BrineWarden UMETA(DisplayName = "Brine Warden"),
	ScarecrowHound UMETA(DisplayName = "Scarecrow Hound"),
	HaywireScarecrow UMETA(DisplayName = "Haywire Scarecrow"),
	ThreshCat UMETA(DisplayName = "Thresh Cat"),
	GrainMite UMETA(DisplayName = "Grain Mite"),
	HarvestOverseer UMETA(DisplayName = "Harvest Overseer"),
	MireLurker UMETA(DisplayName = "Mire Lurker"),
	FenWitchPet UMETA(DisplayName = "Fen Witch-Pet"),
	BogLeech UMETA(DisplayName = "Bog Leech"),
	EmberToad UMETA(DisplayName = "Ember Toad"),
	BogKing UMETA(DisplayName = "Bog King"),
	FrostMoth UMETA(DisplayName = "Frost Moth"),
	IceShardCat UMETA(DisplayName = "Ice Shard Cat"),
	DriftWolf UMETA(DisplayName = "Drift Wolf"),
	AuroraWisp UMETA(DisplayName = "Aurora Wisp"),
	AuroraWarden UMETA(DisplayName = "Aurora Warden"),
	VelvetTyrant UMETA(DisplayName = "Velvet Tyrant"),
	Unmaker UMETA(DisplayName = "The Unmaker"),
	MiracleEater UMETA(DisplayName = "Miracle Eater"),
	ButtonThief UMETA(DisplayName = "Button Thief"),
	UnstuffedShade UMETA(DisplayName = "Unstuffed Shade")
};

UENUM(BlueprintType)
enum class EVillainRank : uint8
{
	Minion,
	Elite,
	Boss,
	WorldBoss
};

UENUM(BlueprintType)
enum class EVillainShape : uint8
{
	Cube,
	Sphere,
	Cone,
	Cylinder
};

UENUM(BlueprintType)
enum class EVillainSpecial : uint8
{
	Rip,
	DrainFaith,
	StealMiracle,
	DoubleStrike,
	ArmorPlates,
	FrostBite,
	PoisonThread,
	Rage,
	ThreadCut,
	FaithBurn
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
struct HOLYPAW_API FVillainDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EHolypawVillain Id = EHolypawVillain::ScrapDog;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EHolypawZone HomeZone = EHolypawZone::ForestCottage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EVillainRank Rank = EVillainRank::Minion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EVillainShape Shape = EVillainShape::Cube;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EVillainSpecial Special = EVillainSpecial::Rip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor Color = FLinearColor(0.45f, 0.4f, 0.42f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor AccentColor = FLinearColor(0.28f, 0.22f, 0.24f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 HP = 28;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Attack = 7;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Scale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AggroRange = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RespawnSeconds = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bBlocksFlee = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ApReward = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 FpReward = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MiracleReward = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString IntroLine;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString AttackLine;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString DefeatLine;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CodexBlurb;
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
		case EHolypawZone::Tidewell: return TEXT("Tidewell");
		case EHolypawZone::Hearthfold: return TEXT("Hearthfold");
		case EHolypawZone::Emberfen: return TEXT("Emberfen");
		case EHolypawZone::Snowveil: return TEXT("Snowveil");
		case EHolypawZone::Homestead: return TEXT("Cozy Homestead");
		case EHolypawZone::Coast: return TEXT("Ribbon Coast");
		case EHolypawZone::Mire: return TEXT("Crimson Hollow");
		case EHolypawZone::Highlands: return TEXT("Velvet Peak");
		case EHolypawZone::Snow: return TEXT("Alabaster Ridge");
		default: return TEXT("The Living World");
		}
	}

	const TArray<FVillainDef>& GetVillains();
	FVillainDef GetVillain(EHolypawVillain Id);
	FString SpecialLabel(EVillainSpecial Special);
	FString RankLabel(EVillainRank Rank);
}
