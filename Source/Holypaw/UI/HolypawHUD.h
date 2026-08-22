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

UCLASS()
class HOLYPAW_API AHolypawHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

protected:
	void DrawLabel(float X, float Y, const FString& Text, const FLinearColor& Color, float Scale = 1.2f);

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
};
