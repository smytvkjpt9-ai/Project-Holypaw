#include "UI/HolypawPlayHudWidget.h"
#include "UI/HolypawUiTheme.h"
#include "Rendering/DrawElements.h"
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
			Q.Toast(Size, 16.f, Pawn->GetToast());
		}
		return Layer + 4;
	}

	const FString Clock = Pawn->GetClockLine();
	const float ClockW = Clock.IsEmpty() ? 0.f : HolypawUi::TextWidth(Clock, 0.85f) + 16.f;
	const bool bWide = Size.X >= 1100.f;
	const bool bMid = Size.X >= 860.f;

	const FVector2D Bar(Size.X - 32.f, 48.f);
	const FVector2D BarPos(16.f, 14.f);
	Q.Fill(BarPos, Bar, Pal.Felt);
	Q.Frame(BarPos, Bar, Pal.Rose, 1.6f);

	float X = BarPos.X + 10.f;
	const float Right = BarPos.X + Bar.X - 10.f - ClockW;
	const FString Zone = HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone);
	const float ZoneW = FMath::Clamp(bWide ? 200.f : (bMid ? 150.f : 36.f), 36.f, Right - X - 280.f);
	Q.Icon(FVector2D(X, BarPos.Y + 13.f), 22.f, EHolypawUiIcon::MapPin, Pal.Powder);
	if (ZoneW > 50.f)
	{
		Q.Text(FVector2D(X + 26.f, BarPos.Y + 14.f), HolypawUi::Ellipsize(Zone, ZoneW - 30.f, 0.9f), Pal.Cream, 0.9f, ZoneW - 26.f);
	}
	X += ZoneW + 8.f;

	const int32 Hearts = Pawn->Story ? Pawn->Story->Converts : 0;
	const int32 CityH = Pawn->GetCityHearts(Pawn->CurrentZone);
	auto Stat = [&](EHolypawUiIcon IconId, const FString& Value, FLinearColor Tint)
	{
		const float W = FMath::Clamp(28.f + HolypawUi::TextWidth(Value, 0.9f), 54.f, 90.f);
		if (X + W > Right)
		{
			return;
		}
		Q.Icon(FVector2D(X, BarPos.Y + 14.f), 20.f, IconId, Tint);
		Q.Text(FVector2D(X + 24.f, BarPos.Y + 14.f), Value, Pal.Cream, 0.9f, W - 24.f);
		X += W + 10.f;
	};
	Stat(EHolypawUiIcon::Heart, FString::FromInt(Hearts), Pal.Heart);
	Stat(EHolypawUiIcon::Coin, FString::FromInt(Pawn->Affection->AP), Pal.Gold);
	Stat(EHolypawUiIcon::Jar, FString::FromInt(Pawn->Affection->FP), Pal.Powder);
	if (bMid)
	{
		Stat(EHolypawUiIcon::Heart, FString::FromInt(CityH), HolypawUi::HeartsHeat(CityH));
	}

	const float HpW = FMath::Clamp(Right - X - 70.f, 70.f, 180.f);
	if (HpW >= 70.f)
	{
		Q.Meter(FVector2D(X, BarPos.Y + 18.f), FVector2D(HpW, 12.f),
			Pawn->HPMax > 0 ? float(Pawn->HP) / float(Pawn->HPMax) : 0.f,
			HolypawUiCopy::HpFrac(Pawn->HP, Pawn->HPMax).ToString(), Pal.HpFill);
	}

	if (!Clock.IsEmpty())
	{
		Q.Text(FVector2D(BarPos.X + Bar.X - ClockW, BarPos.Y + 14.f), Clock, Pal.Gold, 0.85f, ClockW);
	}
	const FString Faith = Pawn->GetFaithLine();
	if (!Faith.IsEmpty() && Size.X >= 860.f)
	{
		Q.Text(FVector2D(BarPos.X + 12.f, BarPos.Y + Bar.Y + 6.f), HolypawUi::Ellipsize(Faith, Size.X - 80.f, 0.75f), Pal.Rose, 0.75f, Size.X - 80.f);
	}

	const float Pct = Pawn->Affection->MiracleMax > 0.f ? Pawn->Affection->MiracleCharge / Pawn->Affection->MiracleMax : 0.f;
	Q.Icon(FVector2D(20.f, 72.f), 16.f, EHolypawUiIcon::Halo, Pal.Miracle);
	Q.Bar(FVector2D(42.f, 76.f), FVector2D(FMath::Min(220.f, Size.X * 0.22f), 8.f), Pct, Pal.Miracle);
	float InfoX = 42.f + FMath::Min(220.f, Size.X * 0.22f) + 16.f;
	if (Pawn->Story)
	{
		Q.Icon(FVector2D(InfoX, 70.f), 16.f, EHolypawUiIcon::Book, Pal.Gold);
		const float TitleW = FMath::Max(120.f, Size.X - InfoX - 200.f);
		Q.Text(FVector2D(InfoX + 20.f, 70.f), HolypawUi::Ellipsize(Pawn->Story->GetCurrent().Title.ToString(), TitleW, 0.8f), Pal.Gold, 0.8f, TitleW);
		InfoX += 28.f + FMath::Min(TitleW, 280.f);
	}
	if (!Pawn->GetCompassLine().IsEmpty() && Size.X - InfoX > 120.f)
	{
		Q.Text(FVector2D(InfoX + 8.f, 70.f), HolypawUi::Ellipsize(Pawn->GetCompassLine(), Size.X - InfoX - 40.f, 0.75f), Pal.Powder, 0.75f, Size.X - InfoX - 36.f);
	}
	const FString Faith = Pawn->GetFaithLine();
	if (!Faith.IsEmpty())
	{
		Q.Text(FVector2D(20.f, 92.f), HolypawUi::Ellipsize(Faith, Size.X * 0.42f, 0.72f), Pal.Heart, 0.72f, Size.X * 0.42f);
	}

	if (Pawn->GetToastAlpha() > 0.f)
	{
		Q.Toast(Size, 100.f, Pawn->GetToast());
	}

	if (!Pawn->GetPrompt().IsEmpty())
	{
		const FString Prompt = Pawn->GetPrompt().ToString();
		const float PromptW = FMath::Min(420.f, FMath::Max(180.f, 40.f + HolypawUi::TextWidth(Prompt, 0.85f)));
		Q.Chip(FVector2D((Size.X - PromptW) * 0.5f, Size.Y - 86.f), FVector2D(PromptW, 36.f),
			EHolypawUiIcon::Key, Prompt, Pal.Rose);
	}

	return Layer + 4;
}
