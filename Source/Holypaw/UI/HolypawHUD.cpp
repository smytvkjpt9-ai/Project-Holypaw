#include "UI/HolypawHUD.h"
#include "UI/HolypawBattleWidget.h"
#include "UI/HolypawTitleWidget.h"
#include "UI/HolypawPauseWidget.h"
#include "UI/HolypawMapWidget.h"
#include "UI/HolypawJournalWidget.h"
#include "UI/HolypawTalkWidget.h"
#include "UI/HolypawShopWidget.h"
#include "UI/HolypawPlayHudWidget.h"
#include "UI/HolypawCodexWidget.h"
#include "Blueprint/UserWidget.h"

void AHolypawHUD::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return;
	}

	PlayHudWidget = CreateWidget<UHolypawPlayHudWidget>(PC);
	if (PlayHudWidget)
	{
		PlayHudWidget->AddToViewport(10);
	}
	OverlayWidget = CreateWidget<UHolypawBattleWidget>(PC);
	if (OverlayWidget)
	{
		OverlayWidget->AddToViewport(20);
	}
	MapWidget = CreateWidget<UHolypawMapWidget>(PC);
	if (MapWidget)
	{
		MapWidget->AddToViewport(30);
	}
	JournalWidget = CreateWidget<UHolypawJournalWidget>(PC);
	if (JournalWidget)
	{
		JournalWidget->AddToViewport(31);
	}
	TalkWidget = CreateWidget<UHolypawTalkWidget>(PC);
	if (TalkWidget)
	{
		TalkWidget->AddToViewport(32);
	}
	ShopWidget = CreateWidget<UHolypawShopWidget>(PC);
	if (ShopWidget)
	{
		ShopWidget->AddToViewport(33);
	}
	CodexWidget = CreateWidget<UHolypawCodexWidget>(PC);
	if (CodexWidget)
	{
		CodexWidget->AddToViewport(34);
	}
	TitleWidget = CreateWidget<UHolypawTitleWidget>(PC);
	if (TitleWidget)
	{
		TitleWidget->AddToViewport(40);
	}
	PauseWidget = CreateWidget<UHolypawPauseWidget>(PC);
	if (PauseWidget)
	{
		PauseWidget->AddToViewport(41);
	}
}

void AHolypawHUD::DrawHUD()
{
	Super::DrawHUD();
}
