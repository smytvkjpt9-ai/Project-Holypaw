#include "UI/HolypawTitleWidget.h"
#include "UI/HolypawUiTheme.h"
#include "Rendering/DrawElements.h"
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
	const bool bTitle = !Pawn || Pawn->Mode == EHolypawPawnMode::Title;
	if (!bTitle)
	{
		return Layer;
	}

	HolypawUi::FPaint Q{OutDrawElements, AllottedGeometry, Layer};
	const HolypawUi::FPalette& Pal = HolypawUi::Colors();
	const FVector2D Size = Q.Canvas();
	if (Size.X < 8.f || Size.Y < 8.f)
	{
		return Layer;
	}
	const UHolypawGameInstance* GI = UHolypawGameInstance::Get(Pawn ? static_cast<const UObject*>(Pawn) : GetOwningPlayer());

	Q.Fill(FVector2D::ZeroVector, Size, Pal.Dim);
	if (!Pawn)
	{
		Q.Text(FVector2D((Size.X - 240.f) * 0.5f, Size.Y * 0.52f), TEXT("Stitching the porch…"), Pal.Muted, 0.9f, 240.f);
	}
	Q.Fill(FVector2D(0.f, Size.Y * 0.16f), FVector2D(Size.X, 10.f), HolypawUi::WithAlpha(Pal.Rose, 0.35f));
	Q.Fill(FVector2D(0.f, Size.Y * 0.78f), FVector2D(Size.X, 8.f), HolypawUi::WithAlpha(Pal.Gold, 0.28f));

	const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(820.f, 560.f), 40.f);
	const FVector2D Origin = HolypawUi::Centered(Size, Panel);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(32.f, 24.f), EHolypawUiIcon::Teddy, HolypawUiCopy::GameTitle().ToString(), Pal.Rose, Panel.X - 64.f);
	const float TagH = Q.TextBlock(Origin + FVector2D(32.f, 64.f), HolypawUiCopy::Tagline().ToString(), Pal.Powder, 0.9f, Panel.X - 64.f, 2);

	TArray<HolypawUi::FChipSpec> Actions;
	Actions.Add({EHolypawUiIcon::Paw, HolypawUiCopy::Continue().ToString(), Pal.Gold});
	Actions.Add({EHolypawUiIcon::Teddy, HolypawUiCopy::NewCoup().ToString(), Pal.Rose});
	Actions.Add({EHolypawUiIcon::Save, HolypawUiCopy::LoadSlot().ToString(), Pal.Mint});
	if (GI && GI->Settings)
	{
		const bool bMuted = GI->Settings->bMuted;
		Actions.Add({EHolypawUiIcon::Mute,
			bMuted ? HolypawUiCopy::TitleMuteOn().ToString() : HolypawUiCopy::TitleMuteOff().ToString(),
			bMuted ? Pal.Danger : Pal.Mint});
	}
	const float ActionMaxW = Panel.X - 64.f;
	float ActionH = 32.f;
	float RowW = 0.f;
	for (const HolypawUi::FChipSpec& C : Actions)
	{
		const float W = Q.MeasureChip(C.Label, 32.f);
		if (RowW > 0.f && RowW + W > ActionMaxW)
		{
			ActionH += 40.f;
			RowW = 0.f;
		}
		RowW += W + 8.f;
	}

	const float HeaderH = 64.f + TagH + 12.f;
	const float SlotArea = Panel.Y - HeaderH - ActionH - 36.f;
	const float SlotH = FMath::Clamp((SlotArea / 3.f) - 10.f, 56.f, 80.f);
	const int32 TitleCursor = GI ? GI->TitleCursor : 0;
	for (int32 I = 0; I < UHolypawGameInstance::SlotCount; ++I)
	{
		const bool Sel = (I == TitleCursor);
		const FVector2D Card = Origin + FVector2D(40.f, HeaderH + I * (SlotH + 8.f));
		Q.SlotCard(Card, FVector2D(Panel.X - 80.f, SlotH), Sel,
			HolypawUiCopy::SlotN(I + 1).ToString(),
			GI ? GI->SlotSummary(I) : HolypawUiCopy::EmptyPorch().ToString(),
			FString::FromInt(I + 1));
	}

	Q.ChipRow(Origin + FVector2D(32.f, Panel.Y - ActionH - 20.f), ActionMaxW, Actions, 32.f);
	return Layer + 6;
}
