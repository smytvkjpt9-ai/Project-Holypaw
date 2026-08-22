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
		Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	bool TitleScreenUsesCanvas(const APlayerController* PC)
	{
		if (!PC)
		{
			return true;
		}
		int32 ViewX = 0;
		int32 ViewY = 0;
		PC->GetViewportSize(ViewX, ViewY);
		return ViewX < 640 || ViewY < 480;
	}
}

AHolypawHUD::AHolypawHUD()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AHolypawHUD::StretchViewportWidget(UUserWidget* Widget, const FVector2D& ViewportSize) const
{
	if (!Widget || !Widget->IsInViewport() || ViewportSize.X < 8.f || ViewportSize.Y < 8.f)
	{
		return;
	}
	Widget->SetAnchorsInViewport(FAnchors(0.f, 0.f, 1.f, 1.f));
	Widget->SetAlignmentInViewport(FVector2D(0.f, 0.f));
	Widget->SetPositionInViewport(FVector2D::ZeroVector, false);
	Widget->SetDesiredSizeInViewport(ViewportSize);
}

void AHolypawHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return;
	}
	int32 ViewX = 0;
	int32 ViewY = 0;
	PC->GetViewportSize(ViewX, ViewY);
	const FVector2D ViewportSize(static_cast<float>(ViewX), static_cast<float>(ViewY));
	StretchViewportWidget(PlayHudWidget, ViewportSize);
	StretchViewportWidget(OverlayWidget, ViewportSize);
	StretchViewportWidget(MapWidget, ViewportSize);
	StretchViewportWidget(JournalWidget, ViewportSize);
	StretchViewportWidget(TalkWidget, ViewportSize);
	StretchViewportWidget(ShopWidget, ViewportSize);
	StretchViewportWidget(CodexWidget, ViewportSize);
	StretchViewportWidget(TitleWidget, ViewportSize);
	StretchViewportWidget(PauseWidget, ViewportSize);
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

void AHolypawHUD::DrawTile(float X, float Y, float W, float H, const FLinearColor& Color, float Alpha) const
{
	if (!Canvas || W < 1.f || H < 1.f)
	{
		return;
	}
	FLinearColor Tint = Color;
	Tint.A = Alpha;
	FCanvasTileItem Tile(FVector2D(X, Y), FVector2D(W, H), Tint);
	Tile.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Tile);
}

void AHolypawHUD::SyncViewportWidgets(const AHolypawCharacter* Pawn) const
{
	if (!Pawn)
	{
		return;
	}
	const bool bTitle = Pawn->Mode == EHolypawPawnMode::Title;
	const bool bPause = Pawn->Mode == EHolypawPawnMode::Pause;
	const bool bBattle = Pawn->Mode == EHolypawPawnMode::Battle;
	const bool bBusyUi = Pawn->IsSkillsOpen() || Pawn->IsPartyOpen() || Pawn->IsInventoryOpen()
		|| Pawn->IsMapOpen() || Pawn->IsJournalOpen() || Pawn->IsTalkOpen() || Pawn->IsShopOpen()
		|| Pawn->IsCodexOpen();
	const bool bOverlayPanel = Pawn->IsSkillsOpen() || Pawn->IsPartyOpen() || Pawn->IsInventoryOpen();

	if (PlayHudWidget)
	{
		const bool bShow = !bTitle && !bPause && !bBattle && !bBusyUi;
		PlayHudWidget->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (OverlayWidget)
	{
		const bool bShow = bBattle || bOverlayPanel;
		OverlayWidget->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (MapWidget)
	{
		MapWidget->SetVisibility(Pawn->IsMapOpen() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (JournalWidget)
	{
		JournalWidget->SetVisibility(Pawn->IsJournalOpen() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (TalkWidget)
	{
		TalkWidget->SetVisibility(Pawn->IsTalkOpen() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (ShopWidget)
	{
		ShopWidget->SetVisibility(Pawn->IsShopOpen() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (CodexWidget)
	{
		CodexWidget->SetVisibility(Pawn->IsCodexOpen() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (TitleWidget)
	{
		TitleWidget->SetVisibility(bTitle ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (PauseWidget)
	{
		PauseWidget->SetVisibility(bPause ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
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
	SyncViewportWidgets(Pawn);

	APlayerController* PC = GetOwningPlayerController();
	const bool bTitle = !Pawn || Pawn->Mode == EHolypawPawnMode::Title;
	if (bTitle && TitleScreenUsesCanvas(PC))
	{
		DrawTitleCanvas(Pawn);
	}
}
