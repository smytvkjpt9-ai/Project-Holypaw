#pragma once

#include "CoreMinimal.h"
#include "HolypawTypes.h"

/**
 * Hearts are not a score. They are the city's conversion stage.
 * Mill-held → shops open → mill banners fall → dusk hymn.
 */
enum class EHolypawFaithStage : uint8
{
	MillHeld,
	FirstClap,
	MarketThaw,
	CoupVisible,
	ChoirCity
};

class AHolypawCharacter;

namespace HolypawFaith
{
	/** First Heart unlatches stalls. */
	inline constexpr int32 ShopOpenNeed = 1;
	/** Mill propaganda sags, then falls at three. */
	inline constexpr int32 BannerSagNeed = 1;
	inline constexpr int32 BannerFallNeed = 3;
	/** Dusk actually hymns after three converts (mission 6). */
	inline constexpr int32 DuskHymnNeed = 3;
	/** Converted people leave their stoop and clap-walk. */
	inline constexpr int32 ParadeNeed = 1;
	inline constexpr int32 ChoirCityNeed = 5;

	EHolypawFaithStage StageForHearts(int32 Hearts);
	int32 HeartsForStage(EHolypawFaithStage Stage);
	bool ShopsOpen(int32 Hearts);
	bool MillBannersSag(int32 Hearts);
	bool MillBannersDown(int32 Hearts);
	bool DuskHymnUnlocked(int32 Hearts);
	bool BelieversParade(int32 Hearts);
	bool ChoirOwnsDusk(int32 Hearts);

	const TCHAR* StageId(EHolypawFaithStage Stage);
	FString StageLabel(EHolypawFaithStage Stage);
	FString StageToast(EHolypawFaithStage Stage, EHolypawZone Zone);
	FString HudLine(EHolypawZone Zone, int32 Hearts);
	FString ShopClosedLine();
	FString DuskLine(int32 Hearts, bool bInCity);
	FString BannerStateLine(int32 Hearts);
	FText LivingSign(FName Id, int32 Hearts);

	float MillSmogScale(int32 RibbonHearts);
	float BannerPitchDegrees(int32 Hearts);
	float ShutterOpenAlpha(int32 Hearts);
	float ClapRate(int32 Hearts, bool bDusk);
	float ParadeOrbit(int32 Hearts, bool bBeliever);

	AHolypawCharacter* FindTeddy(const UObject* WorldContext);
	int32 HeartsAt(const UObject* WorldContext, EHolypawZone Zone);
	int32 HeartsHere(const UObject* WorldContext, const FVector& WorldPos);
	EHolypawZone ZoneAt(const UObject* WorldContext, const FVector& WorldPos);
}
