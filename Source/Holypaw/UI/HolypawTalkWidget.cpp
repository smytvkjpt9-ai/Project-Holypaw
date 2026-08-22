#include "UI/HolypawTalkWidget.h"
#include "UI/HolypawUiTheme.h"
#include "UI/HolypawUiCopy.h"
#include "Character/HolypawCharacter.h"
#include "HolypawTypes.h"

void UHolypawTalkWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

int32 UHolypawTalkWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(GetOwningPlayerPawn());
	if (!Pawn || !Pawn->IsTalkOpen())
	{
		return Layer;
	}

	HolypawUi::FPaint Q{OutDrawElements, AllottedGeometry, Layer};
	const HolypawUi::FPalette& Pal = HolypawUi::Colors();
	const FVector2D Size = Q.Canvas();

	const FVector2D Panel(FMath::Min(Size.X - 72.f, 980.f), 280.f);
	const FVector2D Origin((Size.X - Panel.X) * 0.5f, Size.Y - Panel.Y - 36.f);
	Q.Panel(Origin, Panel);

	Q.Fill(Origin + FVector2D(20.f, 18.f), FVector2D(220.f, 44.f), Pal.Select);
	Q.Icon(Origin + FVector2D(28.f, 24.f), 28.f, EHolypawUiIcon::Speech, Pal.Powder);
	const FString Speaker = Pawn->GetTalkSpeaker().IsEmpty() ? TEXT("Believer") : Pawn->GetTalkSpeaker();
	Q.Text(Origin + FVector2D(64.f, 26.f), Speaker, Pal.Cream, 1.1f, 180.f);
	Q.Text(Origin + FVector2D(252.f, 22.f), HolypawUiCopy::TalkTitle(), Pal.Gold, 0.85f, 200.f);

	Q.Text(Origin + FVector2D(28.f, 74.f), Pawn->GetTalkBody(), Pal.Cream, 0.95f, Panel.X - 56.f);

	if (const FHolypawTalkDef* Talk = HolypawCatalog::FindTalk(Speaker))
	{
		if (!Talk->LineC.IsEmpty())
		{
			Q.Text(Origin + FVector2D(28.f, 128.f),
				Talk->FaithNeed > 0
					? FString::Printf(TEXT("Quiet line at %d FP."), Talk->FaithNeed)
					: HolypawUiCopy::QuietLine().ToString(),
				Pal.Muted, 0.8f, 500.f);
		}
	}

	FString Errand;
	FLinearColor ErrandCol = Pal.Muted;
	if (const FHolypawQuestDef* Quest = HolypawCatalog::FindQuestByGiver(Speaker))
	{
		if (Pawn->GetQuestDone().Contains(Quest->Id))
		{
			Errand = HolypawUiCopy::ErrandDone().ToString();
			ErrandCol = Pal.Mint;
		}
		else if (Pawn->GetQuestActive().Contains(Quest->Id))
		{
			Errand = Quest->Brief.ToString();
			ErrandCol = Pal.Gold;
		}
		else
		{
			Errand = FString::Printf(TEXT("%s: %s"), *HolypawUiCopy::JobReady().ToString(), *Quest->Title.ToString());
			ErrandCol = Pal.Rose;
		}
	}
	if (!Errand.IsEmpty())
	{
		Q.Chip(Origin + FVector2D(Panel.X - 340.f, 18.f), FVector2D(312.f, 40.f), EHolypawUiIcon::Book, Errand, ErrandCol);
	}

	Q.VerbRow(Origin + FVector2D(24.f, 168.f), TEXT("1"), EHolypawUiIcon::Speech, HolypawUiCopy::Listen().ToString(), true);
	Q.VerbRow(Origin + FVector2D(248.f, 168.f), TEXT("2"), EHolypawUiIcon::Lantern, HolypawUiCopy::AskWay().ToString(), false);
	Q.VerbRow(Origin + FVector2D(472.f, 168.f), TEXT("3"), EHolypawUiIcon::Check, HolypawUiCopy::TurnIn().ToString(), false);
	Q.VerbRow(Origin + FVector2D(696.f, 168.f), TEXT("4"), EHolypawUiIcon::Book, HolypawUiCopy::TakeJob().ToString(), false);
	Q.Footer(Origin, Panel, TEXT("Esc closes. Keep listening for the third thought."));
	return Layer + 6;
}
