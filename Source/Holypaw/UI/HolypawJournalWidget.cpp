#include "UI/HolypawJournalWidget.h"
#include "UI/HolypawUiTheme.h"
#include "UI/HolypawUiCopy.h"
#include "Character/HolypawCharacter.h"
#include "Components/MissionComponent.h"
#include "HolypawTypes.h"

void UHolypawJournalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

int32 UHolypawJournalWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(GetOwningPlayerPawn());
	if (!Pawn || !Pawn->IsJournalOpen() || !Pawn->Story)
	{
		return Layer;
	}

	HolypawUi::FPaint Q{OutDrawElements, AllottedGeometry, Layer};
	const HolypawUi::FPalette& Pal = HolypawUi::Colors();
	const FVector2D Size = Q.Canvas();
	Q.Fill(FVector2D::ZeroVector, Size, Pal.Dim);

	const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(1080.f, 700.f), 32.f);
	const FVector2D Origin = HolypawUi::Centered(Size, Panel);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(24.f, 18.f), EHolypawUiIcon::Book, HolypawUiCopy::JournalTitle().ToString(), Pal.Gold, Panel.X - 56.f);

	const float Mid = Origin.X + Panel.X * 0.42f;
	Q.Fill(FVector2D(Mid, Origin.Y + 62.f), FVector2D(3.f, Panel.Y - 110.f), HolypawUi::WithAlpha(Pal.Rose, 0.45f));

	Q.Text(Origin + FVector2D(28.f, 60.f), HolypawUiCopy::Campaign(), Pal.Rose, 0.9f, 220.f);
	const TArray<FMissionDef>& Missions = HolypawCatalog::GetMissions();
	const int32 Current = Pawn->Story->CurrentIndex;
	const float RowH = FMath::Min(32.f, (Panel.Y - 160.f) / FMath::Max(1, Missions.Num()));
	for (int32 I = 0; I < Missions.Num(); ++I)
	{
		const bool bNow = I == Current;
		const bool bDone = I < Current || (Pawn->Story->bCampaignComplete && I == Missions.Num() - 1);
		const FVector2D Row = Origin + FVector2D(24.f, 86.f + I * RowH);
		if (bNow)
		{
			Q.Fill(Row - FVector2D(4.f, 2.f), FVector2D(Mid - Origin.X - 28.f, RowH - 2.f), Pal.Select);
			Q.Frame(Row - FVector2D(4.f, 2.f), FVector2D(Mid - Origin.X - 28.f, RowH - 2.f), Pal.Gold, 1.4f);
		}
		const EHolypawUiIcon Mark = bDone ? EHolypawUiIcon::Check : (bNow ? EHolypawUiIcon::Arrow : EHolypawUiIcon::Book);
		const FLinearColor Tint = bNow ? Pal.Gold : (bDone ? Pal.Mint : Pal.Muted);
		Q.Icon(Row, 16.f, Mark, Tint);
		Q.Text(Row + FVector2D(22.f, 0.f), HolypawUi::Ellipsize(Missions[I].Title.ToString(), Mid - Origin.X - 60.f, 0.8f),
			Tint, bNow ? 0.88f : 0.78f, Mid - Origin.X - 56.f);
	}

	const FVector2D Right(Mid + 20.f, Origin.Y + 60.f);
	const float RightW = Origin.X + Panel.X - Right.X - 28.f;
	const float FooterY = Origin.Y + Panel.Y - 50.f;
	Q.Text(Right, HolypawUiCopy::Current(), Pal.Gold, 0.9f, 160.f);
	const FMissionDef Cur = Pawn->Story->GetCurrent();
	Q.Text(Right + FVector2D(0.f, 24.f), HolypawUi::Ellipsize(Cur.Title.ToString(), RightW, 1.1f), Pal.Cream, 1.1f, RightW);

	int32 BriefLines = 4;
	int32 HintLines = 3;
	while (Right.Y + 52.f + BriefLines * 16.f + HintLines * 14.f + 120.f > FooterY && BriefLines > 2)
	{
		--BriefLines;
	}
	while (Right.Y + 52.f + BriefLines * 16.f + HintLines * 14.f + 120.f > FooterY && HintLines > 1)
	{
		--HintLines;
	}

	const float BriefH = Q.TextBlock(Right + FVector2D(0.f, 52.f), Cur.Brief.ToString(), Pal.Powder, 0.82f, RightW, BriefLines);
	const float HintH = Q.TextBlock(Right + FVector2D(0.f, 56.f + BriefH), Cur.Hint.ToString(), Pal.Muted, 0.75f, RightW, HintLines);

	TArray<HolypawUi::FChipSpec> Stats;
	Stats.Add({EHolypawUiIcon::Heart, HolypawUiCopy::HeartsN(Pawn->Story->Converts).ToString(), Pal.Heart});
	Stats.Add({EHolypawUiIcon::Party, HolypawUiCopy::RecruitsN(Pawn->Story->Recruits).ToString(), Pal.Gold});
	Stats.Add({EHolypawUiIcon::Halo, HolypawUiCopy::MiraclesN(Pawn->Story->Miracles).ToString(), Pal.Gold});
	Stats.Add({EHolypawUiIcon::MapPin, HolypawUiCopy::CitiesN(Pawn->Story->ZonesVisited.Num(), HolypawCatalog::GetCities().Num()).ToString(), Pal.Mint});
	const float ChipY = Right.Y + 64.f + BriefH + HintH;
	const float ChipH = Q.ChipRow(FVector2D(Right.X, ChipY), RightW, Stats, 30.f);

	Q.Text(FVector2D(Right.X, ChipY + ChipH + 10.f), HolypawUiCopy::Errands(), Pal.Rose, 0.9f, 160.f);
	int32 Shown = 0;
	const float ErrandTop = ChipY + ChipH + 32.f;
	const int32 Room = FMath::Max(2, FMath::FloorToInt((FooterY - ErrandTop) / 26.f));
	for (const FHolypawQuestDef& Quest : HolypawCatalog::GetQuests())
	{
		if (Shown >= Room)
		{
			break;
		}
		const bool bDone = Pawn->GetQuestDone().Contains(Quest.Id);
		const bool bActive = Pawn->GetQuestActive().Contains(Quest.Id);
		if (!bDone && !bActive)
		{
			continue;
		}
		const FVector2D Row(Right.X, ErrandTop + Shown * 26.f);
		Q.Icon(Row, 14.f, bDone ? EHolypawUiIcon::Check : EHolypawUiIcon::Arrow, bDone ? Pal.Mint : Pal.Gold);
		const FString Line = bDone ? Quest.Title.ToString() : Quest.Title.ToString() + TEXT(" — ") + Quest.Brief.ToString();
		Q.Text(Row + FVector2D(20.f, 0.f), HolypawUi::Ellipsize(Line, RightW - 20.f, 0.75f),
			bDone ? Pal.Mint : Pal.Cream, 0.75f, RightW - 16.f);
		++Shown;
	}
	if (Shown == 0)
	{
		Q.Text(FVector2D(Right.X, ErrandTop), HolypawUiCopy::JournalEmptyErrands(), Pal.Muted, 0.8f, RightW);
	}

	Q.Footer(Origin, Panel, HolypawUiCopy::JournalClose().ToString());
	return Layer + 6;
}
