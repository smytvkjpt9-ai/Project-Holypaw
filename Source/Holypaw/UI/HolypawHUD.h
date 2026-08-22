#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HolypawHUD.generated.h"

class UHolypawBattleWidget;
class UHolypawTitleWidget;
class UHolypawPauseWidget;
class UHolypawMapWidget;
class UHolypawJournalWidget;
class UHolypawTalkWidget;
class UHolypawShopWidget;
class UHolypawPlayHudWidget;
class UHolypawCodexWidget;

UCLASS()
class HOLYPAW_API AHolypawHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

protected:
	UPROPERTY()
	TObjectPtr<UHolypawPlayHudWidget> PlayHudWidget;

	UPROPERTY()
	TObjectPtr<UHolypawBattleWidget> OverlayWidget;

	UPROPERTY()
	TObjectPtr<UHolypawTitleWidget> TitleWidget;

	UPROPERTY()
	TObjectPtr<UHolypawPauseWidget> PauseWidget;

	UPROPERTY()
	TObjectPtr<UHolypawMapWidget> MapWidget;

	UPROPERTY()
	TObjectPtr<UHolypawJournalWidget> JournalWidget;

	UPROPERTY()
	TObjectPtr<UHolypawTalkWidget> TalkWidget;

	UPROPERTY()
	TObjectPtr<UHolypawShopWidget> ShopWidget;

	UPROPERTY()
	TObjectPtr<UHolypawCodexWidget> CodexWidget;
};
