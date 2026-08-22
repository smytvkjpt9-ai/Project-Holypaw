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
	const FVector2D Panel(620.f, 460.f);
	const FVector2D Origin = HolypawUi::Centered(Size, Panel);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(32.f, 28.f), EHolypawUiIcon::Pause, HolypawUiCopy::Paused().ToString(), Pal.Rose);
	Q.Text(Origin + FVector2D(32.f, 72.f), HolypawUiCopy::PauseBlurb(), Pal.Powder, 0.95f, 540.f);

	struct FRow
	{
		const TCHAR* Key;
		EHolypawUiIcon Icon;
		FText Label;
	};
	const FRow Rows[4] = {
		{TEXT("Ent"), EHolypawUiIcon::Paw, HolypawUiCopy::Resume()},
		{TEXT("F5"), EHolypawUiIcon::Save, HolypawUiCopy::SaveSlot()},
		{TEXT("F6"), EHolypawUiIcon::Mute, HolypawUiCopy::Mute()},
		{TEXT("F8"), EHolypawUiIcon::Teddy, HolypawUiCopy::BackToTitle()},
	};
	for (int32 I = 0; I < 4; ++I)
	{
		const FVector2D RowPos = Origin + FVector2D(48.f, 126.f + I * 58.f);
		Q.Fill(RowPos, FVector2D(524.f, 50.f), Pal.Idle);
		Q.DashRect(RowPos + FVector2D(4.f, 4.f), FVector2D(516.f, 42.f), I == 0 ? Pal.Gold : Pal.Rose, 1.3f, 6.f);
		Q.Keycap(RowPos + FVector2D(12.f, 14.f), Rows[I].Key);
		Q.Icon(RowPos + FVector2D(52.f, 10.f), 28.f, Rows[I].Icon, HolypawUi::IconTint(Rows[I].Icon));
		Q.Text(RowPos + FVector2D(90.f, 14.f), Rows[I].Label, Pal.Cream, 1.05f, 400.f);
	}

	if (GI && GI->Settings)
	{
		const FString Zone = HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone);
		const int32 Hearts = Pawn->Story ? Pawn->Story->Converts : 0;
		Q.Icon(Origin + FVector2D(40.f, Panel.Y - 48.f), 18.f, EHolypawUiIcon::Heart, Pal.Heart);
		Q.Text(Origin + FVector2D(66.f, Panel.Y - 46.f),
			FString::Printf(TEXT("%s   Hearts %d   %s"), *Zone, Hearts,
				GI->Settings->bMuted ? TEXT("muted") : TEXT("audio on")),
			Pal.Muted, 0.85f, 500.f);
	}
	return Layer + 6;
}
