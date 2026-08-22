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
#include "Components/AffectionComponent.h"
#include "Components/MissionComponent.h"
#include "Actors/HostilePet.h"
#include "HolypawCatalog.h"
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
	AddHolypawViewportWidget(PlayHudWidget, 10);
	PlayHudWidget->SetVisibility(ESlateVisibility::Collapsed);

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
	if (PlayHudWidget)
	{
		PlayHudWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (!OverlayWidget || !Pawn)
	{
		return;
	}
	const bool bPanel = Pawn->IsSkillsOpen() || Pawn->IsPartyOpen() || Pawn->IsInventoryOpen();
	const bool bShowOverlay = bPanel;
	OverlayWidget->SetVisibility(bShowOverlay ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void AHolypawHUD::DrawPlayCanvas(const AHolypawCharacter* Pawn) const
{
	if (!Canvas || !Pawn || !Pawn->Affection || Pawn->Mode != EHolypawPawnMode::Play)
	{
		return;
	}

	const float W = Canvas->SizeX;
	const float H = Canvas->SizeY;
	if (W < 8.f || H < 8.f)
	{
		return;
	}

	DrawTile(16.f, 14.f, W - 32.f, 48.f, FLinearColor(0.10f, 0.06f, 0.12f), 0.88f);
	DrawLabel(28.f, 26.f, HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone), FLinearColor(0.96f, 0.92f, 0.88f), 0.95f);
	DrawLabel(260.f, 26.f, FString::Printf(TEXT("HP %s"), *HolypawUiCopy::HpFrac(Pawn->HP, Pawn->HPMax).ToString()),
		FLinearColor(0.72f, 0.92f, 0.78f), 0.9f);
	DrawLabel(380.f, 26.f, FString::Printf(TEXT("AP %d  FP %d"), Pawn->Affection->AP, Pawn->Affection->FP),
		FLinearColor(0.95f, 0.78f, 0.40f), 0.9f);
	const FString Clock = Pawn->GetClockLine();
	if (!Clock.IsEmpty())
	{
		DrawLabel(W - 220.f, 26.f, Clock, FLinearColor(0.95f, 0.78f, 0.40f), 0.85f);
	}
	const FString Faith = Pawn->GetFaithLine();
	if (!Faith.IsEmpty())
	{
		DrawLabel(28.f, 68.f, Faith, FLinearColor(0.86f, 0.52f, 0.62f), 0.78f);
	}

	if (Pawn->GetToastAlpha() > 0.f)
	{
		const float ToastW = FMath::Min(W - 80.f, 760.f);
		DrawTile((W - ToastW) * 0.5f, 96.f, ToastW, 42.f, FLinearColor(0.08f, 0.04f, 0.10f), 0.92f);
		DrawLabel((W - ToastW) * 0.5f + 16.f, 108.f, Pawn->GetToast(), FLinearColor(0.96f, 0.92f, 0.88f), 0.88f);
	}

	if (!Pawn->GetPrompt().IsEmpty())
	{
		const FString Prompt = Pawn->GetPrompt().ToString();
		const float PromptW = FMath::Clamp(40.f + Prompt.Len() * 9.f, 180.f, 520.f);
		const float PX = (W - PromptW) * 0.5f;
		const float PY = H - 118.f;
		DrawTile(PX, PY, PromptW, 40.f, FLinearColor(0.42f, 0.22f, 0.36f), 0.94f);
		DrawLabel(PX + 16.f, PY + 10.f, Prompt, FLinearColor(0.98f, 0.94f, 0.90f), 0.92f);
	}

	DrawTile(16.f, H - 54.f, W - 32.f, 38.f, FLinearColor(0.08f, 0.04f, 0.10f), 0.82f);
	DrawLabel(28.f, H - 42.f,
		TEXT("WASD move  ·  Mouse look  ·  Scroll zoom  ·  E interact  ·  J journal  ·  N map  ·  1-6 battle"),
		FLinearColor(0.72f, 0.66f, 0.78f), 0.78f);
}

void AHolypawHUD::DrawBattleCanvas(const AHolypawCharacter* Pawn) const
{
	if (!Canvas || !Pawn || Pawn->Mode != EHolypawPawnMode::Battle)
	{
		return;
	}

	const float W = Canvas->SizeX;
	const float H = Canvas->SizeY;
	if (W < 8.f || H < 8.f)
	{
		return;
	}

	const float BannerW = FMath::Min(1100.f, W - 40.f);
	const float BannerX = (W - BannerW) * 0.5f;
	const float BannerY = H * 0.07f;
	DrawTile(BannerX, BannerY, BannerW, 118.f, FLinearColor(0.08f, 0.04f, 0.10f), 0.90f);

	DrawLabel(BannerX + 20.f, BannerY + 14.f, HolypawUiCopy::BattleYou().ToString(), FLinearColor(0.86f, 0.52f, 0.62f), 1.0f);
	DrawLabel(BannerX + 20.f, BannerY + 44.f, HolypawUiCopy::HpFrac(Pawn->HP, Pawn->HPMax).ToString(),
		FLinearColor(0.72f, 0.92f, 0.78f), 0.92f);
	DrawLabel(BannerX + BannerW * 0.5f - 16.f, BannerY + 38.f, HolypawUiCopy::BattleVs().ToString(),
		FLinearColor(0.95f, 0.78f, 0.40f), 1.15f);

	const AHostilePet* Enemy = Pawn->GetBattleEnemy();
	const float EX = BannerX + BannerW - 320.f;
	DrawLabel(EX, BannerY + 14.f, Enemy ? Enemy->DisplayName.ToString() : HolypawUiCopy::Hostile().ToString(),
		FLinearColor(0.92f, 0.38f, 0.42f), 1.0f);
	if (Enemy)
	{
		DrawLabel(EX, BannerY + 44.f, HolypawUiCopy::HpFrac(FMath::Max(0, Enemy->HP), Enemy->HPMax).ToString(),
			FLinearColor(0.92f, 0.38f, 0.42f), 0.92f);
		DrawLabel(EX, BannerY + 68.f,
			FString::Printf(TEXT("%s · %s"), *HolypawCatalog::RankLabel(Enemy->Rank), *HolypawCatalog::SpecialLabel(Enemy->Special)),
			FLinearColor(0.96f, 0.92f, 0.88f), 0.75f);
	}

	const float LogY = BannerY + 126.f;
	DrawTile(BannerX, LogY, BannerW, 36.f, FLinearColor(0.16f, 0.09f, 0.18f), 0.92f);
	DrawLabel(BannerX + 14.f, LogY + 8.f, Pawn->GetBattleLog(), FLinearColor(0.96f, 0.92f, 0.88f), 0.82f);
	const FString Status = Pawn->GetBattleStatusLine();
	if (!Status.IsEmpty())
	{
		DrawLabel(BannerX + BannerW * 0.55f, LogY + 8.f, Status, FLinearColor(0.72f, 0.92f, 0.78f), 0.75f);
	}

	const int32 Page = Pawn->GetBattlePage();
	const float TrayW = FMath::Min(1080.f, W - 32.f);
	const float TrayH = 186.f;
	const float TrayX = (W - TrayW) * 0.5f;
	const float TrayY = H - TrayH - 18.f;
	DrawTile(TrayX, TrayY, TrayW, TrayH, FLinearColor(0.10f, 0.06f, 0.12f), 0.94f);

	const TCHAR* PageName = Page == 1 ? TEXT("Overflow") : (Page == 2 ? TEXT("Party tricks") : TEXT("Basics"));
	DrawLabel(TrayX + 16.f, TrayY + 10.f,
		FString::Printf(TEXT("%s   ·   Tab page   ·   Keys 1-6 attack"), PageName),
		FLinearColor(0.95f, 0.78f, 0.40f), 0.82f);

	const float CardW = (TrayW - 32.f) / 6.f;
	for (int32 Slot = 1; Slot <= 6; ++Slot)
	{
		const FHolypawAbilityDef* Ability = HolypawCatalog::FindAbilityBySlot(Page, Slot);
		const float CX = TrayX + 16.f + (Slot - 1) * CardW;
		const float CY = TrayY + 44.f;
		DrawTile(CX, CY, CardW - 8.f, 118.f, FLinearColor(0.16f, 0.09f, 0.18f), 0.96f);
		DrawLabel(CX + 8.f, CY + 8.f, FString::FromInt(Slot), FLinearColor(0.95f, 0.78f, 0.40f), 0.95f);
		if (Ability)
		{
			DrawLabel(CX + 8.f, CY + 34.f, Ability->DisplayName.ToString(), FLinearColor(0.96f, 0.92f, 0.88f), 0.78f);
			if (Ability->FpCost > 0)
			{
				DrawLabel(CX + 8.f, CY + 86.f, HolypawUiCopy::FpCost(Ability->FpCost).ToString(),
					FLinearColor(0.95f, 0.78f, 0.40f), 0.72f);
			}
			else if (Ability->Stitch > 0)
			{
				DrawLabel(CX + 8.f, CY + 86.f, HolypawUiCopy::StitchPlus(Ability->Stitch).ToString(),
					FLinearColor(0.72f, 0.92f, 0.78f), 0.72f);
			}
		}
	}

	if (Pawn->IsBattleBusy())
	{
		DrawLabel(TrayX + TrayW * 0.5f - 80.f, TrayY - 28.f, TEXT("…"), FLinearColor(0.72f, 0.66f, 0.78f), 1.1f);
	}
	if (Pawn->GetToastAlpha() > 0.f)
	{
		DrawLabel(TrayX + 16.f, TrayY - 28.f, Pawn->GetToast(), FLinearColor(0.96f, 0.92f, 0.88f), 0.82f);
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

	const bool bTitle = !Pawn || Pawn->Mode == EHolypawPawnMode::Title;
	if (bTitle && !TitleWidgetFillsViewport(TitleWidget))
	{
		DrawTitleCanvas(Pawn);
		return;
	}

	if (Pawn)
	{
		if (Pawn->Mode == EHolypawPawnMode::Play)
		{
			DrawPlayCanvas(Pawn);
		}
		else if (Pawn->Mode == EHolypawPawnMode::Battle)
		{
			DrawBattleCanvas(Pawn);
		}
	}
}
