#include "Faith/HolypawFaithSim.h"
#include "HolypawCatalog.h"
#include "Actors/HugHuman.h"
#include "Character/HolypawCharacter.h"
#include "HolypawWorldBuilder.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

namespace HolypawFaith
{
	EHolypawFaithStage StageForHearts(const int32 Hearts)
	{
		if (Hearts >= ChoirCityNeed)
		{
			return EHolypawFaithStage::ChoirCity;
		}
		if (Hearts >= BannerFallNeed)
		{
			return EHolypawFaithStage::CoupVisible;
		}
		if (Hearts >= 2)
		{
			return EHolypawFaithStage::MarketThaw;
		}
		if (Hearts >= ShopOpenNeed)
		{
			return EHolypawFaithStage::FirstClap;
		}
		return EHolypawFaithStage::MillHeld;
	}

	int32 HeartsForStage(const EHolypawFaithStage Stage)
	{
		switch (Stage)
		{
		case EHolypawFaithStage::ChoirCity: return ChoirCityNeed;
		case EHolypawFaithStage::CoupVisible: return BannerFallNeed;
		case EHolypawFaithStage::MarketThaw: return 2;
		case EHolypawFaithStage::FirstClap: return ShopOpenNeed;
		default: return 0;
		}
	}

	bool ShopsOpen(const int32 Hearts)
	{
		return Hearts >= ShopOpenNeed;
	}

	bool MillBannersSag(const int32 Hearts)
	{
		return Hearts >= BannerSagNeed && Hearts < BannerFallNeed;
	}

	bool MillBannersDown(const int32 Hearts)
	{
		return Hearts >= BannerFallNeed;
	}

	bool DuskHymnUnlocked(const int32 Hearts)
	{
		return Hearts >= DuskHymnNeed;
	}

	bool BelieversParade(const int32 Hearts)
	{
		return Hearts >= ParadeNeed;
	}

	bool ChoirOwnsDusk(const int32 Hearts)
	{
		return Hearts >= DuskHymnNeed;
	}

	const TCHAR* StageId(const EHolypawFaithStage Stage)
	{
		switch (Stage)
		{
		case EHolypawFaithStage::FirstClap: return TEXT("firstClap");
		case EHolypawFaithStage::MarketThaw: return TEXT("marketThaw");
		case EHolypawFaithStage::CoupVisible: return TEXT("coupVisible");
		case EHolypawFaithStage::ChoirCity: return TEXT("choirCity");
		default: return TEXT("millHeld");
		}
	}

	FString StageLabel(const EHolypawFaithStage Stage)
	{
		switch (Stage)
		{
		case EHolypawFaithStage::FirstClap: return TEXT("shops open");
		case EHolypawFaithStage::MarketThaw: return TEXT("banners sag");
		case EHolypawFaithStage::CoupVisible: return TEXT("banners down · dusk hymn");
		case EHolypawFaithStage::ChoirCity: return TEXT("choir city");
		default: return TEXT("mill-held");
		}
	}

	FString StageToast(const EHolypawFaithStage Stage, const EHolypawZone Zone)
	{
		const bool bRibbon = Zone == EHolypawZone::RibbonCity;
		switch (Stage)
		{
		case EHolypawFaithStage::FirstClap:
			return bRibbon
				? TEXT("Ribbon heard that clap. Market shutters rattle up. Buy a bun.")
				: TEXT("A stall unlatches. Converted people clap like they invented hands.");
		case EHolypawFaithStage::MarketThaw:
			return bRibbon
				? TEXT("Second Heart. Stalls lean open. Mill banners sag like embarrassed polyester.")
				: TEXT("The block thaws. Banners lose their factory posture.");
		case EHolypawFaithStage::CoupVisible:
			return bRibbon
				? TEXT("Three Hearts. Mill banners come down. Dusk will hymn if you stay.")
				: TEXT("The mill ads fall. At dusk the chapel will actually sing.");
		case EHolypawFaithStage::ChoirCity:
			return bRibbon
				? TEXT("Ribbon is a choir now. Polyester looks personally offended.")
				: TEXT("This city claps on the hour. The mill can keep the coupons.");
		default:
			return TEXT("The mill still owns the hour. Hug until a Heart sticks.");
		}
	}

	FString HudLine(const EHolypawZone Zone, const int32 Hearts)
	{
		const EHolypawFaithStage Stage = StageForHearts(Hearts);
		if (!HolypawCatalog::IsCityZone(Zone))
		{
			return Hearts > 0
				? FString::Printf(TEXT("Hearts %d  ·  carry them to a city"), Hearts)
				: TEXT("Wild hearts wait. Cities change.");
		}
		return FString::Printf(TEXT("%s  ·  %s  ·  %d"),
			HolypawCatalog::ZoneDisplayName(Zone),
			*StageLabel(Stage),
			Hearts);
	}

	FString ShopClosedLine()
	{
		return TEXT("Shutters down. Convert a local — Hearts open shops.");
	}

	FString DuskLine(const int32 Hearts, const bool bInCity)
	{
		if (!bInCity)
		{
			return TEXT("Dusk on the road. Cities hymn; the park just gets gold.");
		}
		if (ChoirOwnsDusk(Hearts))
		{
			return TEXT("Dusk hymn. Believers clap toward the chapel. Mill banners hang their heads.");
		}
		if (Hearts > 0)
		{
			return TEXT("Choir hour. A few clap-walk. Convert more and dusk will actually sing.");
		}
		return TEXT("Dusk, but the mill still owns the hour. Convert someone.");
	}

	FString BannerStateLine(const int32 Hearts)
	{
		if (MillBannersDown(Hearts))
		{
			return TEXT("Mill banners down. Handmade ribbons on the poles.");
		}
		if (MillBannersSag(Hearts))
		{
			return TEXT("Mill banners sag. One more Heart and they come down.");
		}
		return TEXT("Mill banners up. Identical smiles, beige theology.");
	}

	FText LivingSign(const FName Id, const int32 Hearts)
	{
		const EHolypawFaithStage Stage = StageForHearts(Hearts);
		if (Id == TEXT("ribbonPlaza"))
		{
			if (Stage >= EHolypawFaithStage::CoupVisible)
			{
				return NSLOCTEXT("Holypaw", "PlazaCoup", "Ribbon Plaza  |  mill banners down. dusk hymns. clap at the fountain");
			}
			if (Stage >= EHolypawFaithStage::FirstClap)
			{
				return NSLOCTEXT("Holypaw", "PlazaOpen", "Ribbon Plaza  |  fountain stall unlatched. believers clap-walk");
			}
			return NSLOCTEXT("Holypaw", "PlazaHeld", "Ribbon Plaza  |  mill ads still up. hug someone — Hearts change this");
		}
		if (Id == TEXT("ribbonMarket"))
		{
			if (ShopsOpen(Hearts))
			{
				return NSLOCTEXT("Holypaw", "MarketOpen", "Market  |  shutters up. Hearts discount. no factory smiles");
			}
			return NSLOCTEXT("Holypaw", "MarketShut", "Market  |  shutters down until a Heart sticks");
		}
		if (Id == TEXT("millProtest"))
		{
			if (MillBannersDown(Hearts))
			{
				return NSLOCTEXT("Holypaw", "MillFell", "Handmade not polyester  |  the ads fell. clap, don't stamp");
			}
			return NSLOCTEXT("Holypaw", "MillUp", "POLY ads  |  beige theology, identical smiles. three Hearts drop them");
		}
		return FText::GetEmpty();
	}

	float MillSmogScale(const int32 RibbonHearts)
	{
		if (RibbonHearts >= ChoirCityNeed)
		{
			return 0.22f;
		}
		if (RibbonHearts >= BannerFallNeed)
		{
			return 0.4f;
		}
		if (RibbonHearts >= ShopOpenNeed)
		{
			return 0.72f;
		}
		return 1.f;
	}

	float BannerPitchDegrees(const int32 Hearts)
	{
		if (MillBannersDown(Hearts))
		{
			return 82.f;
		}
		if (MillBannersSag(Hearts))
		{
			return 28.f;
		}
		return 0.f;
	}

	float ShutterOpenAlpha(const int32 Hearts)
	{
		return ShopsOpen(Hearts) ? 1.f : 0.f;
	}

	float ClapRate(const int32 Hearts, const bool bDusk)
	{
		float Rate = 8.f;
		if (BelieversParade(Hearts))
		{
			Rate = 11.f;
		}
		if (bDusk && ChoirOwnsDusk(Hearts))
		{
			Rate = 14.f;
		}
		return Rate;
	}

	float ParadeOrbit(const int32 Hearts, const bool bBeliever)
	{
		if (!bBeliever)
		{
			return 90.f;
		}
		if (Hearts >= ChoirCityNeed)
		{
			return 620.f;
		}
		if (BelieversParade(Hearts))
		{
			return 420.f;
		}
		return 160.f;
	}

	AHolypawCharacter* FindTeddy(const UObject* WorldContext)
	{
		if (!WorldContext)
		{
			return nullptr;
		}
		return Cast<AHolypawCharacter>(UGameplayStatics::GetPlayerPawn(WorldContext, 0));
	}

	int32 HeartsAt(const UObject* WorldContext, const EHolypawZone Zone)
	{
		if (const AHolypawCharacter* Teddy = FindTeddy(WorldContext))
		{
			return Teddy->GetCityHearts(Zone);
		}
		return 0;
	}

	EHolypawZone ZoneAt(const UObject* WorldContext, const FVector& WorldPos)
	{
		if (!WorldContext || !WorldContext->GetWorld())
		{
			return EHolypawZone::ForestCottage;
		}
		for (TActorIterator<AHolypawWorldBuilder> It(WorldContext->GetWorld()); It; ++It)
		{
			return It->ResolveZone(WorldPos);
		}
		return EHolypawZone::ForestCottage;
	}

	int32 HeartsHere(const UObject* WorldContext, const FVector& WorldPos)
	{
		return HeartsAt(WorldContext, ZoneAt(WorldContext, WorldPos));
	}

	EHolypawZone CreditZone(const AHugHuman* Human, const EHolypawZone Fallback)
	{
		if (Human && Human->bHomeZoneReady)
		{
			return Human->HomeZone;
		}
		return Fallback;
	}
}
