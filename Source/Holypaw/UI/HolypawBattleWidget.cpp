#include "UI/HolypawBattleWidget.h"
#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"
#include "Components/SkillTreeComponent.h"
#include "Components/PartyComponent.h"
#include "Actors/HostilePet.h"
#include "HolypawTypes.h"
#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"
#include "Layout/SlateLayoutTransform.h"
#include "Brushes/SlateColorBrush.h"

void UHolypawBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UHolypawBattleWidget::PaintText(FSlateWindowElementList& OutDrawElements, int32 LayerId, const FGeometry& AllottedGeometry,
	const FVector2D& Pos, const FString& Text, const FLinearColor& Color, float Scale) const
{
	FSlateFontInfo Font = FCoreStyle::GetDefaultFontStyle("Bold", FMath::Max(10, FMath::RoundToInt(16.f * Scale)));
	const FPaintGeometry Geo = AllottedGeometry.ToPaintGeometry(
		FVector2f(920.f, 40.f),
		FSlateLayoutTransform(FVector2f(Pos.X, Pos.Y)));
	FSlateDrawElement::MakeText(OutDrawElements, LayerId, Geo, Text, Font, ESlateDrawEffect::None, Color);
}

void UHolypawBattleWidget::PaintPanel(FSlateWindowElementList& OutDrawElements, int32 LayerId, const FGeometry& AllottedGeometry,
	const FVector2D& Pos, const FVector2D& Size, const FLinearColor& Color) const
{
	static FSlateColorBrush White(FLinearColor::White);
	const FPaintGeometry Geo = AllottedGeometry.ToPaintGeometry(
		FVector2f(Size.X, Size.Y),
		FSlateLayoutTransform(FVector2f(Pos.X, Pos.Y)));
	FSlateDrawElement::MakeBox(OutDrawElements, LayerId, Geo, &White, ESlateDrawEffect::None, Color);
}

int32 UHolypawBattleWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const AHolypawCharacter* P = Cast<AHolypawCharacter>(GetOwningPlayerPawn());
	if (!P)
	{
		return Layer;
	}

	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const float CX = Size.X * 0.5f;

	if (P->Mode == EHolypawPawnMode::Battle)
	{
		PaintPanel(OutDrawElements, Layer, AllottedGeometry, FVector2D(0.f, Size.Y * 0.10f), FVector2D(Size.X, Size.Y * 0.42f), FLinearColor(0.05f, 0.03f, 0.08f, 0.62f));
		PaintPanel(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(36.f, Size.Y * 0.14f), FVector2D(Size.X - 72.f, 168.f), FLinearColor(0.12f, 0.08f, 0.16f, 0.78f));
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 280.f, Size.Y * 0.16f), TEXT("YOU"), FLinearColor(1.f, 0.85f, 0.92f), 1.15f);
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 40.f, Size.Y * 0.16f), TEXT("VS"), FLinearColor(1.f, 0.9f, 0.5f), 1.4f);
		const FString EnemyName = P->GetBattleEnemy() ? P->GetBattleEnemy()->DisplayName.ToString() : TEXT("Hostile");
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX + 80.f, Size.Y * 0.16f), EnemyName, FLinearColor(1.f, 0.55f, 0.62f), 1.2f);
		if (P->GetDamagePopupTime() > 0.f)
		{
			PaintText(OutDrawElements, Layer + 3, AllottedGeometry, FVector2D(CX + 80.f, Size.Y * 0.12f),
				FString::Printf(TEXT("-%d"), P->GetLastDamageDealt()), FLinearColor(1.f, 0.88f, 0.4f), 1.35f);
		}
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 280.f, Size.Y * 0.20f),
			FString::Printf(TEXT("HP %d/%d"), P->HP, P->HPMax), FLinearColor(0.9f, 0.95f, 1.f), 1.0f);
		if (const AHostilePet* E = P->GetBattleEnemy())
		{
			PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX + 80.f, Size.Y * 0.20f),
				FString::Printf(TEXT("%s  %s  HP %d/%d"), *HolypawCatalog::RankLabel(E->Rank), *HolypawCatalog::SpecialLabel(E->Special), FMath::Max(0, E->HP), E->HPMax),
				FLinearColor(1.f, 0.8f, 0.8f), 0.95f);
			if (E->GetDef().Faction == EHolypawFaction::PolyMill)
			{
				PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX + 80.f, Size.Y * 0.235f),
					TEXT("Poly Mill — cheap polyester"), FLinearColor(0.85f, 0.7f, 0.45f), 0.9f);
			}
			if (E->bPhaseTwo)
			{
				PaintText(OutDrawElements, Layer + 3, AllottedGeometry, FVector2D(CX + 80.f, Size.Y * 0.255f),
					TEXT("PHASE TWO"), FLinearColor(1.f, 0.55f, 0.42f), 1.05f);
			}
		}
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 300.f, Size.Y * 0.27f), P->GetBattleLog(), FLinearColor(0.95f, 0.9f, 1.f), 1.05f);
		const FString Status = P->GetBattleStatusLine();
		if (!Status.IsEmpty())
		{
			PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 300.f, Size.Y * 0.31f), Status, FLinearColor(0.7f, 0.95f, 0.85f), 0.95f);
		}

		PaintPanel(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(40.f, Size.Y - 280.f), FVector2D(420.f, 220.f), FLinearColor(0.10f, 0.07f, 0.14f, 0.82f));
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(56.f, Size.Y - 268.f), TEXT("1  Soft Slap"), FLinearColor(1.f, 1.f, 1.f), 1.1f);
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(56.f, Size.Y - 236.f), TEXT("2  Cuddle Beam (12 FP)"), FLinearColor(0.85f, 0.8f, 1.f), 1.1f);
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(56.f, Size.Y - 204.f), TEXT("3  Party Assault"), FLinearColor(0.7f, 0.95f, 0.85f), 1.1f);
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(56.f, Size.Y - 172.f), TEXT("4  Flee"), FLinearColor(1.f, 0.7f, 0.75f), 1.1f);
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(56.f, Size.Y - 140.f), TEXT("5  Guard + stitch"), FLinearColor(0.85f, 0.9f, 1.f), 1.1f);
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(56.f, Size.Y - 108.f), TEXT("6  Hymn (8 FP, lullaby?)"), FLinearColor(1.f, 0.85f, 0.55f), 1.1f);
	}

	if (P->IsSkillsOpen() && P->Skills && P->Affection)
	{
		PaintPanel(OutDrawElements, Layer, AllottedGeometry, FVector2D(CX - 320.f, 190.f), FVector2D(640.f, 430.f), FLinearColor(0.08f, 0.05f, 0.12f, 0.88f));
		PaintText(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 200.f, 204.f), HolypawCatalog::SkillTreeName(P->Skills->ActiveTree), FLinearColor(1.f, 0.8f, 0.9f), 1.35f);
		PaintText(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 280.f, 236.f), TEXT("Tab cycles Hug / Miracle / Party"), FLinearColor(0.8f, 0.75f, 0.9f), 0.9f);
		int32 I = 1;
		for (const FSkillDef& S : P->Skills->GetTreeSkills(P->Skills->ActiveTree))
		{
			const bool Owned = P->Skills->HasSkill(S.Id);
			const bool Can = P->Skills->CanBuy(S.Id, P->Affection->AP);
			const FString Line = FString::Printf(TEXT("%d  %s  (%d AP)%s%s"), I, *S.DisplayName.ToString(), S.Cost,
				Owned ? TEXT("  OWNED") : TEXT(""), (!Owned && Can) ? TEXT("  [buy]") : TEXT(""));
			PaintText(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 280.f, 250.f + I * 48.f), Line,
				Owned ? FLinearColor(0.7f, 0.55f, 1.f) : (Can ? FLinearColor(1.f, 0.8f, 0.9f) : FLinearColor(0.55f, 0.5f, 0.6f)), 1.0f);
			PaintText(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 260.f, 270.f + I * 48.f), S.Description.ToString(), FLinearColor(0.7f, 0.65f, 0.78f), 0.75f);
			++I;
		}
		PaintText(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 200.f, 250.f + I * 48.f + 8.f), TEXT("1-6 buy   K close"), FLinearColor(0.8f, 0.75f, 0.9f), 0.95f);
	}

	if (P->IsPartyOpen() && P->Party)
	{
		PaintPanel(OutDrawElements, Layer, AllottedGeometry, FVector2D(CX - 280.f, 220.f), FVector2D(560.f, 280.f), FLinearColor(0.08f, 0.05f, 0.12f, 0.88f));
		PaintText(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 120.f, 236.f), TEXT("Fluffy Party  (max 4)"), FLinearColor(1.f, 0.8f, 0.9f), 1.3f);
		if (P->Party->Members.Num() == 0)
		{
			PaintText(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 220.f, 300.f), TEXT("Empty. Recruit wild fluffies with E."), FLinearColor(0.75f, 0.7f, 0.85f), 1.0f);
		}
		else
		{
			int32 I = 0;
			for (const FPartyMember& M : P->Party->Members)
			{
				PaintText(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 200.f, 292.f + I * 36.f),
					FString::Printf(TEXT("%d  %s   %s   ATK %d"), I + 1, *M.DisplayName.ToString(), *M.Rarity, M.Attack),
					FLinearColor(0.95f, 0.85f, 1.f), 1.05f);
				++I;
			}
		}
		PaintText(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 80.f, 470.f), TEXT("P to close"), FLinearColor(0.8f, 0.75f, 0.9f), 0.95f);
	}

	auto PaintLines = [&](const FString& Title, const TArray<FString>& Lines)
	{
		PaintPanel(OutDrawElements, Layer, AllottedGeometry, FVector2D(CX - 340.f, 180.f), FVector2D(680.f, 360.f), FLinearColor(0.08f, 0.05f, 0.12f, 0.9f));
		PaintText(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 200.f, 196.f), Title, FLinearColor(1.f, 0.82f, 0.9f), 1.3f);
		int32 I = 0;
		for (const FString& Line : Lines)
		{
			PaintText(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 300.f, 240.f + I * 28.f), Line, FLinearColor(0.95f, 0.9f, 1.f), 0.95f);
			++I;
		}
	};
	if (P->IsTalkOpen())
	{
		PaintLines(TEXT("Testimony"), P->GetTalkLines());
	}
	if (P->IsShopOpen())
	{
		PaintLines(TEXT("Faith stall"), P->GetShopLines());
	}
	if (P->IsInventoryOpen())
	{
		PaintLines(TEXT("Pockets"), P->GetInventoryLines());
	}

	return Layer + 3;
}
