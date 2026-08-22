#include "UI/HolypawPauseWidget.h"
#include "UI/HolypawUiTheme.h"
#include "UI/HolypawUiCopy.h"
#include "HolypawGameInstance.h"
#include "Save/HolypawSaveGame.h"
#include "Character/HolypawCharacter.h"
#include "HolypawTypes.h"

void UHolypawPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

int32 UHolypawPauseWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(GetOwningPlayerPawn());
	if (!Pawn || Pawn->Mode != EHolypawPawnMode::Pause)
	{
		return Layer;
	}

	HolypawUi::FPaint Q{OutDrawElements, AllottedGeometry, Layer};
	const HolypawUi::FPalette& Pal = HolypawUi::Colors();
	const FVector2D Size = Q.Canvas();
	const UHolypawGameInstance* GI = UHolypawGameInstance::Get(Pawn);

	Q.Fill(FVector2D::ZeroVector, Size, Pal.Dim);
	const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(600.f, 470.f), 36.f);
	const FVector2D Origin = HolypawUi::Centered(Size, Panel);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(28.f, 22.f), EHolypawUiIcon::Pause, HolypawUiCopy::Paused().ToString(), Pal.Rose);
	Q.TextBlock(Origin + FVector2D(28.f, 64.f), HolypawUiCopy::PauseBlurb().ToString(), Pal.Powder, 0.9f, Panel.X - 56.f, 2);

	struct FRow
	{
		const TCHAR* Key;
		EHolypawUiIcon Icon;
		FText Label;
	};
	const FRow Rows[4] = {
		{TEXT("Enter"), EHolypawUiIcon::Paw, HolypawUiCopy::Resume()},
		{TEXT("F5"), EHolypawUiIcon::Save, HolypawUiCopy::SaveSlot()},
		{TEXT("F6"), EHolypawUiIcon::Mute, HolypawUiCopy::Mute()},
		{TEXT("F8"), EHolypawUiIcon::Teddy, HolypawUiCopy::BackToTitle()},
	};
	for (int32 I = 0; I < 4; ++I)
	{
		const FVector2D RowPos = Origin + FVector2D(40.f, 120.f + I * 58.f);
		Q.Fill(RowPos, FVector2D(Panel.X - 80.f, 50.f), Pal.Idle);
		Q.Frame(RowPos, FVector2D(Panel.X - 80.f, 50.f), Pal.Rose, 1.6f);
		Q.Keycap(RowPos + FVector2D(12.f, 14.f), Rows[I].Key);
		Q.Icon(RowPos + FVector2D(78.f, 11.f), 28.f, Rows[I].Icon, HolypawUi::IconTint(Rows[I].Icon));
		Q.Text(RowPos + FVector2D(116.f, 14.f), Rows[I].Label, Pal.Cream, 1.05f, Panel.X - 220.f);
	}

	Q.Chip(Origin + FVector2D(28.f, Panel.Y - 52.f), FVector2D(200.f, 30.f), EHolypawUiIcon::MapPin,
		HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone), Pal.Powder);
	Q.Chip(Origin + FVector2D(236.f, Panel.Y - 52.f), FVector2D(140.f, 30.f), EHolypawUiIcon::Heart,
		FString::Printf(TEXT("%s %d"), *HolypawUiCopy::Hearts().ToString(), Pawn->Story ? Pawn->Story->Converts : 0), Pal.Heart);
	if (GI && GI->Settings)
	{
		Q.Chip(Origin + FVector2D(384.f, Panel.Y - 52.f), FVector2D(150.f, 30.f), EHolypawUiIcon::Mute,
			GI->Settings->bMuted ? HolypawUiCopy::TitleMuteOn().ToString() : HolypawUiCopy::AudioOn().ToString(),
			GI->Settings->bMuted ? Pal.Danger : Pal.Mint);
	}
	return Layer + 6;
}
