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
#include "UI/HolypawUiCopy.h"
#include "Character/HolypawCharacter.h"
#include "HolypawGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

namespace
{
	void AddHolypawViewportWidget(UUserWidget* Widget, APlayerController* PC, const int32 ZOrder)
	{
		if (!Widget || !PC)
		{
			return;
		}
		Widget->AddToViewport(ZOrder);
		Widget->SetAnchorsInViewport(FAnchors(0.f, 0.f, 1.f, 1.f));
		Widget->SetAlignmentInViewport(FVector2D(0.f, 0.f));
		Widget->SetOffsets(FMargin(0.f));
		Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		int32 ViewX = 0;
		int32 ViewY = 0;
		PC->GetViewportSize(ViewX, ViewY);
		if (ViewX > 0 && ViewY > 0)
		{
			Widget->SetDesiredSizeInViewport(FVector2D(static_cast<float>(ViewX), static_cast<float>(ViewY)));
		}
	}

	bool TitleWidgetFillsViewport(const UHolypawTitleWidget* Title)
	{
		if (!Title)
		{
			return false;
		}
		const FVector2D Size = Title->GetCachedGeometry().GetLocalSize();
		return Size.X >= 8.f && Size.Y >= 8.f;
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
	AddHolypawViewportWidget(PlayHudWidget, PC, 10);

	OverlayWidget = CreateWidget<UHolypawBattleWidget>(PC);
	AddHolypawViewportWidget(OverlayWidget, PC, 20);

	MapWidget = CreateWidget<UHolypawMapWidget>(PC);
	AddHolypawViewportWidget(MapWidget, PC, 30);

	JournalWidget = CreateWidget<UHolypawJournalWidget>(PC);
	AddHolypawViewportWidget(JournalWidget, PC, 31);

	TalkWidget = CreateWidget<UHolypawTalkWidget>(PC);
	AddHolypawViewportWidget(TalkWidget, PC, 32);

	ShopWidget = CreateWidget<UHolypawShopWidget>(PC);
	AddHolypawViewportWidget(ShopWidget, PC, 33);

	CodexWidget = CreateWidget<UHolypawCodexWidget>(PC);
	AddHolypawViewportWidget(CodexWidget, PC, 34);

	TitleWidget = CreateWidget<UHolypawTitleWidget>(PC);
	AddHolypawViewportWidget(TitleWidget, PC, 40);

	PauseWidget = CreateWidget<UHolypawPauseWidget>(PC);
	AddHolypawViewportWidget(PauseWidget, PC, 41);
}

void AHolypawHUD::DrawLabel(float X, float Y, const FString& Text, const FLinearColor& Color, float Scale) const
{
	if (!Canvas)
	{
		return;
	}
	UFont* Font = GEngine ? GEngine->GetLargeFont() : nullptr;
	FCanvasTextItem Item(FVector2D(X, Y), FText::FromString(Text), Font, Color);
	Item.Scale = FVector2D(Scale, Scale);
	Item.EnableShadow(FLinearColor(0.f, 0.f, 0.f, 0.65f));
	Canvas->DrawItem(Item);
}

void AHolypawHUD::DrawTitleCanvas(const AHolypawCharacter* Pawn) const
{
	if (!Canvas)
	{
		return;
	}

	const float W = Canvas->SizeX;
	const float H = Canvas->SizeY;
	if (W < 8.f || H < 8.f)
	{
		return;
	}

	FCanvasTileItem Dim(FVector2D::ZeroVector, FVector2D(W, H), FLinearColor(0.04f, 0.02f, 0.06f, 0.92f));
	Dim.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Dim);

	const UHolypawGameInstance* GI = Pawn
		? UHolypawGameInstance::Get(Pawn)
		: (GetOwningPlayerController() ? UHolypawGameInstance::Get(GetOwningPlayerController()) : nullptr);

	DrawLabel(W * 0.5f - 260.f, H * 0.14f, HolypawUiCopy::GameTitle().ToString(), FLinearColor(0.86f, 0.52f, 0.62f), 1.65f);
	DrawLabel(W * 0.5f - 320.f, H * 0.22f, HolypawUiCopy::Tagline().ToString(), FLinearColor(0.76f, 0.86f, 0.96f), 0.95f);

	const int32 TitleCursor = GI ? GI->TitleCursor : 0;
	for (int32 I = 0; I < UHolypawGameInstance::SlotCount; ++I)
	{
		const bool Sel = (I == TitleCursor);
		const float Y = H * 0.34f + I * 72.f;
		const FLinearColor Card = Sel ? FLinearColor(0.42f, 0.22f, 0.36f, 0.96f) : FLinearColor(0.16f, 0.09f, 0.18f, 0.82f);
		FCanvasTileItem CardBg(FVector2D(W * 0.5f - 300.f, Y), FVector2D(600.f, 58.f), Card);
		CardBg.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(CardBg);
		const FString SlotLine = FString::Printf(TEXT("%s   %s"),
			*HolypawUiCopy::SlotN(I + 1).ToString(),
			GI ? *GI->SlotSummary(I) : *HolypawUiCopy::EmptyPorch().ToString());
		DrawLabel(W * 0.5f - 280.f, Y + 16.f, SlotLine, Sel ? FLinearColor(0.98f, 0.94f, 0.90f) : FLinearColor(0.72f, 0.66f, 0.78f), 1.05f);
	}

	DrawLabel(W * 0.5f - 280.f, H - 96.f, HolypawUiCopy::SlotHint().ToString(), FLinearColor(0.95f, 0.78f, 0.40f), 0.95f);
	if (!Pawn)
	{
		DrawLabel(W * 0.5f - 120.f, H * 0.52f, TEXT("Stitching the porch…"), FLinearColor(0.72f, 0.66f, 0.78f), 1.0f);
	}
}

void AHolypawHUD::DrawHUD()
{
	Super::DrawHUD();

	AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(GetOwningPawn());
	const bool bTitle = !Pawn || Pawn->Mode == EHolypawPawnMode::Title;
	if (bTitle && !TitleWidgetFillsViewport(TitleWidget))
	{
		DrawTitleCanvas(Pawn);
	}
}
