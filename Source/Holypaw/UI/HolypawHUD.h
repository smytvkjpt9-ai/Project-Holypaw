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
	void DrawLabel(float X, float Y, const FString& Text, const FLinearColor& Color, float Scale) const;
	void DrawTile(float X, float Y, float W, float H, const FLinearColor& Color, float Alpha = 1.f) const;
	void DrawTitleCanvas(const class AHolypawCharacter* Pawn) const;
	void DrawPlayCanvas(const class AHolypawCharacter* Pawn) const;
	void DrawBattleCanvas(const class AHolypawCharacter* Pawn) const;
	void SyncViewportWidgets(const class AHolypawCharacter* Pawn) const;

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
