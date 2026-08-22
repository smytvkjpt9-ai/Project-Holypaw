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

	Q.Chip(FVector2D(20.f, 16.f), FVector2D(280.f, 36.f), EHolypawUiIcon::MapPin,
		HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone), Pal.Powder);
	Q.Chip(FVector2D(308.f, 16.f), FVector2D(240.f, 36.f), EHolypawUiIcon::Teddy,
		HolypawUiCopy::PlayTitle().ToString(), Pal.Rose);

	if (!Pawn->GetClockLine().IsEmpty())
	{
		Q.Chip(FVector2D(Size.X - 220.f, 16.f), FVector2D(200.f, 36.f), EHolypawUiIcon::Halo,
			Pawn->GetClockLine(), Pal.Gold);
	}

	const int32 Hearts = Pawn->Story ? Pawn->Story->Converts : 0;
	const int32 CityH = Pawn->GetCityHearts(Pawn->CurrentZone);
	Q.Chip(FVector2D(20.f, 60.f), FVector2D(150.f, 34.f), EHolypawUiIcon::Heart,
		FString::Printf(TEXT("%s %d"), *HolypawUiCopy::Hearts().ToString(), Hearts), Pal.Heart);
	Q.Chip(FVector2D(178.f, 60.f), FVector2D(130.f, 34.f), EHolypawUiIcon::Coin,
		FString::Printf(TEXT("AP %d"), Pawn->Affection->AP), Pal.Gold);
	Q.Chip(FVector2D(316.f, 60.f), FVector2D(130.f, 34.f), EHolypawUiIcon::Jar,
		FString::Printf(TEXT("FP %d"), Pawn->Affection->FP), Pal.Powder);
	Q.Chip(FVector2D(454.f, 60.f), FVector2D(160.f, 34.f), EHolypawUiIcon::Health,
		FString::Printf(TEXT("HP %d/%d"), Pawn->HP, Pawn->HPMax), Pal.HpFill);
	Q.Chip(FVector2D(622.f, 60.f), FVector2D(150.f, 34.f), EHolypawUiIcon::Heart,
		FString::Printf(TEXT("%s %d"), *HolypawUiCopy::CityHearts().ToString(), CityH), HolypawUi::HeartsHeat(CityH));

	const float Pct = Pawn->Affection->MiracleMax > 0.f ? Pawn->Affection->MiracleCharge / Pawn->Affection->MiracleMax : 0.f;
	Q.Icon(FVector2D(20.f, 104.f), 20.f, EHolypawUiIcon::Halo, Pal.Miracle);
	Q.Text(FVector2D(46.f, 104.f),
		FString::Printf(TEXT("%s  %d / %d"), *HolypawUiCopy::Miracle().ToString(),
			FMath::FloorToInt(Pawn->Affection->MiracleCharge), FMath::FloorToInt(Pawn->Affection->MiracleMax)),
		Pal.Gold, 0.85f, 280.f);
	Q.Bar(FVector2D(20.f, 128.f), FVector2D(280.f, 12.f), Pct, Pal.Miracle);

	const bool bBusyOverlay = Pawn->IsMapOpen() || Pawn->IsJournalOpen() || Pawn->IsTalkOpen() || Pawn->IsShopOpen()
		|| Pawn->IsSkillsOpen() || Pawn->IsPartyOpen() || Pawn->IsCodexOpen() || Pawn->IsInventoryOpen()
		|| Pawn->Mode == EHolypawPawnMode::Battle;
	if (Pawn->Mode == EHolypawPawnMode::Play && !bBusyOverlay)
	{
		if (!Pawn->GetCompassLine().IsEmpty())
		{
			Q.Chip(FVector2D(20.f, 150.f), FVector2D(420.f, 32.f), EHolypawUiIcon::Lantern, Pawn->GetCompassLine(), Pal.Powder);
		}
		if (Pawn->Story)
		{
			Q.Chip(FVector2D(20.f, 188.f), FVector2D(420.f, 32.f), EHolypawUiIcon::Book,
				Pawn->Story->GetCurrent().Title.ToString(), Pal.Gold);
		}
	}

	if (!Pawn->GetPrompt().IsEmpty())
	{
		const FString Prompt = Pawn->GetPrompt().ToString();
		Q.Chip(FVector2D(Size.X * 0.5f - 220.f, Size.Y - 128.f), FVector2D(440.f, 40.f),
			EHolypawUiIcon::Key, Prompt, Pal.Rose);
	}

	if (Pawn->GetToastAlpha() > 0.f)
	{
		Q.Fill(FVector2D(Size.X * 0.5f - 280.f, 210.f), FVector2D(560.f, 44.f), Pal.Felt);
		Q.DashRect(FVector2D(Size.X * 0.5f - 274.f, 216.f), FVector2D(548.f, 32.f), Pal.Gold, 1.3f, 6.f);
		Q.Icon(FVector2D(Size.X * 0.5f - 262.f, 218.f), 22.f, EHolypawUiIcon::Halo, Pal.Gold);
		Q.Text(FVector2D(Size.X * 0.5f - 232.f, 220.f), Pawn->GetToast(), Pal.Cream, 0.95f, 500.f);
	}

	if (Pawn->Mode == EHolypawPawnMode::Play && !bBusyOverlay)
	{
		Q.Fill(FVector2D(Size.X - 640.f, Size.Y - 40.f), FVector2D(620.f, 28.f), HolypawUi::WithAlpha(Pal.Felt, 0.7f));
		Q.Text(FVector2D(Size.X - 628.f, Size.Y - 36.f), HolypawUiCopy::Legend(), Pal.Muted, 0.7f, 600.f);
	}

	return Layer + 4;
}
