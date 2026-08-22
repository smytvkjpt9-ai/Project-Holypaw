#include "UI/HolypawMapWidget.h"
#include "UI/HolypawUiTheme.h"
#include "UI/HolypawUiCopy.h"
#include "Character/HolypawCharacter.h"
#include "HolypawWorldBuilder.h"
#include "HolypawTypes.h"
#include "EngineUtils.h"

void UHolypawMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

namespace
{
	FVector2D WorldToMap(FVector2D World, FVector2D Origin, FVector2D Size)
	{
		const float MinX = -110000.f;
		const float MaxX = 122000.f;
		const float MinY = -132000.f;
		const float MaxY = 108000.f;
		const float U = FMath::Clamp((World.X - MinX) / (MaxX - MinX), 0.f, 1.f);
		const float V = FMath::Clamp(1.f - (World.Y - MinY) / (MaxY - MinY), 0.f, 1.f);
		return Origin + FVector2D(U * Size.X, V * Size.Y);
	}
}

int32 UHolypawMapWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(GetOwningPlayerPawn());
	if (!Pawn || !Pawn->IsMapOpen())
	{
		return Layer;
	}

	HolypawUi::FPaint Q{OutDrawElements, AllottedGeometry, Layer};
	const HolypawUi::FPalette& Pal = HolypawUi::Colors();
	const FVector2D Size = Q.Canvas();
	Q.Fill(FVector2D::ZeroVector, Size, Pal.Dim);

	const FVector2D Panel(FMath::Min(Size.X - 48.f, 1180.f), FMath::Min(Size.Y - 48.f, 720.f));
	const FVector2D Origin((Size.X - Panel.X) * 0.5f, (Size.Y - Panel.Y) * 0.5f);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(28.f, 20.f), EHolypawUiIcon::Lantern, HolypawUiCopy::MapTitle().ToString(), Pal.Gold);
	Q.Text(Origin + FVector2D(28.f, 56.f), HolypawUiCopy::MapBlurb(), Pal.Powder, 0.85f, 520.f);

	const FVector2D AtlasPos = Origin + FVector2D(28.f, 88.f);
	const FVector2D AtlasSize(Panel.X - 360.f, Panel.Y - 140.f);
	Q.Fill(AtlasPos, AtlasSize, Pal.Sea);
	Q.DashRect(AtlasPos + FVector2D(4.f, 4.f), AtlasSize - FVector2D(8.f, 8.f), Pal.Gold, 1.4f, 8.f);

	for (const HolypawCatalog::FHolypawLandmass& Land : HolypawCatalog::GetLandmasses())
	{
		const FVector2D Center = WorldToMap(Land.Center, AtlasPos, AtlasSize);
		const float W = FMath::Clamp(Land.RadiusX / 230000.f * AtlasSize.X, 28.f, AtlasSize.X * 0.55f);
		const float H = FMath::Clamp(Land.RadiusY / 240000.f * AtlasSize.Y, 22.f, AtlasSize.Y * 0.45f);
		Q.Fill(Center - FVector2D(W * 0.5f, H * 0.5f), FVector2D(W, H), HolypawUi::WithAlpha(Pal.Mint, 0.28f));
	}

	for (const HolypawCatalog::FHolypawRoadLink& Road : HolypawCatalog::GetRoads())
	{
		const FHolypawCity A = HolypawCatalog::GetCity(Road.A);
		const FHolypawCity B = HolypawCatalog::GetCity(Road.B);
		Q.Line(WorldToMap(A.Pos, AtlasPos, AtlasSize), WorldToMap(B.Pos, AtlasPos, AtlasSize),
			HolypawUi::WithAlpha(Pal.Gold, 0.45f), 1.4f);
	}

	FVector2D Cottage(-32000.f, 800.f);
	FVector2D Peak(2000.f, 16000.f);
	if (UWorld* World = Pawn->GetWorld())
	{
		for (TActorIterator<AHolypawWorldBuilder> It(World); It; ++It)
		{
			Cottage = FVector2D(It->GetCottageSpawn().X, It->GetCottageSpawn().Y);
			Peak = It->GetPeakCenter();
			break;
		}
	}
	const FVector2D Home = WorldToMap(Cottage, AtlasPos, AtlasSize);
	const FVector2D PeakPt = WorldToMap(Peak, AtlasPos, AtlasSize);
	Q.Icon(Home - FVector2D(8.f, 8.f), 16.f, EHolypawUiIcon::Teddy, Pal.Rose);
	Q.Icon(PeakPt - FVector2D(8.f, 8.f), 16.f, EHolypawUiIcon::Halo, Pal.Gold);

	const EHolypawZone Selected = Pawn->GetSelectedTravel();
	FHolypawCity Focus = HolypawCatalog::GetCity(EHolypawZone::RibbonCity);
	bool bFocusCity = false;
	for (const FHolypawCity& City : HolypawCatalog::GetCities())
	{
		const int32 Hearts = Pawn->GetCityHearts(City.Zone);
		const FVector2D Pt = WorldToMap(City.Pos, AtlasPos, AtlasSize);
		const float Pin = 9.f + FMath::Min(Hearts, 5) * 2.4f;
		Q.Icon(Pt - FVector2D(Pin * 0.5f, Pin * 0.5f), Pin, EHolypawUiIcon::Heart, HolypawUi::HeartsHeat(Hearts));
		if (City.Zone == Selected)
		{
			Q.DashRect(Pt - FVector2D(14.f, 14.f), FVector2D(28.f, 28.f), Pal.Gold, 1.8f, 4.f);
			Focus = City;
			bFocusCity = true;
		}
		if (City.Zone == Pawn->CurrentZone)
		{
			Q.Icon(Pt + FVector2D(6.f, -10.f), 14.f, EHolypawUiIcon::Paw, Pal.Cream);
		}
	}

	const FVector2D You = WorldToMap(FVector2D(Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y), AtlasPos, AtlasSize);
	Q.Icon(You - FVector2D(10.f, 10.f), 20.f, EHolypawUiIcon::Paw, Pal.Cream);

	const FVector2D Side(Origin.X + Panel.X - 316.f, Origin.Y + 88.f);
	Q.Fill(Side, FVector2D(288.f, Panel.Y - 140.f), Pal.FeltInner);
	Q.DashRect(Side + FVector2D(6.f, 6.f), FVector2D(276.f, Panel.Y - 152.f), Pal.Rose, 1.3f, 6.f);

	const TCHAR* FocusName = bFocusCity ? *Focus.DisplayName.ToString() : HolypawCatalog::ZoneDisplayName(Selected);
	Q.Icon(Side + FVector2D(16.f, 16.f), 24.f, EHolypawUiIcon::Lantern, Pal.Gold);
	Q.Text(Side + FVector2D(48.f, 18.f), FocusName, Pal.Cream, 1.1f, 220.f);
	if (bFocusCity)
	{
		Q.Text(Side + FVector2D(16.f, 50.f), Focus.Continent, Pal.Mint, 0.85f, 250.f);
		Q.Text(Side + FVector2D(16.f, 74.f), Focus.Flavor, Pal.Muted, 0.75f, 250.f);
	}
	const int32 FocusHearts = Pawn->GetCityHearts(Selected);
	Q.Text(Side + FVector2D(16.f, 128.f), HolypawUiCopy::MapHearts(), Pal.Gold, 0.85f, 200.f);
	Q.HeartRow(Side + FVector2D(16.f, 154.f), FocusHearts, 16.f, 6);

	const bool bLit = Pawn->GetUnlockedTravel().Contains(Selected);
	Q.Text(Side + FVector2D(16.f, 186.f),
		bLit ? HolypawUiCopy::MapHop() : HolypawUiCopy::MapUnlit(),
		bLit ? Pal.Mint : Pal.Muted, 0.8f, 250.f);

	Q.Text(Side + FVector2D(16.f, 220.f), HolypawUiCopy::MapHere(), Pal.Powder, 0.8f, 250.f);
	Q.Text(Side + FVector2D(16.f, 242.f), HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone), Pal.Cream, 0.95f, 250.f);
	Q.HeartRow(Side + FVector2D(16.f, 268.f), Pawn->GetCityHearts(Pawn->CurrentZone), 14.f, 5);

	Q.Text(Side + FVector2D(16.f, 308.f), TEXT("Lanterns"), Pal.Gold, 0.85f, 200.f);
	const TArray<EHolypawZone>& Travel = Pawn->GetUnlockedTravel();
	const int32 Show = FMath::Min(8, Travel.Num());
	for (int32 I = 0; I < Show; ++I)
	{
		const bool Sel = Travel[I] == Selected;
		Q.Text(Side + FVector2D(16.f, 332.f + I * 22.f),
			FString::Printf(TEXT("%s %s"), Sel ? TEXT(">") : TEXT(" "), HolypawCatalog::ZoneDisplayName(Travel[I])),
			Sel ? Pal.Gold : Pal.Cream, 0.8f, 250.f);
	}

	Q.Footer(Origin, Panel, TEXT("Hearts heat the pins. Gold ring is the selected lantern."));
	return Layer + 6;
}
