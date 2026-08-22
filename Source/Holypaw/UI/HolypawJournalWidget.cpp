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
	Q.Caption(Origin + FVector2D(24.f, 18.f), EHolypawUiIcon::Book, HolypawUiCopy::JournalTitle().ToString(), Pal.Gold);

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
	Q.Text(Right, HolypawUiCopy::Current(), Pal.Gold, 0.9f, 160.f);
	const FMissionDef Cur = Pawn->Story->GetCurrent();
	Q.Text(Right + FVector2D(0.f, 24.f), Cur.Title, Pal.Cream, 1.1f, RightW);
	const float BriefH = Q.TextBlock(Right + FVector2D(0.f, 52.f), Cur.Brief.ToString(), Pal.Powder, 0.82f, RightW, 4);
	Q.TextBlock(Right + FVector2D(0.f, 56.f + BriefH), Cur.Hint.ToString(), Pal.Muted, 0.75f, RightW, 3);

	const float ChipY = Right.Y + 56.f + BriefH + 62.f;
	const float ChipW = FMath::Max(120.f, (RightW - 16.f) * 0.5f);
	Q.Chip(FVector2D(Right.X, ChipY), FVector2D(ChipW, 32.f), EHolypawUiIcon::Heart,
		FString::Printf(TEXT("%s %d"), *HolypawUiCopy::Hearts().ToString(), Pawn->Story->Converts), Pal.Heart);
	Q.Chip(FVector2D(Right.X + ChipW + 8.f, ChipY), FVector2D(ChipW, 32.f), EHolypawUiIcon::Party,
		FString::Printf(TEXT("Recruits %d"), Pawn->Story->Recruits), Pal.Gold);
	Q.Chip(FVector2D(Right.X, ChipY + 38.f), FVector2D(ChipW, 32.f), EHolypawUiIcon::Halo,
		FString::Printf(TEXT("Miracles %d"), Pawn->Story->Miracles), Pal.Gold);
	Q.Chip(FVector2D(Right.X + ChipW + 8.f, ChipY + 38.f), FVector2D(ChipW, 32.f), EHolypawUiIcon::MapPin,
		FString::Printf(TEXT("Cities %d/%d"), Pawn->Story->ZonesVisited.Num(), HolypawCatalog::GetCities().Num()), Pal.Mint);

	Q.Text(FVector2D(Right.X, ChipY + 80.f), HolypawUiCopy::Errands(), Pal.Rose, 0.9f, 160.f);
	int32 Shown = 0;
	const float ErrandTop = ChipY + 104.f;
	const int32 Room = FMath::Max(2, FMath::FloorToInt((Origin.Y + Panel.Y - 56.f - ErrandTop) / 26.f));
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
		Q.Text(Row + FVector2D(20.f, 0.f),
			HolypawUi::Ellipsize(bDone ? Quest.Title.ToString() : Quest.Title.ToString() + TEXT(" — ") + Quest.Brief.ToString(), RightW - 20.f, 0.75f),
			bDone ? Pal.Mint : Pal.Cream, 0.75f, RightW - 16.f);
		++Shown;
	}
	if (Shown == 0)
	{
		Q.Text(FVector2D(Right.X, ErrandTop), TEXT("Talk 4 takes a job. Talk 3 turns it in."), Pal.Muted, 0.8f, RightW);
	}

	Q.Footer(Origin, Panel, HolypawUiCopy::JournalClose().ToString());
	return Layer + 6;
}
