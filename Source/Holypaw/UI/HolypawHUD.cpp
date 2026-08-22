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

namespace
{
	void AddHolypawViewportWidget(UUserWidget* Widget, const int32 ZOrder)
	{
		if (!Widget)
		{
			return;
		}
		Widget->AddToViewport(ZOrder);
		Widget->SetAnchorsInViewport(FAnchors(0.f, 0.f, 1.f, 1.f));
		Widget->SetAlignmentInViewport(FVector2D(0.f, 0.f));
		Widget->SetPositionInViewport(FVector2D::ZeroVector, false);
	}
}

void AHolypawHUD::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return;
	}

	PlayHudWidget = CreateWidget<UHolypawPlayHudWidget>(PC);
	AddHolypawViewportWidget(PlayHudWidget, 10);

	OverlayWidget = CreateWidget<UHolypawBattleWidget>(PC);
	AddHolypawViewportWidget(OverlayWidget, 20);

	MapWidget = CreateWidget<UHolypawMapWidget>(PC);
	AddHolypawViewportWidget(MapWidget, 30);

	JournalWidget = CreateWidget<UHolypawJournalWidget>(PC);
	AddHolypawViewportWidget(JournalWidget, 31);

	TalkWidget = CreateWidget<UHolypawTalkWidget>(PC);
	AddHolypawViewportWidget(TalkWidget, 32);

	ShopWidget = CreateWidget<UHolypawShopWidget>(PC);
	AddHolypawViewportWidget(ShopWidget, 33);

	CodexWidget = CreateWidget<UHolypawCodexWidget>(PC);
	AddHolypawViewportWidget(CodexWidget, 34);

	TitleWidget = CreateWidget<UHolypawTitleWidget>(PC);
	AddHolypawViewportWidget(TitleWidget, 40);

	PauseWidget = CreateWidget<UHolypawPauseWidget>(PC);
	AddHolypawViewportWidget(PauseWidget, 41);
}

void AHolypawHUD::DrawHUD()
{
	Super::DrawHUD();
}
