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
	AHolypawHUD();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void DrawHUD() override;

protected:
	void DrawLabel(float X, float Y, const FString& Text, const FLinearColor& Color, float Scale) const;
	void DrawTile(float X, float Y, float W, float H, const FLinearColor& Color, float Alpha = 1.f) const;
	void DrawTitleCanvas(const class AHolypawCharacter* Pawn) const;
	void SyncViewportWidgets(const class AHolypawCharacter* Pawn) const;
	void StretchViewportWidget(UUserWidget* Widget, const FVector2D& ViewportSize) const;

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
