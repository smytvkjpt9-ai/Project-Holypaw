#include "UI/HolypawShopWidget.h"
#include "UI/HolypawUiTheme.h"
#include "Rendering/DrawElements.h"
#include "UI/HolypawUiCopy.h"
#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"
#include "HolypawCatalog.h"

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

	const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(860.f, 540.f), 36.f);
	const FVector2D Origin = HolypawUi::Centered(Size, Panel);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(24.f, 16.f), EHolypawUiIcon::Stall, HolypawUiCopy::ShopTitle().ToString(), Pal.Gold, Panel.X * 0.45f);

	const int32 CityHearts = Pawn->GetCityHearts(Pawn->CurrentZone);
	const bool bDiscount = CityHearts >= 1;
	const int32 AP = Pawn->Affection ? Pawn->Affection->AP : 0;
	TArray<HolypawUi::FChipSpec> Head;
	Head.Add({EHolypawUiIcon::MapPin, HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone), Pal.Powder});
	Head.Add({EHolypawUiIcon::Coin, HolypawUiCopy::ApN(AP).ToString(), Pal.Gold});
	Head.Add({EHolypawUiIcon::Heart,
		bDiscount ? HolypawUiCopy::ShopDiscount().ToString() : HolypawUiCopy::ShopFullPrice().ToString(),
		bDiscount ? Pal.Heart : Pal.Muted});
	const float HeadH = Q.ChipRow(Origin + FVector2D(24.f, 56.f), Panel.X - 48.f, Head, 30.f);
	Q.HeartRow(Origin + FVector2D(24.f, 56.f + HeadH + 6.f), CityHearts, 13.f, 5);

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

	const float GridTop = 56.f + HeadH + 28.f;
	const float GridW = (Panel.X - 56.f) * 0.5f;
	const float GridH = FMath::Max(120.f, (Panel.Y - GridTop - 56.f - 8.f) * 0.5f);
	for (int32 I = 0; I < Offers.Num(); ++I)
	{
		const FOffer& O = Offers[I];
		const int32 Price = Pawn->ShopPrice(O.Base);
		const bool bAfford = AP >= Price;
		const FVector2D Card = Origin + FVector2D(24.f + (I % 2) * (GridW + 8.f), GridTop + (I / 2) * (GridH + 8.f));
		Q.Fill(Card, FVector2D(GridW, GridH), bAfford ? Pal.Idle : HolypawUi::WithAlpha(Pal.Ink, 0.75f));
		Q.Frame(Card, FVector2D(GridW, GridH), bAfford ? Pal.Gold : Pal.Muted, 1.6f);
		Q.Keycap(Card + FVector2D(12.f, 12.f), FString::FromInt(O.Slot));
		Q.Icon(Card + FVector2D(16.f, 44.f), 36.f, O.Icon, bAfford ? HolypawUi::IconTint(O.Icon) : Pal.Muted);
		Q.Text(Card + FVector2D(64.f, 14.f), HolypawUi::Ellipsize(O.Name, GridW - 80.f, 0.95f), Pal.Cream, 0.95f, GridW - 76.f);
		Q.TextBlock(Card + FVector2D(64.f, 42.f), O.Blurb, Pal.Muted, 0.72f, GridW - 80.f, 2);
		const FString PriceS = (bDiscount && Price != O.Base)
			? HolypawUiCopy::PriceDrop(O.Base, Price).ToString()
			: HolypawUiCopy::PriceAp(Price).ToString();
		Q.Text(Card + FVector2D(64.f, GridH - 26.f), PriceS, bAfford ? Pal.Gold : Pal.Muted, 0.85f, GridW * 0.5f);
		if (!bAfford)
		{
			Q.Text(Card + FVector2D(GridW * 0.52f, GridH - 26.f), HolypawUiCopy::NeedAp(Price - AP).ToString(), Pal.Rose, 0.75f, GridW * 0.44f);
		}
	}

	Q.Footer(Origin, Panel, HolypawUiCopy::ShopFooter().ToString());
	return Layer + 6;
}
