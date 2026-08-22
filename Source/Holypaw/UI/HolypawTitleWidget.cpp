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
	Q.Fill(FVector2D(0.f, Size.Y * 0.18f), FVector2D(Size.X, Size.Y * 0.08f), HolypawUi::WithAlpha(Pal.Rose, 0.18f));
	Q.Fill(FVector2D(0.f, Size.Y * 0.74f), FVector2D(Size.X, Size.Y * 0.06f), HolypawUi::WithAlpha(Pal.Gold, 0.12f));

	const FVector2D Panel(840.f, FMath::Clamp(Size.Y - 120.f, 520.f, 640.f));
	const FVector2D Origin = HolypawUi::Centered(Size, Panel);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(36.f, 28.f), EHolypawUiIcon::Teddy, HolypawUiCopy::GameTitle().ToString(), Pal.Rose);
	Q.Text(Origin + FVector2D(36.f, 68.f), HolypawUiCopy::Tagline(), Pal.Powder, 0.95f, 760.f);

	const int32 Cursor = GI ? GI->TitleCursor : 0;
	for (int32 I = 0; I < UHolypawGameInstance::SlotCount; ++I)
	{
		const bool Sel = (I == Cursor);
		const FString Title = FString::Printf(TEXT("Slot %d"), I + 1);
		const FString Body = GI ? GI->SlotSummary(I) : HolypawUiCopy::EmptyPorch().ToString();
		Q.SlotCard(Origin + FVector2D(48.f, 118.f + I * 92.f), FVector2D(744.f, 80.f), Sel, Title, Body);
		Q.Keycap(Origin + FVector2D(760.f, 146.f + I * 92.f), FString::FromInt(I + 1));
	}

	Q.Icon(Origin + FVector2D(36.f, Panel.Y - 88.f), 22.f, EHolypawUiIcon::Heart, Pal.Heart);
	Q.Text(Origin + FVector2D(66.f, Panel.Y - 84.f), HolypawUiCopy::SlotHint(), Pal.Cream, 0.9f, 720.f);
	if (GI && GI->Settings)
	{
		const bool bMuted = GI->Settings->bMuted;
		Q.Icon(Origin + FVector2D(36.f, Panel.Y - 52.f), 18.f, EHolypawUiIcon::Mute, bMuted ? Pal.Danger : Pal.Mint);
		Q.Text(Origin + FVector2D(66.f, Panel.Y - 50.f),
			bMuted ? HolypawUiCopy::TitleMuteOn() : HolypawUiCopy::TitleMuteOff(),
			bMuted ? Pal.Danger : Pal.Mint, 0.85f, 400.f);
	}
	return Layer + 6;
}
