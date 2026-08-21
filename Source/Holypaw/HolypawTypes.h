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
	Snow UMETA(DisplayName = "Alabaster Ridge"),
	LanternAngeles UMETA(DisplayName = "Lantern Angeles"),
	Mossgate UMETA(DisplayName = "Mossgate"),
	Quiltland UMETA(DisplayName = "Quiltland"),
	DustMesa UMETA(DisplayName = "Dust Mesa"),
	PalmaDusk UMETA(DisplayName = "Palma Dusk"),
	IvorySpire UMETA(DisplayName = "Ivory Spire"),
	SandHymn UMETA(DisplayName = "Sand Hymn"),
	CapePlush UMETA(DisplayName = "Cape Plush"),
	CherryLoom UMETA(DisplayName = "Cherry Loom"),
	AuroraBorough UMETA(DisplayName = "Aurora Borough"),
	TundraParish UMETA(DisplayName = "Tundra Parish"),
	Desert UMETA(DisplayName = "Button Desert"),
	Jungle UMETA(DisplayName = "Palm Stitch"),
	Ocean UMETA(DisplayName = "Plush Sea"),
	Ice UMETA(DisplayName = "Felt Ice"),
	CarnivalBahia UMETA(DisplayName = "Carnival Bahia"),
	AndesLoom UMETA(DisplayName = "Andes Loom"),
	Clockhaven UMETA(DisplayName = "Clockhaven"),
	VelvetSeine UMETA(DisplayName = "Velvet Seine"),
	MarbleForum UMETA(DisplayName = "Marble Forum"),
	SavannahBell UMETA(DisplayName = "Savannah Bell"),
	SilkDelta UMETA(DisplayName = "Silk Delta"),
	SpiceHarbor UMETA(DisplayName = "Spice Harbor"),
	CoralChoir UMETA(DisplayName = "Coral Choir"),
	FeltIceCamp UMETA(DisplayName = "Felt Ice Camp")
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FHolypawCity
{
	GENERATED_BODY()

	UPROPERTY()
	EHolypawZone Zone = EHolypawZone::RibbonCity;

	UPROPERTY()
	FText DisplayName;

	UPROPERTY()
	FVector2D Pos = FVector2D::ZeroVector;

	UPROPERTY()
	FLinearColor Accent = FLinearColor(0.82f, 0.42f, 0.55f);

	UPROPERTY()
	int32 Cols = 2;

	UPROPERTY()
	int32 Rows = 2;

	UPROPERTY()
	bool bTallSpire = false;

	UPROPERTY()
	float Radius = 3200.f;

	UPROPERTY()
	float FlattenZ = 55.f;

	UPROPERTY()
	FText Continent;

	UPROPERTY()
	FText Flavor;
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

UENUM(BlueprintType)
enum class EHolypawSkillTree : uint8
{
	Hug UMETA(DisplayName = "Hug Tree"),
	Miracle UMETA(DisplayName = "Miracle Tree"),
	Party UMETA(DisplayName = "Party Tree")
};

UENUM(BlueprintType)
enum class EHolypawFaction : uint8
{
	Wild,
	PolyMill
};

UENUM(BlueprintType)
enum class EHolypawMission : uint8
{
	Wake,
	FirstFriend,
	FirstRip,
	FirstMiracle,
	RibbonGates,
	ConvertThree,
	PolyCourt,
	OutlandRoads,
	GlobeTrek,
	FourRites,
	VelvetCrown,
	Unmake,
	BearFaith
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EHolypawFaction Faction = EHolypawFaction::Wild;
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

	UPROPERTY(BlueprintReadOnly)
	EHolypawSkillTree Tree = EHolypawSkillTree::Hug;
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FMissionDef
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EHolypawMission Id = EHolypawMission::Wake;

	UPROPERTY(BlueprintReadOnly)
	FText Title;

	UPROPERTY(BlueprintReadOnly)
	FText Brief;

	UPROPERTY(BlueprintReadOnly)
	FText Hint;
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FHolypawHeartRecord
{
	GENERATED_BODY()

	UPROPERTY()
	EHolypawZone Zone = EHolypawZone::RibbonCity;

	UPROPERTY()
	int32 Hearts = 0;
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FHolypawItemStack
{
	GENERATED_BODY()

	UPROPERTY()
	FName Id;

	UPROPERTY()
	int32 Count = 0;
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FHolypawItemDef
{
	GENERATED_BODY()

	UPROPERTY()
	FName Id;

	UPROPERTY()
	FText DisplayName;

	UPROPERTY()
	FText Description;

	UPROPERTY()
	int32 ShopCostAP = 0;

	UPROPERTY()
	int32 Heal = 0;

	UPROPERTY()
	int32 Faith = 0;
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FHolypawTalkDef
{
	GENERATED_BODY()

	UPROPERTY()
	FString Speaker;

	UPROPERTY()
	FString Line;

	UPROPERTY()
	FString LineB;

	UPROPERTY()
	FString Hint;
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FHolypawQuestDef
{
	GENERATED_BODY()

	UPROPERTY()
	FName Id;

	UPROPERTY()
	FText Title;

	UPROPERTY()
	FText Brief;

	UPROPERTY()
	FString Giver;

	UPROPERTY()
	FString TurnIn;

	UPROPERTY()
	FName NeedItem;

	UPROPERTY()
	int32 NeedCount = 1;

	UPROPERTY()
	int32 RewardAP = 12;

	UPROPERTY()
	int32 RewardFP = 6;

	UPROPERTY()
	float RewardMiracle = 0.f;

	UPROPERTY()
	FString OfferLine;

	UPROPERTY()
	FString DoneLine;
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

	TArray<FSkillDef> MakeSkills();
	TArray<FSkillDef> SkillsForTree(EHolypawSkillTree Tree);
	const TCHAR* SkillTreeName(EHolypawSkillTree Tree);
	const TArray<FMissionDef>& GetMissions();
	FMissionDef GetMission(EHolypawMission Id);

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
		case EHolypawZone::LanternAngeles: return TEXT("Lantern Angeles");
		case EHolypawZone::Mossgate: return TEXT("Mossgate");
		case EHolypawZone::Quiltland: return TEXT("Quiltland");
		case EHolypawZone::DustMesa: return TEXT("Dust Mesa");
		case EHolypawZone::PalmaDusk: return TEXT("Palma Dusk");
		case EHolypawZone::IvorySpire: return TEXT("Ivory Spire");
		case EHolypawZone::SandHymn: return TEXT("Sand Hymn");
		case EHolypawZone::CapePlush: return TEXT("Cape Plush");
		case EHolypawZone::CherryLoom: return TEXT("Cherry Loom");
		case EHolypawZone::AuroraBorough: return TEXT("Aurora Borough");
		case EHolypawZone::TundraParish: return TEXT("Tundra Parish");
		case EHolypawZone::Desert: return TEXT("Button Desert");
		case EHolypawZone::Jungle: return TEXT("Palm Stitch");
		case EHolypawZone::Ocean: return TEXT("Plush Sea");
		case EHolypawZone::Ice: return TEXT("Felt Ice");
		case EHolypawZone::CarnivalBahia: return TEXT("Carnival Bahia");
		case EHolypawZone::AndesLoom: return TEXT("Andes Loom");
		case EHolypawZone::Clockhaven: return TEXT("Clockhaven");
		case EHolypawZone::VelvetSeine: return TEXT("Velvet Seine");
		case EHolypawZone::MarbleForum: return TEXT("Marble Forum");
		case EHolypawZone::SavannahBell: return TEXT("Savannah Bell");
		case EHolypawZone::SilkDelta: return TEXT("Silk Delta");
		case EHolypawZone::SpiceHarbor: return TEXT("Spice Harbor");
		case EHolypawZone::CoralChoir: return TEXT("Coral Choir");
		case EHolypawZone::FeltIceCamp: return TEXT("Felt Ice Camp");
		default: return TEXT("The Living World");
		}
	}

	struct FHolypawRoadLink
	{
		EHolypawZone A = EHolypawZone::RibbonCity;
		EHolypawZone B = EHolypawZone::Tidewell;
		int32 Steps = 14;
		int32 Salt = 0;
	};

	struct FHolypawLandmass
	{
		const TCHAR* Name = TEXT("");
		FVector2D Center = FVector2D::ZeroVector;
		float RadiusX = 10000.f;
		float RadiusY = 10000.f;
		float HeightBias = 80.f;
	};

	const TArray<FHolypawCity>& GetCities();
	FHolypawCity GetCity(EHolypawZone Zone);
	FLinearColor ZoneTerrainColor(EHolypawZone Zone);
	const TArray<FHolypawRoadLink>& GetRoads();
	const TArray<FHolypawLandmass>& GetLandmasses();
	float LandHeightBias(const FVector2D& P);
	EHolypawZone ResolveWilderness(const FVector2D& P);
	bool IsCityZone(EHolypawZone Zone);

	const TArray<FVillainDef>& GetVillains();
	FVillainDef GetVillain(EHolypawVillain Id);
	FString SpecialLabel(EVillainSpecial Special);
	FString RankLabel(EVillainRank Rank);

	const TArray<FHolypawItemDef>& GetItems();
	const FHolypawItemDef* FindItem(FName Id);
	const FHolypawTalkDef* FindTalk(const FString& Speaker);
	const TArray<FHolypawQuestDef>& GetQuests();
	const FHolypawQuestDef* FindQuest(FName Id);
	const FHolypawQuestDef* FindQuestByGiver(const FString& Speaker);
}
