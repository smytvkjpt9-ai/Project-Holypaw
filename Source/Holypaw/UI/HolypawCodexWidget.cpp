#include "UI/HolypawCodexWidget.h"
#include "UI/HolypawUiTheme.h"
#include "Rendering/DrawElements.h"
#include "UI/HolypawUiCopy.h"
#include "Character/HolypawCharacter.h"
#include "HolypawCatalog.h"

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
	Q.Caption(Origin + FVector2D(24.f, 16.f), EHolypawUiIcon::Mill, HolypawUiCopy::CodexTitle().ToString(), Pal.Danger, Panel.X * 0.42f);

	TArray<HolypawUi::FChipSpec> Head;
	Head.Add({EHolypawUiIcon::Book, HolypawUiCopy::SeenN(Pawn->GetCodexSeenCount()).ToString(), Pal.Gold});
	Head.Add({EHolypawUiIcon::Check, HolypawUiCopy::FellN(Pawn->GetCodexDefeatedCount(), Pawn->GetCodexTotal()).ToString(), Pal.Mint});
	Q.ChipRow(Origin + FVector2D(Panel.X - 360.f, 16.f), 340.f, Head, 30.f);

	const TArray<FVillainDef>& All = HolypawCatalog::GetVillains();
	TArray<int32> Order;
	for (int32 I = 0; I < All.Num(); ++I)
	{
		if (Pawn->HasDefeatedVillain(All[I].Id))
		{
			Order.Add(I);
		}
	}
	for (int32 I = 0; I < All.Num(); ++I)
	{
		if (Pawn->HasSeenVillain(All[I].Id) && !Pawn->HasDefeatedVillain(All[I].Id))
		{
			Order.Add(I);
		}
	}
	for (int32 I = 0; I < All.Num(); ++I)
	{
		if (!Pawn->HasSeenVillain(All[I].Id))
		{
			Order.Add(I);
		}
	}

	const float ColW = (Panel.X - 56.f) * 0.5f;
	const float RowH = FMath::Min(30.f, (Panel.Y - 100.f) / 20.f);
	for (int32 N = 0; N < Order.Num(); ++N)
	{
		const FVillainDef& D = All[Order[N]];
		const bool bLeft = N < 20;
		const FVector2D Row(
			Origin.X + 24.f + (bLeft ? 0.f : ColW + 8.f),
			Origin.Y + 58.f + (bLeft ? N : N - 20) * RowH);
		const bool Seen = Pawn->HasSeenVillain(D.Id);
		const bool Beat = Pawn->HasDefeatedVillain(D.Id);
		if (!Seen)
		{
			Q.Icon(Row, 14.f, EHolypawUiIcon::Heart, HolypawUi::WithAlpha(Pal.Powder, 0.35f));
			Q.Text(Row + FVector2D(20.f, 0.f), HolypawUiCopy::CodexUnknown(), Pal.Muted, 0.78f, ColW - 28.f);
			if (RowH >= 26.f)
			{
				Q.Text(Row + FVector2D(20.f, 13.f), FString(HolypawCatalog::ZoneDisplayName(D.HomeZone)), Pal.Muted, 0.62f, ColW - 28.f);
			}
			continue;
		}
		Q.Icon(Row, 14.f, Beat ? EHolypawUiIcon::Check : EHolypawUiIcon::Mill, Beat ? Pal.Mint : Pal.Danger);
		Q.Text(Row + FVector2D(20.f, 0.f), HolypawUi::Ellipsize(D.DisplayName.ToString(), ColW - 28.f, 0.78f),
			Beat ? Pal.Mint : Pal.Cream, 0.78f, ColW - 24.f);
		if (RowH >= 26.f)
		{
			Q.Text(Row + FVector2D(20.f, 13.f),
				HolypawUi::Ellipsize(FString::Printf(TEXT("%s  ·  %s"),
					HolypawCatalog::ZoneDisplayName(D.HomeZone),
					*(Beat ? HolypawUiCopy::CodexFell() : HolypawUiCopy::CodexHunting()).ToString()), ColW - 28.f, 0.62f),
				Beat ? Pal.Mint : Pal.Powder, 0.62f, ColW - 24.f);
		}
	}

	Q.Footer(Origin, Panel, HolypawUiCopy::CodexUnseen().ToString());
	return Layer + 6;
}
