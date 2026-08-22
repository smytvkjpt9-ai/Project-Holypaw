#pragma once

#include "HolypawTypes.h"

/**
 * Non-UHT catalog API. Keep this out of HolypawTypes.h — UHT 5.8 fails the
 * Holypaw module (and the editor only says the module could not be found)
 * when a generated header also contains a large C++ namespace.
 */
namespace HolypawCatalog
{
	TArray<FFluffyTypeDef> MakeFluffyTypes();

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
	const TArray<FHolypawAbilityDef>& GetAbilities();
	const FHolypawAbilityDef* FindAbility(FName Id);
	const FHolypawAbilityDef* FindAbilityBySlot(int32 Page, int32 Slot);
}
