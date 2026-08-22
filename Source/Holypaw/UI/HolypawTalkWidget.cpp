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

	const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(1000.f, 360.f), 36.f);
	const FVector2D Origin((Size.X - Panel.X) * 0.5f, Size.Y - Panel.Y - 28.f);
	Q.Panel(Origin, Panel);

	const FString Speaker = Pawn->GetTalkSpeaker().IsEmpty() ? HolypawUiCopy::Believer().ToString() : Pawn->GetTalkSpeaker();
	const float NameW = FMath::Min(260.f, Panel.X * 0.34f);
	Q.Fill(Origin + FVector2D(20.f, 16.f), FVector2D(NameW, 40.f), Pal.Select);
	Q.Icon(Origin + FVector2D(28.f, 20.f), 26.f, EHolypawUiIcon::Speech, Pal.Powder);
	Q.Text(Origin + FVector2D(60.f, 22.f), HolypawUi::Ellipsize(Speaker, NameW - 48.f, 1.05f), Pal.Cream, 1.05f, NameW - 44.f);

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
		const float ErrandW = FMath::Min(260.f, Panel.X - NameW - 56.f);
		if (ErrandW > 110.f)
		{
			Q.Chip(Origin + FVector2D(Panel.X - ErrandW - 24.f, 16.f), FVector2D(ErrandW, 40.f), EHolypawUiIcon::Book, Errand, ErrandCol);
		}
	}

	const float VerbY = Origin.Y + Panel.Y - 70.f;
	const float BodyTop = Origin.Y + 68.f;
	FString Quiet;
	if (const FHolypawTalkDef* Talk = HolypawCatalog::FindTalk(Speaker))
	{
		if (!Talk->LineC.IsEmpty())
		{
			Quiet = Talk->FaithNeed > 0
				? HolypawUiCopy::QuietAt(Talk->FaithNeed).ToString()
				: HolypawUiCopy::QuietLine().ToString();
		}
	}
	const float QuietH = Quiet.IsEmpty() ? 0.f : 22.f;
	const float BodyH = FMath::Max(36.f, VerbY - BodyTop - QuietH - 10.f);
	const float Step = 18.f * 0.95f;
	const int32 Lines = FMath::Clamp(FMath::FloorToInt(BodyH / Step), 2, 6);
	const float Used = Q.TextBlock(FVector2D(Origin.X + 24.f, BodyTop), Pawn->GetTalkBody(), Pal.Cream, 0.95f, Panel.X - 48.f, Lines);
	if (!Quiet.IsEmpty())
	{
		Q.Text(FVector2D(Origin.X + 24.f, BodyTop + Used + 2.f), HolypawUi::Ellipsize(Quiet, Panel.X - 48.f, 0.75f), Pal.Muted, 0.75f, Panel.X - 48.f);
	}

	const float VerbW = (Panel.X - 56.f) / 4.f;
	Q.VerbRow(FVector2D(Origin.X + 20.f, VerbY), VerbW - 8.f, TEXT("1"), EHolypawUiIcon::Speech, HolypawUiCopy::Listen().ToString());
	Q.VerbRow(FVector2D(Origin.X + 20.f + VerbW, VerbY), VerbW - 8.f, TEXT("2"), EHolypawUiIcon::Lantern, HolypawUiCopy::AskWay().ToString());
	Q.VerbRow(FVector2D(Origin.X + 20.f + VerbW * 2.f, VerbY), VerbW - 8.f, TEXT("3"), EHolypawUiIcon::Check, HolypawUiCopy::TurnIn().ToString());
	Q.VerbRow(FVector2D(Origin.X + 20.f + VerbW * 3.f, VerbY), VerbW - 8.f, TEXT("4"), EHolypawUiIcon::Book, HolypawUiCopy::TakeJob().ToString());
	return Layer + 6;
}
