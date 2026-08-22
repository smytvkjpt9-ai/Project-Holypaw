#include "UI/HolypawShopWidget.h"
#include "UI/HolypawUiTheme.h"
#include "UI/HolypawUiCopy.h"
#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"
#include "HolypawTypes.h"

void UHolypawShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

int32 UHolypawShopWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(GetOwningPlayerPawn());
	if (!Pawn || !Pawn->IsShopOpen())
	{
		return Layer;
	}

	HolypawUi::FPaint Q{OutDrawElements, AllottedGeometry, Layer};
	const HolypawUi::FPalette& Pal = HolypawUi::Colors();
	const FVector2D Size = Q.Canvas();
	Q.Fill(FVector2D::ZeroVector, Size, HolypawUi::WithAlpha(Pal.Dim, 0.55f));

	const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(820.f, 520.f), 36.f);
	const FVector2D Origin = HolypawUi::Centered(Size, Panel);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(24.f, 18.f), EHolypawUiIcon::Stall, HolypawUiCopy::ShopTitle().ToString(), Pal.Gold);
	Q.Text(Origin + FVector2D(24.f, 54.f), HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone), Pal.Powder, 0.85f, 240.f);

	const int32 CityHearts = Pawn->GetCityHearts(Pawn->CurrentZone);
	const bool bDiscount = CityHearts >= 1;
	Q.Chip(Origin + FVector2D(Panel.X - 250.f, 18.f), FVector2D(220.f, 34.f), EHolypawUiIcon::Heart,
		bDiscount ? HolypawUiCopy::ShopDiscount().ToString() : HolypawUiCopy::ShopFullPrice().ToString(),
		bDiscount ? Pal.Heart : Pal.Muted);
	Q.HeartRow(Origin + FVector2D(Panel.X - 250.f, 58.f), CityHearts, 13.f, 5);

	const int32 AP = Pawn->Affection ? Pawn->Affection->AP : 0;
	Q.Chip(Origin + FVector2D(24.f, 78.f), FVector2D(140.f, 30.f), EHolypawUiIcon::Coin,
		FString::Printf(TEXT("AP %d"), AP), Pal.Gold);

	struct FOffer
	{
		int32 Slot;
		FString Name;
		FString Blurb;
		int32 Base;
		EHolypawUiIcon Icon;
	};
	TArray<FOffer> Offers;
	Offers.Add({1, HolypawUiCopy::FaithJar().ToString(), HolypawUiCopy::FaithJarBlurb().ToString(), 10, EHolypawUiIcon::Jar});
	if (const FHolypawItemDef* Bun = HolypawCatalog::FindItem(TEXT("stuffingBun")))
	{
		Offers.Add({2, Bun->DisplayName.ToString(), Bun->Description.ToString(), Bun->ShopCostAP, HolypawUi::IconForItem(Bun->Id)});
	}
	if (const FHolypawItemDef* Rib = HolypawCatalog::FindItem(TEXT("hymnRibbon")))
	{
		Offers.Add({3, Rib->DisplayName.ToString(), Rib->Description.ToString(), Rib->ShopCostAP, HolypawUi::IconForItem(Rib->Id)});
	}
	if (const FHolypawItemDef* Cocoa = HolypawCatalog::FindItem(TEXT("cocoaButton")))
	{
		Offers.Add({4, Cocoa->DisplayName.ToString(), Cocoa->Description.ToString(), Cocoa->ShopCostAP, HolypawUi::IconForItem(Cocoa->Id)});
	}

	const float GridW = (Panel.X - 56.f) * 0.5f;
	const float GridH = 150.f;
	for (int32 I = 0; I < Offers.Num(); ++I)
	{
		const FOffer& O = Offers[I];
		const int32 Price = Pawn->ShopPrice(O.Base);
		const bool bAfford = AP >= Price;
		const FVector2D Card = Origin + FVector2D(24.f + (I % 2) * (GridW + 8.f), 118.f + (I / 2) * (GridH + 8.f));
		Q.Fill(Card, FVector2D(GridW, GridH), bAfford ? Pal.Idle : HolypawUi::WithAlpha(Pal.Ink, 0.75f));
		Q.Frame(Card, FVector2D(GridW, GridH), bAfford ? Pal.Gold : Pal.Muted, 1.6f);
		Q.Keycap(Card + FVector2D(12.f, 12.f), FString::FromInt(O.Slot));
		Q.Icon(Card + FVector2D(GridW * 0.5f - 22.f, 16.f), 40.f, O.Icon, HolypawUi::IconTint(O.Icon));
		Q.Text(Card + FVector2D(14.f, 64.f), O.Name, Pal.Cream, 0.95f, GridW - 28.f);
		Q.TextBlock(Card + FVector2D(14.f, 88.f), O.Blurb, Pal.Muted, 0.72f, GridW - 28.f, 2);
		if (bDiscount && Price != O.Base)
		{
			Q.Text(Card + FVector2D(14.f, GridH - 24.f), FString::Printf(TEXT("%d → %d AP"), O.Base, Price), Pal.Gold, 0.85f, GridW - 28.f);
		}
		else
		{
			Q.Text(Card + FVector2D(14.f, GridH - 24.f), FString::Printf(TEXT("%d AP"), Price), Pal.Gold, 0.85f, GridW - 28.f);
		}
	}

	Q.Footer(Origin, Panel, HolypawUiCopy::ShopFooter().ToString());
	return Layer + 6;
}
