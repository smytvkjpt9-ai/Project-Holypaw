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

	const FVector2D Panel(760.f, 520.f);
	const FVector2D Origin = HolypawUi::Centered(Size, Panel);
	Q.Panel(Origin, Panel);
	Q.Caption(Origin + FVector2D(28.f, 22.f), EHolypawUiIcon::Stall, HolypawUiCopy::ShopTitle().ToString(), Pal.Gold);
	Q.Text(Origin + FVector2D(28.f, 58.f), HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone), Pal.Powder, 0.9f, 300.f);

	const int32 CityHearts = Pawn->GetCityHearts(Pawn->CurrentZone);
	const bool bDiscount = CityHearts >= 1;
	Q.Chip(Origin + FVector2D(Panel.X - 280.f, 24.f), FVector2D(248.f, 40.f), EHolypawUiIcon::Heart,
		bDiscount ? HolypawUiCopy::ShopDiscount().ToString() : HolypawUiCopy::ShopFullPrice().ToString(),
		bDiscount ? Pal.Heart : Pal.Muted);
	Q.HeartRow(Origin + FVector2D(Panel.X - 280.f, 70.f), CityHearts, 14.f, 5);

	const int32 AP = Pawn->Affection ? Pawn->Affection->AP : 0;
	Q.Chip(Origin + FVector2D(28.f, 86.f), FVector2D(160.f, 36.f), EHolypawUiIcon::Coin,
		FString::Printf(TEXT("AP %d"), AP), Pal.Gold);

	struct FOffer
	{
		int32 Slot;
		FName Item;
		FString Name;
		FString Blurb;
		int32 Base;
		EHolypawUiIcon Icon;
	};
	TArray<FOffer> Offers;
	Offers.Add({1, NAME_None, HolypawUiCopy::FaithJar().ToString(), HolypawUiCopy::FaithJarBlurb().ToString(), 10, EHolypawUiIcon::Jar});
	if (const FHolypawItemDef* Bun = HolypawCatalog::FindItem(TEXT("stuffingBun")))
	{
		Offers.Add({2, Bun->Id, Bun->DisplayName.ToString(), Bun->Description.ToString(), Bun->ShopCostAP, HolypawUi::IconForItem(Bun->Id)});
	}
	if (const FHolypawItemDef* Rib = HolypawCatalog::FindItem(TEXT("hymnRibbon")))
	{
		Offers.Add({3, Rib->Id, Rib->DisplayName.ToString(), Rib->Description.ToString(), Rib->ShopCostAP, HolypawUi::IconForItem(Rib->Id)});
	}
	if (const FHolypawItemDef* Cocoa = HolypawCatalog::FindItem(TEXT("cocoaButton")))
	{
		Offers.Add({4, Cocoa->Id, Cocoa->DisplayName.ToString(), Cocoa->Description.ToString(), Cocoa->ShopCostAP, HolypawUi::IconForItem(Cocoa->Id)});
	}

	for (int32 I = 0; I < Offers.Num(); ++I)
	{
		const FOffer& O = Offers[I];
		const int32 Price = Pawn->ShopPrice(O.Base);
		const bool bAfford = AP >= Price;
		const FVector2D Card = Origin + FVector2D(28.f, 136.f + I * 80.f);
		Q.Fill(Card, FVector2D(704.f, 72.f), bAfford ? Pal.Idle : HolypawUi::WithAlpha(Pal.Ink, 0.7f));
		Q.DashRect(Card + FVector2D(4.f, 4.f), FVector2D(696.f, 64.f), bAfford ? Pal.Gold : Pal.Muted, 1.3f, 6.f);
		Q.Keycap(Card + FVector2D(12.f, 24.f), FString::FromInt(O.Slot));
		Q.Icon(Card + FVector2D(50.f, 16.f), 40.f, O.Icon, HolypawUi::IconTint(O.Icon));
		Q.Text(Card + FVector2D(102.f, 12.f), O.Name, Pal.Cream, 1.05f, 360.f);
		Q.Text(Card + FVector2D(102.f, 40.f), O.Blurb, Pal.Muted, 0.8f, 420.f);
		Q.Icon(Card + FVector2D(540.f, 22.f), 22.f, EHolypawUiIcon::Coin, Pal.Gold);
		if (bDiscount && Price != O.Base)
		{
			Q.Text(Card + FVector2D(568.f, 12.f), FString::Printf(TEXT("%d"), O.Base), Pal.Muted, 0.75f, 80.f);
			Q.Text(Card + FVector2D(568.f, 34.f), FString::Printf(TEXT("%d AP"), Price), Pal.Gold, 1.0f, 110.f);
		}
		else
		{
			Q.Text(Card + FVector2D(568.f, 24.f), FString::Printf(TEXT("%d AP"), Price), Pal.Gold, 1.0f, 110.f);
		}
	}

	Q.Footer(Origin, Panel, HolypawUiCopy::ShopFooter().ToString());
	return Layer + 6;
}
