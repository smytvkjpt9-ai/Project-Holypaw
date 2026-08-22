#include "UI/HolypawPlayHudWidget.h"
#include "UI/HolypawUiTheme.h"
#include "UI/HolypawUiCopy.h"
#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"
#include "Components/MissionComponent.h"
#include "HolypawTypes.h"

void UHolypawPlayHudWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

int32 UHolypawPlayHudWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(GetOwningPlayerPawn());
	if (!Pawn || !Pawn->Affection || Pawn->Mode == EHolypawPawnMode::Title || Pawn->Mode == EHolypawPawnMode::Pause)
	{
		return Layer;
	}

	HolypawUi::FPaint Q{OutDrawElements, AllottedGeometry, Layer};
	const HolypawUi::FPalette& Pal = HolypawUi::Colors();
	const FVector2D Size = Q.Canvas();

	const bool bBusyOverlay = Pawn->IsMapOpen() || Pawn->IsJournalOpen() || Pawn->IsTalkOpen() || Pawn->IsShopOpen()
		|| Pawn->IsSkillsOpen() || Pawn->IsPartyOpen() || Pawn->IsCodexOpen() || Pawn->IsInventoryOpen();
	if (Pawn->Mode == EHolypawPawnMode::Battle || bBusyOverlay)
	{
		if (Pawn->GetToastAlpha() > 0.f)
		{
			Q.Fill(FVector2D(Size.X * 0.5f - 260.f, 16.f), FVector2D(520.f, 36.f), Pal.Felt);
			Q.Frame(FVector2D(Size.X * 0.5f - 260.f, 16.f), FVector2D(520.f, 36.f), Pal.Gold, 1.4f);
			Q.Icon(FVector2D(Size.X * 0.5f - 248.f, 20.f), 20.f, EHolypawUiIcon::Halo, Pal.Gold);
			Q.Text(FVector2D(Size.X * 0.5f - 220.f, 22.f), HolypawUi::Ellipsize(Pawn->GetToast(), 480.f, 0.85f), Pal.Cream, 0.85f, 480.f);
		}
		return Layer + 4;
	}

	const FVector2D Bar(Size.X - 32.f, 56.f);
	const FVector2D BarPos(16.f, 14.f);
	Q.Fill(BarPos, Bar, Pal.Felt);
	Q.Frame(BarPos, Bar, Pal.Rose, 1.6f);

	Q.Icon(BarPos + FVector2D(10.f, 16.f), 22.f, EHolypawUiIcon::MapPin, Pal.Powder);
	Q.Text(BarPos + FVector2D(36.f, 16.f), HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone), Pal.Cream, 0.95f, 180.f);

	const int32 Hearts = Pawn->Story ? Pawn->Story->Converts : 0;
	const int32 CityH = Pawn->GetCityHearts(Pawn->CurrentZone);
	float X = BarPos.X + 230.f;
	Q.Icon(FVector2D(X, BarPos.Y + 16.f), 20.f, EHolypawUiIcon::Heart, Pal.Heart);
	Q.Text(FVector2D(X + 24.f, BarPos.Y + 16.f), FString::FromInt(Hearts), Pal.Cream, 0.95f, 50.f);
	X += 78.f;
	Q.Icon(FVector2D(X, BarPos.Y + 16.f), 20.f, EHolypawUiIcon::Coin, Pal.Gold);
	Q.Text(FVector2D(X + 24.f, BarPos.Y + 16.f), FString::FromInt(Pawn->Affection->AP), Pal.Cream, 0.95f, 50.f);
	X += 78.f;
	Q.Icon(FVector2D(X, BarPos.Y + 16.f), 20.f, EHolypawUiIcon::Jar, Pal.Powder);
	Q.Text(FVector2D(X + 24.f, BarPos.Y + 16.f), FString::FromInt(Pawn->Affection->FP), Pal.Cream, 0.95f, 50.f);
	X += 86.f;
	Q.Bar(FVector2D(X, BarPos.Y + 20.f), FVector2D(160.f, 12.f),
		Pawn->HPMax > 0 ? float(Pawn->HP) / float(Pawn->HPMax) : 0.f, Pal.HpFill);
	Q.Text(FVector2D(X + 168.f, BarPos.Y + 16.f), FString::Printf(TEXT("%d/%d"), Pawn->HP, Pawn->HPMax), Pal.Cream, 0.8f, 80.f);
	X += 260.f;
	Q.Icon(FVector2D(X, BarPos.Y + 16.f), 18.f, EHolypawUiIcon::Heart, HolypawUi::HeartsHeat(CityH));
	Q.Text(FVector2D(X + 22.f, BarPos.Y + 16.f), FString::Printf(TEXT("%s %d"), *HolypawUiCopy::CityHearts().ToString(), CityH), Pal.Cream, 0.8f, 80.f);

	if (!Pawn->GetClockLine().IsEmpty())
	{
		Q.Text(BarPos + FVector2D(Bar.X - 150.f, 16.f), Pawn->GetClockLine(), Pal.Gold, 0.85f, 140.f);
	}

	const float Pct = Pawn->Affection->MiracleMax > 0.f ? Pawn->Affection->MiracleCharge / Pawn->Affection->MiracleMax : 0.f;
	Q.Icon(FVector2D(20.f, 78.f), 16.f, EHolypawUiIcon::Halo, Pal.Miracle);
	Q.Bar(FVector2D(42.f, 82.f), FVector2D(220.f, 8.f), Pct, Pal.Miracle);
	if (Pawn->Story)
	{
		Q.Icon(FVector2D(276.f, 76.f), 16.f, EHolypawUiIcon::Book, Pal.Gold);
		Q.Text(FVector2D(296.f, 76.f), HolypawUi::Ellipsize(Pawn->Story->GetCurrent().Title.ToString(), 280.f, 0.8f), Pal.Gold, 0.8f, 280.f);
	}
	if (!Pawn->GetCompassLine().IsEmpty())
	{
		Q.Text(FVector2D(20.f, 98.f), HolypawUi::Ellipsize(Pawn->GetCompassLine(), 420.f, 0.75f), Pal.Powder, 0.75f, 420.f);
	}

	if (Pawn->GetToastAlpha() > 0.f)
	{
		Q.Fill(FVector2D(Size.X * 0.5f - 260.f, 118.f), FVector2D(520.f, 36.f), Pal.Felt);
		Q.Frame(FVector2D(Size.X * 0.5f - 260.f, 118.f), FVector2D(520.f, 36.f), Pal.Gold, 1.4f);
		Q.Icon(FVector2D(Size.X * 0.5f - 248.f, 122.f), 20.f, EHolypawUiIcon::Halo, Pal.Gold);
		Q.Text(FVector2D(Size.X * 0.5f - 220.f, 124.f), HolypawUi::Ellipsize(Pawn->GetToast(), 480.f, 0.85f), Pal.Cream, 0.85f, 480.f);
	}

	if (!Pawn->GetPrompt().IsEmpty())
	{
		const FString Prompt = Pawn->GetPrompt().ToString();
		Q.Chip(FVector2D(Size.X * 0.5f - 200.f, Size.Y - 86.f), FVector2D(400.f, 36.f),
			EHolypawUiIcon::Key, Prompt, Pal.Rose);
	}

	return Layer + 4;
}
