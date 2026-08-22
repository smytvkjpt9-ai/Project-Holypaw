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

UENUM(BlueprintType)
enum class EPartyRole : uint8
{
	Brace UMETA(DisplayName = "Brace"),
	Choir UMETA(DisplayName = "Choir"),
	Ripper UMETA(DisplayName = "Ripper"),
	Scout UMETA(DisplayName = "Scout")
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

	UPROPERTY(BlueprintReadOnly)
	EPartyRole Role = EPartyRole::Choir;

	UPROPERTY(BlueprintReadOnly)
	EFluffyId FluffyId = EFluffyId::Bunny;
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
	FString LineC;

	UPROPERTY()
	int32 FaithNeed = 0;

	UPROPERTY()
	FString Hint;
};

USTRUCT(BlueprintType)
struct HOLYPAW_API FHolypawAbilityDef
{
	GENERATED_BODY()

	UPROPERTY()
	FName Id;

	UPROPERTY()
	FText DisplayName;

	UPROPERTY()
	FText Description;

	UPROPERTY()
	int32 FpCost = 0;

	UPROPERTY()
	int32 Slot = 0;

	UPROPERTY()
	int32 Stitch = 0;

	UPROPERTY()
	int32 Page = 0;
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
