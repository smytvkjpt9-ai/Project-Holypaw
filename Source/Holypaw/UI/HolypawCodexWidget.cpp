#include "UI/HolypawCodexWidget.h"
#include "UI/HolypawUiTheme.h"
#include "UI/HolypawUiCopy.h"
#include "Character/HolypawCharacter.h"
#include "HolypawTypes.h"

void UHolypawCodexWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

int32 UHolypawCodexWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(GetOwningPlayerPawn());
	if (!Pawn || !Pawn->IsCodexOpen())
	{
		return Layer;
	}

	HolypawUi::FPaint Q{OutDrawElements, AllottedGeometry, Layer};
	const HolypawUi::FPalette& Pal = HolypawUi::Colors();
	const FVector2D Size = Q.Canvas();
	Q.Fill(FVector2D::ZeroVector, Size, Pal.Dim);

	const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(1040.f, 680.f), 28.f);
	const FVector2D Origin = HolypawUi::Centered(Size, Panel);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(24.f, 16.f), EHolypawUiIcon::Mill, HolypawUiCopy::CodexTitle().ToString(), Pal.Danger);
	Q.Chip(Origin + FVector2D(Panel.X - 360.f, 18.f), FVector2D(140.f, 30.f), EHolypawUiIcon::Book,
		FString::Printf(TEXT("Seen %d"), Pawn->GetCodexSeenCount()), Pal.Gold);
	Q.Chip(Origin + FVector2D(Panel.X - 208.f, 18.f), FVector2D(180.f, 30.f), EHolypawUiIcon::Check,
		FString::Printf(TEXT("Fell %d / %d"), Pawn->GetCodexDefeatedCount(), Pawn->GetCodexTotal()), Pal.Mint);

	const TArray<FVillainDef>& All = HolypawCatalog::GetVillains();
	const float ColW = (Panel.X - 56.f) * 0.5f;
	const float RowH = FMath::Min(28.f, (Panel.Y - 90.f) / 20.f);
	for (int32 I = 0; I < All.Num(); ++I)
	{
		const FVillainDef& D = All[I];
		const bool bLeft = I < 20;
		const FVector2D Row(
			Origin.X + 24.f + (bLeft ? 0.f : ColW + 8.f),
			Origin.Y + 60.f + (bLeft ? I : I - 20) * RowH);
		const bool Seen = Pawn->HasSeenVillain(D.Id);
		const bool Beat = Pawn->HasDefeatedVillain(D.Id);
		if (!Seen)
		{
			Q.Icon(Row, 14.f, EHolypawUiIcon::Heart, HolypawUi::WithAlpha(Pal.Powder, 0.35f));
			Q.Text(Row + FVector2D(20.f, 0.f),
				FString::Printf(TEXT("????  ·  %s"), HolypawCatalog::ZoneDisplayName(D.HomeZone)),
				Pal.Muted, 0.72f, ColW - 28.f);
			continue;
		}
		Q.Icon(Row, 14.f, Beat ? EHolypawUiIcon::Check : EHolypawUiIcon::Mill, Beat ? Pal.Mint : Pal.Danger);
		Q.Text(Row + FVector2D(20.f, 0.f),
			HolypawUi::Ellipsize(FString::Printf(TEXT("%s  ·  %s  ·  %s"),
				*D.DisplayName.ToString(),
				HolypawCatalog::ZoneDisplayName(D.HomeZone),
				Beat ? TEXT("fell") : TEXT("seen")), ColW - 28.f, 0.72f),
			Beat ? Pal.Mint : Pal.Cream, 0.72f, ColW - 24.f);
	}

	Q.Footer(Origin, Panel, HolypawUiCopy::CodexUnseen().ToString());
	return Layer + 6;
}
