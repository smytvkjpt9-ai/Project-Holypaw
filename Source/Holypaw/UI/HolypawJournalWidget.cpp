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

	const FVector2D Panel(FMath::Min(Size.X - 64.f, 1080.f), FMath::Min(Size.Y - 64.f, 680.f));
	const FVector2D Origin((Size.X - Panel.X) * 0.5f, (Size.Y - Panel.Y) * 0.5f);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(28.f, 22.f), EHolypawUiIcon::Book, HolypawUiCopy::JournalTitle().ToString(), Pal.Gold);

	const float Mid = Origin.X + Panel.X * 0.46f;
	Q.Fill(FVector2D(Mid, Origin.Y + 70.f), FVector2D(4.f, Panel.Y - 120.f), HolypawUi::WithAlpha(Pal.Rose, 0.45f));

	Q.Text(Origin + FVector2D(32.f, 68.f), HolypawUiCopy::Campaign(), Pal.Rose, 0.95f, 300.f);
	const TArray<FMissionDef>& Missions = HolypawCatalog::GetMissions();
	const int32 Current = Pawn->Story->CurrentIndex;
	for (int32 I = 0; I < Missions.Num(); ++I)
	{
		const bool bNow = I == Current;
		const bool bDone = I < Current || (Pawn->Story->bCampaignComplete && I == Missions.Num() - 1);
		const FVector2D Row = Origin + FVector2D(32.f, 96.f + I * 34.f);
		const EHolypawUiIcon Mark = bDone ? EHolypawUiIcon::Check : (bNow ? EHolypawUiIcon::Arrow : EHolypawUiIcon::Book);
		const FLinearColor Tint = bNow ? Pal.Gold : (bDone ? Pal.Mint : Pal.Muted);
		Q.Icon(Row, 18.f, Mark, Tint);
		Q.Text(Row + FVector2D(26.f, 0.f), Missions[I].Title, Tint, bNow ? 0.95f : 0.82f, 400.f);
	}

	const FVector2D Right(Mid + 24.f, Origin.Y + 68.f);
	Q.Text(Right, HolypawUiCopy::Current(), Pal.Gold, 0.95f, 200.f);
	const FMissionDef Cur = Pawn->Story->GetCurrent();
	Q.Text(Right + FVector2D(0.f, 28.f), Cur.Title, Pal.Cream, 1.15f, 460.f);
	Q.Text(Right + FVector2D(0.f, 60.f), Cur.Brief, Pal.Powder, 0.85f, 460.f);
	Q.Text(Right + FVector2D(0.f, 118.f), Cur.Hint, Pal.Muted, 0.8f, 460.f);

	const float ChipY = Right.Y + 168.f;
	Q.Chip(FVector2D(Right.X, ChipY), FVector2D(148.f, 36.f), EHolypawUiIcon::Party,
		FString::Printf(TEXT("Recruits %d"), Pawn->Story->Recruits), Pal.Gold);
	Q.Chip(FVector2D(Right.X + 156.f, ChipY), FVector2D(148.f, 36.f), EHolypawUiIcon::Heart,
		FString::Printf(TEXT("Hearts %d"), Pawn->Story->Converts), Pal.Heart);
	Q.Chip(FVector2D(Right.X + 312.f, ChipY), FVector2D(148.f, 36.f), EHolypawUiIcon::Mill,
		FString::Printf(TEXT("Unstuffed %d"), Pawn->Story->Kills), Pal.Danger);
	Q.Chip(FVector2D(Right.X, ChipY + 44.f), FVector2D(148.f, 36.f), EHolypawUiIcon::Halo,
		FString::Printf(TEXT("Miracles %d"), Pawn->Story->Miracles), Pal.Gold);
	Q.Chip(FVector2D(Right.X + 156.f, ChipY + 44.f), FVector2D(200.f, 36.f), EHolypawUiIcon::MapPin,
		FString::Printf(TEXT("Cities %d/%d"), Pawn->Story->ZonesVisited.Num(), HolypawCatalog::GetCities().Num()), Pal.Mint);

	Q.Text(Right + FVector2D(0.f, 268.f), HolypawUiCopy::Errands(), Pal.Rose, 0.95f, 200.f);
	int32 Shown = 0;
	for (const FHolypawQuestDef& Quest : HolypawCatalog::GetQuests())
	{
		if (Shown >= 6)
		{
			break;
		}
		const bool bDone = Pawn->GetQuestDone().Contains(Quest.Id);
		const bool bActive = Pawn->GetQuestActive().Contains(Quest.Id);
		if (!bDone && !bActive)
		{
			continue;
		}
		const FVector2D Row = Right + FVector2D(0.f, 296.f + Shown * 28.f);
		Q.Icon(Row, 16.f, bDone ? EHolypawUiIcon::Check : EHolypawUiIcon::Arrow, bDone ? Pal.Mint : Pal.Gold);
		Q.Text(Row + FVector2D(22.f, 0.f),
			bDone ? Quest.Title.ToString() : FString::Printf(TEXT("%s — %s"), *Quest.Title.ToString(), *Quest.Brief.ToString()),
			bDone ? Pal.Mint : Pal.Cream, 0.8f, 440.f);
		++Shown;
	}
	if (Shown == 0)
	{
		Q.Text(Right + FVector2D(0.f, 296.f), TEXT("Talk 4 takes a job. Talk 3 turns it in."), Pal.Muted, 0.85f, 440.f);
	}

	Q.Footer(Origin, Panel, HolypawUiCopy::JournalClose().ToString());
	return Layer + 6;
}
