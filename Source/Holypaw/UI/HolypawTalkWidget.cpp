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

	const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(1000.f, 340.f), 36.f);
	const FVector2D Origin((Size.X - Panel.X) * 0.5f, Size.Y - Panel.Y - 28.f);
	Q.Panel(Origin, Panel);

	Q.Fill(Origin + FVector2D(20.f, 16.f), FVector2D(FMath::Min(240.f, Panel.X * 0.28f), 40.f), Pal.Select);
	Q.Icon(Origin + FVector2D(28.f, 20.f), 26.f, EHolypawUiIcon::Speech, Pal.Powder);
	const FString Speaker = Pawn->GetTalkSpeaker().IsEmpty() ? TEXT("Believer") : Pawn->GetTalkSpeaker();
	Q.Text(Origin + FVector2D(60.f, 22.f), HolypawUi::Ellipsize(Speaker, 170.f, 1.05f), Pal.Cream, 1.05f, 180.f);

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
			Errand = Quest->Title.ToString();
			ErrandCol = Pal.Gold;
		}
		else
		{
			Errand = Quest->Title.ToString();
			ErrandCol = Pal.Rose;
		}
	}
	if (!Errand.IsEmpty())
	{
		Q.Chip(Origin + FVector2D(Panel.X - 280.f, 16.f), FVector2D(256.f, 40.f), EHolypawUiIcon::Book, Errand, ErrandCol);
	}

	Q.TextBlock(Origin + FVector2D(24.f, 68.f), Pawn->GetTalkBody(), Pal.Cream, 0.95f, Panel.X - 48.f, 4);
	if (const FHolypawTalkDef* Talk = HolypawCatalog::FindTalk(Speaker))
	{
		if (!Talk->LineC.IsEmpty())
		{
			Q.Text(Origin + FVector2D(24.f, 148.f),
				Talk->FaithNeed > 0
					? FString::Printf(TEXT("Quiet line at %d FP."), Talk->FaithNeed)
					: HolypawUiCopy::QuietLine().ToString(),
				Pal.Muted, 0.75f, Panel.X - 48.f);
		}
	}

	const float VerbW = (Panel.X - 56.f) / 4.f;
	const float VerbY = Origin.Y + Panel.Y - 78.f;
	Q.VerbRow(FVector2D(Origin.X + 20.f, VerbY), VerbW - 8.f, TEXT("1"), EHolypawUiIcon::Speech, HolypawUiCopy::Listen().ToString());
	Q.VerbRow(FVector2D(Origin.X + 20.f + VerbW, VerbY), VerbW - 8.f, TEXT("2"), EHolypawUiIcon::Lantern, HolypawUiCopy::AskWay().ToString());
	Q.VerbRow(FVector2D(Origin.X + 20.f + VerbW * 2.f, VerbY), VerbW - 8.f, TEXT("3"), EHolypawUiIcon::Check, HolypawUiCopy::TurnIn().ToString());
	Q.VerbRow(FVector2D(Origin.X + 20.f + VerbW * 3.f, VerbY), VerbW - 8.f, TEXT("4"), EHolypawUiIcon::Book, HolypawUiCopy::TakeJob().ToString());
	return Layer + 6;
}
