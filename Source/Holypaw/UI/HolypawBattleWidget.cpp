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
		Q.Fill(FVector2D(0.f, Size.Y * 0.07f), FVector2D(Size.X, Size.Y * 0.28f), HolypawUi::WithAlpha(Pal.Ink, 0.48f));
		const FVector2D Banner = HolypawUi::Fit(Size, FVector2D(1100.f, 132.f), 36.f);
		const FVector2D BannerPos((Size.X - Banner.X) * 0.5f, Size.Y * 0.09f);
		Q.Panel(BannerPos, Banner);

		const float PlateW = Banner.X * 0.42f;
		Q.Icon(BannerPos + FVector2D(20.f, 18.f), 32.f, EHolypawUiIcon::Teddy, Pal.Rose);
		Q.Text(BannerPos + FVector2D(58.f, 16.f), HolypawUiCopy::BattleYou(), Pal.Rose, 1.05f, 140.f);
		Q.Bar(BannerPos + FVector2D(58.f, 48.f), FVector2D(PlateW - 50.f, 14.f),
			Pawn->HPMax > 0 ? float(Pawn->HP) / float(Pawn->HPMax) : 0.f, Pal.HpFill);
		Q.Text(BannerPos + FVector2D(58.f, 68.f), FString::Printf(TEXT("%d / %d"), Pawn->HP, Pawn->HPMax), Pal.Cream, 0.8f, 140.f);

		Q.Text(FVector2D(CX - 18.f, BannerPos.Y + 40.f), HolypawUiCopy::BattleVs(), Pal.Gold, 1.25f, 60.f);

		const AHostilePet* Enemy = Pawn->GetBattleEnemy();
		const float EX = BannerPos.X + Banner.X - PlateW;
		Q.Icon(FVector2D(EX, BannerPos.Y + 18.f), 32.f, EHolypawUiIcon::Mill, Pal.Danger);
		Q.Text(FVector2D(EX + 38.f, BannerPos.Y + 16.f), Enemy ? Enemy->DisplayName : FText::FromString(TEXT("Hostile")), Pal.Danger, 1.05f, PlateW - 50.f);
		if (Enemy)
		{
			Q.Bar(FVector2D(EX + 38.f, BannerPos.Y + 48.f), FVector2D(PlateW - 50.f, 14.f),
				Enemy->HPMax > 0 ? float(FMath::Max(0, Enemy->HP)) / float(Enemy->HPMax) : 0.f, Pal.EnemyFill);
			Q.Text(FVector2D(EX + 38.f, BannerPos.Y + 68.f),
				FString::Printf(TEXT("%s · %s · %d/%d"),
					*HolypawCatalog::RankLabel(Enemy->Rank),
					*HolypawCatalog::SpecialLabel(Enemy->Special),
					FMath::Max(0, Enemy->HP), Enemy->HPMax),
				Pal.Cream, 0.75f, PlateW - 50.f);
			if (Enemy->GetDef().Faction == EHolypawFaction::PolyMill)
			{
				Q.Chip(FVector2D(EX + 38.f, BannerPos.Y + 92.f), FVector2D(150.f, 24.f), EHolypawUiIcon::Mill, TEXT("Poly Mill"), Pal.Gold);
			}
			if (Enemy->bPhaseTwo)
			{
				Q.Chip(FVector2D(EX + 196.f, BannerPos.Y + 92.f), FVector2D(120.f, 24.f), EHolypawUiIcon::Halo, HolypawUiCopy::PhaseTwo().ToString(), Pal.Danger);
			}
		}
		if (Pawn->GetDamagePopupTime() > 0.f)
		{
			Q.Text(FVector2D(EX + 38.f, BannerPos.Y - 8.f), FString::Printf(TEXT("-%d"), Pawn->GetLastDamageDealt()), Pal.Gold, 1.25f, 80.f);
		}

		const FVector2D LogPos(BannerPos.X, BannerPos.Y + Banner.Y + 8.f);
		Q.Fill(LogPos, FVector2D(Banner.X, 36.f), Pal.Felt);
		Q.Frame(LogPos, FVector2D(Banner.X, 36.f), Pal.Gold, 1.3f);
		Q.Text(LogPos + FVector2D(14.f, 8.f), HolypawUi::Ellipsize(Pawn->GetBattleLog(), Banner.X - 40.f, 0.85f), Pal.Cream, 0.85f, Banner.X - 36.f);
		const FString Status = Pawn->GetBattleStatusLine();
		if (!Status.IsEmpty())
		{
			Q.Text(LogPos + FVector2D(Banner.X * 0.55f, 8.f), HolypawUi::Ellipsize(Status, Banner.X * 0.42f, 0.75f), Pal.Mint, 0.75f, Banner.X * 0.42f);
		}

		const int32 Page = Pawn->GetBattlePage();
		const FVector2D Tray = HolypawUi::Fit(Size, FVector2D(1080.f, 178.f), 24.f);
		const FVector2D TrayPos((Size.X - Tray.X) * 0.5f, Size.Y - Tray.Y - 20.f);
		Q.Panel(TrayPos, Tray);
		Q.Chip(TrayPos + FVector2D(16.f, 12.f), FVector2D(120.f, 26.f), EHolypawUiIcon::Slap, HolypawUiCopy::PageBasics().ToString(), Page == 0 ? Pal.Gold : Pal.Muted);
		Q.Chip(TrayPos + FVector2D(142.f, 12.f), FVector2D(130.f, 26.f), EHolypawUiIcon::Mill, HolypawUiCopy::PageOverflow().ToString(), Page == 1 ? Pal.Gold : Pal.Muted);
		Q.Chip(TrayPos + FVector2D(278.f, 12.f), FVector2D(140.f, 26.f), EHolypawUiIcon::Party, HolypawUiCopy::PageParty().ToString(), Page == 2 ? Pal.Gold : Pal.Muted);
		Q.Text(TrayPos + FVector2D(430.f, 16.f), HolypawUiCopy::TabHint(), Pal.Muted, 0.75f, 80.f);

		const float CardW = (Tray.X - 32.f) / 6.f;
		for (int32 Slot = 1; Slot <= 6; ++Slot)
		{
			const FHolypawAbilityDef* Ability = HolypawCatalog::FindAbilityBySlot(Page, Slot);
			const FVector2D Card(TrayPos.X + 16.f + (Slot - 1) * CardW, TrayPos.Y + 46.f);
			Q.Fill(Card, FVector2D(CardW - 8.f, 114.f), Pal.Idle);
			Q.Frame(Card, FVector2D(CardW - 8.f, 114.f), Pal.Gold, 1.4f);
			Q.Keycap(Card + FVector2D(6.f, 8.f), FString::FromInt(Slot));
			if (Ability)
			{
				const EHolypawUiIcon Icon = HolypawUi::IconForAbility(Ability->Id);
				Q.Icon(Card + FVector2D((CardW - 40.f) * 0.5f, 28.f), 28.f, Icon, HolypawUi::IconTint(Icon));
				Q.Text(Card + FVector2D(8.f, 64.f), HolypawUi::Ellipsize(Ability->DisplayName.ToString(), CardW - 20.f, 0.72f), Pal.Cream, 0.72f, CardW - 16.f);
				if (Ability->FpCost > 0)
				{
					Q.Text(Card + FVector2D(8.f, 86.f), FString::Printf(TEXT("%d FP"), Ability->FpCost), Pal.Gold, 0.68f, CardW - 16.f);
				}
				else if (Ability->Stitch > 0)
				{
					Q.Text(Card + FVector2D(8.f, 86.f), FString::Printf(TEXT("+%d stitch"), Ability->Stitch), Pal.Mint, 0.68f, CardW - 16.f);
				}
			}
		}
	}

	if (Pawn->IsSkillsOpen() && Pawn->Skills && Pawn->Affection)
	{
		const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(640.f, 430.f), 40.f);
		const FVector2D Origin = HolypawUi::Centered(Size, Panel);
		Q.Panel(Origin, Panel);
		Q.Caption(Origin + FVector2D(22.f, 14.f), EHolypawUiIcon::Halo, HolypawCatalog::SkillTreeName(Pawn->Skills->ActiveTree), Pal.Rose);
		Q.Text(Origin + FVector2D(22.f, 50.f), HolypawUiCopy::TabHint(), Pal.Muted, 0.8f, 200.f);
		int32 I = 1;
		for (const FSkillDef& S : Pawn->Skills->GetTreeSkills(Pawn->Skills->ActiveTree))
		{
			const bool Owned = Pawn->Skills->HasSkill(S.Id);
			const bool Can = Pawn->Skills->CanBuy(S.Id, Pawn->Affection->AP);
			const FVector2D Row = Origin + FVector2D(22.f, 68.f + I * 50.f);
			Q.Keycap(Row, FString::FromInt(I));
			Q.Text(Row + FVector2D(44.f, 0.f), S.DisplayName, Owned ? Pal.Mint : (Can ? Pal.Cream : Pal.Muted), 0.95f, 280.f);
			Q.Text(Row + FVector2D(330.f, 0.f), FString::Printf(TEXT("%d AP"), S.Cost), Pal.Gold, 0.8f, 80.f);
			if (Owned)
			{
				Q.Chip(Row + FVector2D(410.f, -4.f), FVector2D(90.f, 24.f), EHolypawUiIcon::Check, HolypawUiCopy::Owned().ToString(), Pal.Mint);
			}
			else if (Can)
			{
				Q.Chip(Row + FVector2D(410.f, -4.f), FVector2D(80.f, 24.f), EHolypawUiIcon::Coin, HolypawUiCopy::Buy().ToString(), Pal.Gold);
			}
			Q.Text(Row + FVector2D(44.f, 22.f), HolypawUi::Ellipsize(S.Description.ToString(), Panel.X - 90.f, 0.72f), Pal.Muted, 0.72f, Panel.X - 86.f);
			++I;
		}
	}

	if (Pawn->IsPartyOpen() && Pawn->Party)
	{
		const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(560.f, 300.f), 40.f);
		const FVector2D Origin = HolypawUi::Centered(Size, Panel);
		Q.Panel(Origin, Panel);
		Q.Caption(Origin + FVector2D(22.f, 14.f), EHolypawUiIcon::Party, TEXT("Fluffy Party"), Pal.Rose);
		if (Pawn->Party->Members.Num() == 0)
		{
			Q.Text(Origin + FVector2D(24.f, 80.f), TEXT("Empty. Recruit wild fluffies with E."), Pal.Muted, 0.95f, Panel.X - 48.f);
		}
		else
		{
			int32 I = 0;
			for (const FPartyMember& M : Pawn->Party->Members)
			{
				const FVector2D Row = Origin + FVector2D(24.f, 70.f + I * 48.f);
				Q.Fill(Row, FVector2D(Panel.X - 48.f, 42.f), Pal.Idle);
				Q.Frame(Row, FVector2D(Panel.X - 48.f, 42.f), Pal.Rose, 1.4f);
				Q.Icon(Row + FVector2D(10.f, 8.f), 24.f, EHolypawUiIcon::Paw, Pal.Rose);
				Q.Text(Row + FVector2D(42.f, 10.f), M.DisplayName, Pal.Cream, 0.95f, 180.f);
				Q.Text(Row + FVector2D(230.f, 10.f), UPartyComponent::RoleLabel(M.Role), Pal.Gold, 0.8f, 100.f);
				Q.Text(Row + FVector2D(340.f, 10.f), FString::Printf(TEXT("ATK %d"), M.Attack), Pal.Powder, 0.8f, 80.f);
				++I;
			}
		}
	}

	if (Pawn->IsInventoryOpen())
	{
		const FVector2D Panel = HolypawUi::Fit(Size, FVector2D(640.f, 360.f), 40.f);
		const FVector2D Origin = HolypawUi::Centered(Size, Panel);
		Q.Panel(Origin, Panel);
		Q.Caption(Origin + FVector2D(22.f, 14.f), EHolypawUiIcon::Bun, TEXT("Pockets"), Pal.Gold);
		const TArray<FHolypawItemStack>& Bags = Pawn->GetInventory();
		if (Bags.Num() == 0)
		{
			Q.Text(Origin + FVector2D(24.f, 80.f), TEXT("Empty. Stalls sell buns. Mills drop scrap."), Pal.Muted, 0.9f, Panel.X - 48.f);
		}
		for (int32 I = 0; I < Bags.Num(); ++I)
		{
			const FHolypawItemDef* Def = HolypawCatalog::FindItem(Bags[I].Id);
			const FString Name = Def ? Def->DisplayName.ToString() : Bags[I].Id.ToString();
			const EHolypawUiIcon Icon = HolypawUi::IconForItem(Bags[I].Id);
			const FVector2D Row = Origin + FVector2D(24.f, 64.f + I * 48.f);
			Q.Fill(Row, FVector2D(Panel.X - 48.f, 42.f), Pal.Idle);
			Q.Frame(Row, FVector2D(Panel.X - 48.f, 42.f), Pal.Gold, 1.3f);
			Q.Keycap(Row + FVector2D(8.f, 10.f), FString::FromInt(I + 1));
			Q.Icon(Row + FVector2D(52.f, 8.f), 24.f, Icon, HolypawUi::IconTint(Icon));
			Q.Text(Row + FVector2D(84.f, 10.f), Name, Pal.Cream, 0.95f, 280.f);
			Q.Text(Row + FVector2D(380.f, 10.f), FString::Printf(TEXT("x%d"), Bags[I].Count), Pal.Gold, 0.9f, 60.f);
		}
	}

	return Layer + 6;
}
