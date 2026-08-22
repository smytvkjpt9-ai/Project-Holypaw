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

	FLinearColor LandTint(const TCHAR* Name)
	{
		const HolypawUi::FPalette& P = HolypawUi::Colors();
		const FString S(Name);
		if (S.Contains(TEXT("Home")) || S.Contains(TEXT("Canyon")))
		{
			return HolypawUi::WithAlpha(P.Mint, 0.38f);
		}
		if (S.Contains(TEXT("Old")) || S.Contains(TEXT("Silk")) || S.Contains(TEXT("Cherry")))
		{
			return HolypawUi::WithAlpha(P.Rose, 0.32f);
		}
		if (S.Contains(TEXT("Sand")) || S.Contains(TEXT("Cape")) || S.Contains(TEXT("South")))
		{
			return HolypawUi::WithAlpha(P.Gold, 0.28f);
		}
		if (S.Contains(TEXT("Felt")) || S.Contains(TEXT("Coral")))
		{
			return HolypawUi::WithAlpha(P.Powder, 0.40f);
		}
		return HolypawUi::WithAlpha(P.Mint, 0.26f);
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

	const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(1180.f, 720.f), 28.f);
	const FVector2D Origin = HolypawUi::Centered(Size, Panel);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(24.f, 16.f), EHolypawUiIcon::Lantern, HolypawUiCopy::MapTitle().ToString(), Pal.Gold, Panel.X - 56.f);

	const float SideW = FMath::Clamp(Panel.X * 0.28f, 240.f, 300.f);
	const FVector2D AtlasPos = Origin + FVector2D(24.f, 62.f);
	const FVector2D AtlasSize(Panel.X - SideW - 48.f, Panel.Y - 108.f);
	Q.Fill(AtlasPos, AtlasSize, Pal.Sea);
	Q.Frame(AtlasPos, AtlasSize, Pal.Gold, 2.f);

	for (const HolypawCatalog::FHolypawLandmass& Land : HolypawCatalog::GetLandmasses())
	{
		const FVector2D Center = WorldToMap(Land.Center, AtlasPos, AtlasSize);
		const float W = FMath::Clamp(Land.RadiusX / 230000.f * AtlasSize.X, 28.f, AtlasSize.X * 0.55f);
		const float H = FMath::Clamp(Land.RadiusY / 240000.f * AtlasSize.Y, 22.f, AtlasSize.Y * 0.45f);
		const FLinearColor Tint = LandTint(Land.Name);
		Q.Fill(Center - FVector2D(W * 0.5f, H * 0.5f), FVector2D(W, H), Tint);
		Q.Fill(Center - FVector2D(W * 0.32f, H * 0.32f), FVector2D(W * 0.64f, H * 0.64f), HolypawUi::WithAlpha(Tint, Tint.A + 0.08f));
	}

	for (const HolypawCatalog::FHolypawRoadLink& Road : HolypawCatalog::GetRoads())
	{
		const FHolypawCity A = HolypawCatalog::GetCity(Road.A);
		const FHolypawCity B = HolypawCatalog::GetCity(Road.B);
		Q.Line(WorldToMap(A.Pos, AtlasPos, AtlasSize), WorldToMap(B.Pos, AtlasPos, AtlasSize),
			HolypawUi::WithAlpha(Pal.Gold, 0.40f), 1.3f);
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
	Q.Text(Home + FVector2D(10.f, -8.f), HolypawUiCopy::HomePin(), Pal.Rose, 0.68f, 70.f);
	Q.Icon(PeakPt - FVector2D(8.f, 8.f), 16.f, EHolypawUiIcon::Halo, Pal.Gold);
	Q.Text(PeakPt + FVector2D(10.f, -8.f), HolypawUiCopy::PeakPin(), Pal.Gold, 0.68f, 70.f);

	const EHolypawZone Selected = Pawn->GetSelectedTravel();
	FHolypawCity Focus;
	bool bFocusCity = false;
	for (const FHolypawCity& City : HolypawCatalog::GetCities())
	{
		if (City.Zone == Selected)
		{
			Focus = City;
			bFocusCity = true;
		}
	}

	for (const FHolypawCity& City : HolypawCatalog::GetCities())
	{
		const int32 Hearts = Pawn->GetCityHearts(City.Zone);
		const FVector2D Pt = WorldToMap(City.Pos, AtlasPos, AtlasSize);
		const bool bSel = City.Zone == Selected;
		const bool bHere = City.Zone == Pawn->CurrentZone;
		const float Pin = bSel ? 18.f : (9.f + FMath::Min(Hearts, 4) * 2.0f);
		Q.Icon(Pt - FVector2D(Pin * 0.5f, Pin * 0.5f), Pin, EHolypawUiIcon::Heart, HolypawUi::HeartsHeat(Hearts));
		if (bSel)
		{
			Q.Frame(Pt - FVector2D(12.f, 12.f), FVector2D(24.f, 24.f), Pal.Gold, 2.f);
		}
		if (bHere || bSel || Hearts > 0)
		{
			Q.Text(Pt + FVector2D(8.f, 8.f), HolypawUi::Ellipsize(City.DisplayName.ToString(), 90.f, 0.62f),
				bSel ? Pal.Gold : Pal.Cream, 0.62f, 100.f);
		}
	}

	const FVector2D You = WorldToMap(FVector2D(Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y), AtlasPos, AtlasSize);
	Q.Frame(You - FVector2D(12.f, 12.f), FVector2D(24.f, 24.f), Pal.Cream, 1.6f);
	Q.Icon(You - FVector2D(9.f, 9.f), 18.f, EHolypawUiIcon::Paw, Pal.Cream);

	TArray<HolypawUi::FChipSpec> Heat;
	Heat.Add({EHolypawUiIcon::Heart, HolypawUiCopy::HeatCold().ToString(), HolypawUi::HeartsHeat(0)});
	Heat.Add({EHolypawUiIcon::Heart, HolypawUiCopy::HeatWarm().ToString(), HolypawUi::HeartsHeat(1)});
	Heat.Add({EHolypawUiIcon::Heart, HolypawUiCopy::HeatGlow().ToString(), HolypawUi::HeartsHeat(3)});
	Q.ChipRow(AtlasPos + FVector2D(8.f, AtlasSize.Y - 36.f), AtlasSize.X - 16.f, Heat, 26.f);

	const FVector2D Side(Origin.X + Panel.X - SideW - 16.f, Origin.Y + 62.f);
	const FVector2D SideSize(SideW, Panel.Y - 108.f);
	Q.Fill(Side, SideSize, Pal.FeltInner);
	Q.Frame(Side, SideSize, Pal.Rose, 1.6f);

	const FString FocusName = bFocusCity ? Focus.DisplayName.ToString() : HolypawCatalog::ZoneDisplayName(Selected);
	Q.Icon(Side + FVector2D(14.f, 14.f), 22.f, EHolypawUiIcon::Lantern, Pal.Gold);
	Q.Text(Side + FVector2D(42.f, 16.f), HolypawUi::Ellipsize(FocusName, SideW - 60.f, 1.0f), Pal.Cream, 1.0f, SideW - 56.f);
	float SideY = 44.f;
	if (bFocusCity)
	{
		Q.Text(Side + FVector2D(14.f, SideY), Focus.Continent, Pal.Mint, 0.8f, SideW - 28.f);
		SideY += 20.f;
		SideY += Q.TextBlock(Side + FVector2D(14.f, SideY), Focus.Flavor.ToString(), Pal.Muted, 0.72f, SideW - 28.f, 3);
	}
	else
	{
		Q.Text(Side + FVector2D(14.f, SideY), HolypawUiCopy::HomeSewn(), Pal.Mint, 0.8f, SideW - 28.f);
		SideY += 22.f;
	}

	const int32 FocusHearts = Pawn->GetCityHearts(Selected);
	Q.Text(Side + FVector2D(14.f, SideY + 6.f), HolypawUiCopy::MapHearts(), Pal.Gold, 0.8f, SideW - 28.f);
	Q.HeartRow(Side + FVector2D(14.f, SideY + 26.f), FocusHearts, 15.f, 5);
	const bool bLit = Pawn->GetUnlockedTravel().Contains(Selected) || Selected == EHolypawZone::ForestCottage;
	Q.Text(Side + FVector2D(14.f, SideY + 50.f), bLit ? HolypawUiCopy::MapHop() : HolypawUiCopy::MapUnlit(),
		bLit ? Pal.Mint : Pal.Muted, 0.75f, SideW - 28.f);

	Q.Text(Side + FVector2D(14.f, SideY + 74.f), HolypawUiCopy::MapHere(), Pal.Powder, 0.75f, SideW - 28.f);
	Q.Text(Side + FVector2D(14.f, SideY + 94.f), HolypawUi::Ellipsize(HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone), SideW - 28.f, 0.9f), Pal.Cream, 0.9f, SideW - 28.f);

	Q.Text(Side + FVector2D(14.f, SideY + 122.f), HolypawUiCopy::Lanterns(), Pal.Gold, 0.8f, SideW - 28.f);
	const TArray<EHolypawZone>& Travel = Pawn->GetUnlockedTravel();
	const float ListTop = SideY + 144.f;
	const int32 Room = FMath::Max(3, FMath::FloorToInt((SideSize.Y - ListTop - 8.f) / 26.f));
	const int32 Show = FMath::Min(Room, Travel.Num());
	for (int32 I = 0; I < Show; ++I)
	{
		const bool Sel = Travel[I] == Selected;
		const FVector2D Row = Side + FVector2D(10.f, ListTop + I * 26.f);
		const FVector2D RowSize(SideW - 20.f, 24.f);
		if (Sel)
		{
			Q.Fill(Row, RowSize, Pal.Select);
			Q.Frame(Row, RowSize, Pal.Gold, 1.3f);
		}
		Q.Icon(Row + FVector2D(4.f, 4.f), 16.f, EHolypawUiIcon::Lantern, Sel ? Pal.Gold : Pal.Powder);
		Q.Text(Row + FVector2D(24.f, 4.f),
			HolypawUi::Ellipsize(HolypawCatalog::ZoneDisplayName(Travel[I]), SideW - 70.f, 0.75f),
			Sel ? Pal.Gold : Pal.Cream, 0.75f, SideW - 68.f);
		const int32 H = Pawn->GetCityHearts(Travel[I]);
		if (H > 0)
		{
			Q.Icon(Row + FVector2D(RowSize.X - 28.f, 5.f), 12.f, EHolypawUiIcon::Heart, HolypawUi::HeartsHeat(H));
			Q.Text(Row + FVector2D(RowSize.X - 14.f, 4.f), FString::FromInt(H), Pal.Cream, 0.7f, 16.f);
		}
	}

	Q.Footer(Origin, Panel, HolypawUiCopy::MapBlurb().ToString());
	return Layer + 6;
}
