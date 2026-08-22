#include "UI/HolypawBattleWidget.h"
#include "UI/HolypawUiTheme.h"
#include "UI/HolypawUiCopy.h"
#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"
#include "Components/SkillTreeComponent.h"
#include "Components/PartyComponent.h"
#include "Actors/HostilePet.h"
#include "HolypawTypes.h"

void UHolypawBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

int32 UHolypawBattleWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(GetOwningPlayerPawn());
	if (!Pawn)
	{
		return Layer;
	}

	HolypawUi::FPaint Q{OutDrawElements, AllottedGeometry, Layer};
	const HolypawUi::FPalette& Pal = HolypawUi::Colors();
	const FVector2D Size = Q.Canvas();
	const float CX = Size.X * 0.5f;

	if (Pawn->Mode == EHolypawPawnMode::Battle)
	{
		Q.Fill(FVector2D(0.f, Size.Y * 0.08f), FVector2D(Size.X, Size.Y * 0.34f), HolypawUi::WithAlpha(Pal.Ink, 0.55f));
		const FVector2D Banner(FMath::Min(Size.X - 72.f, 1100.f), 168.f);
		const FVector2D BannerPos((Size.X - Banner.X) * 0.5f, Size.Y * 0.10f);
		Q.Panel(BannerPos, Banner);

		Q.Icon(BannerPos + FVector2D(28.f, 22.f), 36.f, EHolypawUiIcon::Teddy, Pal.Rose);
		Q.Text(BannerPos + FVector2D(72.f, 20.f), HolypawUiCopy::BattleYou(), Pal.Rose, 1.15f, 160.f);
		Q.Bar(BannerPos + FVector2D(72.f, 54.f), FVector2D(280.f, 16.f),
			Pawn->HPMax > 0 ? float(Pawn->HP) / float(Pawn->HPMax) : 0.f, Pal.HpFill);
		Q.Text(BannerPos + FVector2D(72.f, 76.f), FString::Printf(TEXT("%d / %d"), Pawn->HP, Pawn->HPMax), Pal.Cream, 0.85f, 160.f);

		Q.Text(FVector2D(CX - 24.f, BannerPos.Y + 40.f), HolypawUiCopy::BattleVs(), Pal.Gold, 1.4f, 80.f);

		const AHostilePet* Enemy = Pawn->GetBattleEnemy();
		const FString EnemyName = Enemy ? Enemy->DisplayName.ToString() : TEXT("Hostile");
		Q.Icon(BannerPos + FVector2D(Banner.X - 360.f, 22.f), 36.f, EHolypawUiIcon::Mill, Pal.Danger);
		Q.Text(BannerPos + FVector2D(Banner.X - 316.f, 20.f), EnemyName, Pal.Danger, 1.15f, 280.f);
		if (Enemy)
		{
			Q.Bar(BannerPos + FVector2D(Banner.X - 316.f, 54.f), FVector2D(280.f, 16.f),
				Enemy->HPMax > 0 ? float(FMath::Max(0, Enemy->HP)) / float(Enemy->HPMax) : 0.f, Pal.EnemyFill);
			Q.Text(BannerPos + FVector2D(Banner.X - 316.f, 76.f),
				FString::Printf(TEXT("%s  %s  %d / %d"),
					*HolypawCatalog::RankLabel(Enemy->Rank),
					*HolypawCatalog::SpecialLabel(Enemy->Special),
					FMath::Max(0, Enemy->HP), Enemy->HPMax),
				Pal.Cream, 0.85f, 300.f);
			if (Enemy->GetDef().Faction == EHolypawFaction::PolyMill)
			{
				Q.Chip(BannerPos + FVector2D(Banner.X - 316.f, 104.f), FVector2D(220.f, 28.f),
					EHolypawUiIcon::Mill, HolypawUiCopy::PolyMill().ToString(), Pal.Gold);
			}
			if (Enemy->bPhaseTwo)
			{
				Q.Chip(BannerPos + FVector2D(Banner.X - 86.f, 104.f), FVector2D(140.f, 28.f),
					EHolypawUiIcon::Halo, HolypawUiCopy::PhaseTwo().ToString(), Pal.Danger);
			}
		}
		if (Pawn->GetDamagePopupTime() > 0.f)
		{
			Q.Text(BannerPos + FVector2D(Banner.X - 316.f, -6.f),
				FString::Printf(TEXT("-%d"), Pawn->GetLastDamageDealt()), Pal.Gold, 1.35f, 120.f);
		}
		Q.Text(BannerPos + FVector2D(28.f, 118.f), Pawn->GetBattleLog(), Pal.Cream, 0.9f, Banner.X - 60.f);
		const FString Status = Pawn->GetBattleStatusLine();
		if (!Status.IsEmpty())
		{
			Q.Text(BannerPos + FVector2D(28.f, 142.f), Status, Pal.Mint, 0.8f, Banner.X - 60.f);
		}

		const int32 Page = Pawn->GetBattlePage();
		const FText PageName = Page == 0 ? HolypawUiCopy::PageBasics() : (Page == 1 ? HolypawUiCopy::PageOverflow() : HolypawUiCopy::PageParty());
		const FVector2D Tray(FMath::Min(Size.X - 48.f, 1080.f), 168.f);
		const FVector2D TrayPos((Size.X - Tray.X) * 0.5f, Size.Y - Tray.Y - 28.f);
		Q.Panel(TrayPos, Tray);
		Q.Text(TrayPos + FVector2D(24.f, 14.f), PageName, Pal.Gold, 1.0f, 240.f);
		Q.Text(TrayPos + FVector2D(160.f, 16.f), TEXT("Tab cycles pages"), Pal.Muted, 0.8f, 240.f);

		const float CardW = (Tray.X - 48.f) / 6.f;
		for (int32 Slot = 1; Slot <= 6; ++Slot)
		{
			const FHolypawAbilityDef* Ability = HolypawCatalog::FindAbilityBySlot(Page, Slot);
			const FVector2D Card(TrayPos.X + 16.f + (Slot - 1) * CardW, TrayPos.Y + 46.f);
			Q.Fill(Card, FVector2D(CardW - 10.f, 100.f), Pal.Idle);
			Q.DashRect(Card + FVector2D(3.f, 3.f), FVector2D(CardW - 16.f, 94.f), Pal.Gold, 1.2f, 5.f);
			Q.Keycap(Card + FVector2D(8.f, 10.f), FString::FromInt(Slot));
			if (Ability)
			{
				const EHolypawUiIcon Icon = HolypawUi::IconForAbility(Ability->Id);
				Q.Icon(Card + FVector2D((CardW - 42.f) * 0.5f, 12.f), 32.f, Icon, HolypawUi::IconTint(Icon));
				Q.Text(Card + FVector2D(8.f, 50.f), Ability->DisplayName, Pal.Cream, 0.75f, CardW - 20.f);
				FString Cost;
				if (Ability->FpCost > 0)
				{
					Cost = FString::Printf(TEXT("%d FP"), Ability->FpCost);
				}
				else if (Ability->Stitch > 0)
				{
					Cost = FString::Printf(TEXT("+%d stitch"), Ability->Stitch);
				}
				if (!Cost.IsEmpty())
				{
					Q.Text(Card + FVector2D(8.f, 74.f), Cost, Pal.Gold, 0.7f, CardW - 20.f);
				}
			}
		}
	}

	if (Pawn->IsSkillsOpen() && Pawn->Skills && Pawn->Affection)
	{
		const FVector2D Panel(640.f, 430.f);
		const FVector2D Origin(CX - 320.f, 190.f);
		Q.Panel(Origin, Panel);
		Q.Caption(Origin + FVector2D(24.f, 16.f), EHolypawUiIcon::Halo, HolypawCatalog::SkillTreeName(Pawn->Skills->ActiveTree), Pal.Rose);
		Q.Text(Origin + FVector2D(24.f, 52.f), TEXT("Tab cycles Hug / Miracle / Party"), Pal.Muted, 0.85f, 500.f);
		int32 I = 1;
		for (const FSkillDef& S : Pawn->Skills->GetTreeSkills(Pawn->Skills->ActiveTree))
		{
			const bool Owned = Pawn->Skills->HasSkill(S.Id);
			const bool Can = Pawn->Skills->CanBuy(S.Id, Pawn->Affection->AP);
			Q.Keycap(Origin + FVector2D(28.f, 70.f + I * 48.f), FString::FromInt(I));
			Q.Text(Origin + FVector2D(64.f, 70.f + I * 48.f),
				FString::Printf(TEXT("%s  (%d AP)%s%s"), *S.DisplayName.ToString(), S.Cost,
					Owned ? TEXT("  owned") : TEXT(""), (!Owned && Can) ? TEXT("  buy") : TEXT("")),
				Owned ? Pal.Mint : (Can ? Pal.Cream : Pal.Muted), 0.95f, 520.f);
			Q.Text(Origin + FVector2D(64.f, 90.f + I * 48.f), S.Description, Pal.Muted, 0.75f, 520.f);
			++I;
		}
		Q.Footer(Origin, Panel, TEXT("1-6 buy   K close"));
	}

	if (Pawn->IsPartyOpen() && Pawn->Party)
	{
		const FVector2D Panel(560.f, 280.f);
		const FVector2D Origin(CX - 280.f, 220.f);
		Q.Panel(Origin, Panel);
		Q.Caption(Origin + FVector2D(24.f, 16.f), EHolypawUiIcon::Party, TEXT("Fluffy Party"), Pal.Rose);
		if (Pawn->Party->Members.Num() == 0)
		{
			Q.Text(Origin + FVector2D(28.f, 80.f), TEXT("Empty. Recruit wild fluffies with E."), Pal.Muted, 1.0f, 480.f);
		}
		else
		{
			int32 I = 0;
			for (const FPartyMember& M : Pawn->Party->Members)
			{
				Q.Icon(Origin + FVector2D(28.f, 72.f + I * 40.f), 22.f, EHolypawUiIcon::Paw, Pal.Rose);
				Q.Text(Origin + FVector2D(58.f, 74.f + I * 40.f),
					FString::Printf(TEXT("%d  %s   %s   ATK %d   %s"), I + 1, *M.DisplayName.ToString(), *M.Rarity, M.Attack,
						UPartyComponent::RoleLabel(M.Role)),
					Pal.Cream, 0.95f, 460.f);
				++I;
			}
		}
		Q.Footer(Origin, Panel, TEXT("P closes"));
	}

	if (Pawn->IsInventoryOpen())
	{
		const FVector2D Panel(640.f, 360.f);
		const FVector2D Origin(CX - 320.f, 180.f);
		Q.Panel(Origin, Panel);
		Q.Caption(Origin + FVector2D(24.f, 16.f), EHolypawUiIcon::Bun, TEXT("Pockets"), Pal.Gold);
		const TArray<FString> Lines = Pawn->GetInventoryLines();
		int32 I = 0;
		for (const FString& Line : Lines)
		{
			if (I == 0)
			{
				++I;
				continue;
			}
			Q.Text(Origin + FVector2D(32.f, 64.f + (I - 1) * 28.f), Line, Pal.Cream, 0.95f, 560.f);
			++I;
		}
		Q.Footer(Origin, Panel, TEXT("1-4 use   I closes"));
	}

	return Layer + 6;
}
