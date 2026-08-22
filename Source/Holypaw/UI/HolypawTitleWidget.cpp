#include "UI/HolypawTitleWidget.h"
#include "UI/HolypawUiTheme.h"
#include "UI/HolypawUiCopy.h"
#include "HolypawGameInstance.h"
#include "Save/HolypawSaveGame.h"
#include "Character/HolypawCharacter.h"

void UHolypawTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

int32 UHolypawTitleWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(GetOwningPlayerPawn());
	if (!Pawn || Pawn->Mode != EHolypawPawnMode::Title)
	{
		return Layer;
	}

	HolypawUi::FPaint Q{OutDrawElements, AllottedGeometry, Layer};
	const HolypawUi::FPalette& Pal = HolypawUi::Colors();
	const FVector2D Size = Q.Canvas();
	const UHolypawGameInstance* GI = UHolypawGameInstance::Get(Pawn);

	Q.Fill(FVector2D::ZeroVector, Size, Pal.Dim);
	Q.Fill(FVector2D(0.f, Size.Y * 0.16f), FVector2D(Size.X, 10.f), HolypawUi::WithAlpha(Pal.Rose, 0.35f));
	Q.Fill(FVector2D(0.f, Size.Y * 0.78f), FVector2D(Size.X, 8.f), HolypawUi::WithAlpha(Pal.Gold, 0.28f));

	const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(820.f, 560.f), 40.f);
	const FVector2D Origin = HolypawUi::Centered(Size, Panel);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(32.f, 24.f), EHolypawUiIcon::Teddy, HolypawUiCopy::GameTitle().ToString(), Pal.Rose);
	Q.TextBlock(Origin + FVector2D(32.f, 64.f), HolypawUiCopy::Tagline().ToString(), Pal.Powder, 0.9f, Panel.X - 64.f, 2);

	const int32 Cursor = GI ? GI->TitleCursor : 0;
	for (int32 I = 0; I < UHolypawGameInstance::SlotCount; ++I)
	{
		const bool Sel = (I == Cursor);
		const FVector2D Card = Origin + FVector2D(40.f, 118.f + I * 88.f);
		Q.SlotCard(Card, FVector2D(Panel.X - 80.f, 76.f), Sel,
			FString::Printf(TEXT("Slot %d"), I + 1),
			GI ? GI->SlotSummary(I) : HolypawUiCopy::EmptyPorch().ToString());
		Q.Keycap(Card + FVector2D(Panel.X - 128.f, 26.f), FString::FromInt(I + 1));
	}

	const float ActionY = Origin.Y + Panel.Y - 78.f;
	Q.Chip(FVector2D(Origin.X + 32.f, ActionY), FVector2D(170.f, 32.f), EHolypawUiIcon::Paw, HolypawUiCopy::Continue().ToString(), Pal.Gold);
	Q.Chip(FVector2D(Origin.X + 210.f, ActionY), FVector2D(150.f, 32.f), EHolypawUiIcon::Teddy, HolypawUiCopy::NewCoup().ToString(), Pal.Rose);
	Q.Chip(FVector2D(Origin.X + 368.f, ActionY), FVector2D(130.f, 32.f), EHolypawUiIcon::Save, HolypawUiCopy::LoadSlot().ToString(), Pal.Mint);
	if (GI && GI->Settings)
	{
		const bool bMuted = GI->Settings->bMuted;
		Q.Chip(FVector2D(Origin.X + 506.f, ActionY), FVector2D(140.f, 32.f), EHolypawUiIcon::Mute,
			bMuted ? HolypawUiCopy::TitleMuteOn().ToString() : HolypawUiCopy::TitleMuteOff().ToString(),
			bMuted ? Pal.Danger : Pal.Mint);
	}
	return Layer + 6;
}
