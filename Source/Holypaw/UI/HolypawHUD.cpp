#include "UI/HolypawHUD.h"
#include "UI/HolypawBattleWidget.h"
#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"
#include "Components/SkillTreeComponent.h"
#include "Components/MissionComponent.h"
#include "Actors/HostilePet.h"
#include "HolypawTypes.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"

void AHolypawHUD::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = GetOwningPlayerController())
	{
		OverlayWidget = CreateWidget<UHolypawBattleWidget>(PC);
		if (OverlayWidget)
		{
			OverlayWidget->AddToViewport(20);
		}
	}
}

void AHolypawHUD::DrawLabel(float X, float Y, const FString& Text, const FLinearColor& Color, float Scale)
{
	if (!Canvas)
	{
		return;
	}
	UFont* Font = GEngine ? GEngine->GetLargeFont() : nullptr;
	FCanvasTextItem Item(FVector2D(X, Y), FText::FromString(Text), Font, Color);
	Item.Scale = FVector2D(Scale, Scale);
	Item.EnableShadow(FLinearColor(0.f, 0.f, 0.f, 0.65f));
	Canvas->DrawItem(Item);
}

void AHolypawHUD::DrawHUD()
{
	Super::DrawHUD();
	AHolypawCharacter* P = Cast<AHolypawCharacter>(GetOwningPawn());
	if (!P || !P->Affection || !Canvas)
	{
		return;
	}

	const float W = Canvas->SizeX;
	DrawLabel(32.f, 24.f, HolypawCatalog::ZoneDisplayName(P->CurrentZone), FLinearColor(0.82f, 0.75f, 1.f), 1.05f);
	DrawLabel(32.f, 52.f, TEXT("Fluffy Ascendancy"), FLinearColor(1.f, 0.78f, 0.88f), 1.45f);

	const FString Stats = FString::Printf(TEXT("AP %d    FP %d    LVL %d    HP %d/%d"),
		P->Affection->AP, P->Affection->FP, P->Affection->Level, P->HP, P->HPMax);
	DrawLabel(32.f, 92.f, Stats, FLinearColor(0.95f, 0.93f, 1.f), 1.15f);

	const float Pct = P->Affection->MiracleMax > 0.f ? P->Affection->MiracleCharge / P->Affection->MiracleMax : 0.f;
	DrawLabel(32.f, 124.f, FString::Printf(TEXT("Miracle  %d / %d"), FMath::FloorToInt(P->Affection->MiracleCharge), FMath::FloorToInt(P->Affection->MiracleMax)), FLinearColor(1.f, 0.85f, 0.55f), 1.05f);
	if (Canvas)
	{
		FCanvasTileItem BarBg(FVector2D(32.f, 152.f), FVector2D(280.f, 14.f), FLinearColor(0.05f, 0.04f, 0.08f, 0.7f));
		BarBg.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(BarBg);
		FCanvasTileItem Bar(FVector2D(32.f, 152.f), FVector2D(280.f * Pct, 14.f), FLinearColor(0.95f, 0.72f, 0.35f, 0.95f));
		Bar.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(Bar);
	}

	if (!P->GetCompassLine().IsEmpty() && P->Mode == EHolypawPawnMode::Play)
	{
		DrawLabel(32.f, 176.f, P->GetCompassLine(), FLinearColor(0.85f, 0.9f, 1.f), 1.0f);
		if (P->Story)
		{
			DrawLabel(32.f, 200.f, P->Story->GetCurrent().Title.ToString(), FLinearColor(1.f, 0.82f, 0.7f), 0.95f);
		}
	}

	if (!P->GetPrompt().IsEmpty())
	{
		DrawLabel(W * 0.5f - 180.f, Canvas->SizeY - 140.f, P->GetPrompt().ToString(), FLinearColor(1.f, 0.85f, 0.95f), 1.2f);
	}

	if (P->GetToastAlpha() > 0.f)
	{
		DrawLabel(W * 0.5f - 220.f, 190.f, P->GetToast(), FLinearColor(1.f, 0.95f, 0.75f), 1.25f);
	}

	DrawLabel(W - 560.f, Canvas->SizeY - 48.f, TEXT("WASD  E  K trees  J journal  P party  M miracle  N map  V villains"), FLinearColor(0.75f, 0.7f, 0.9f), 0.82f);

	const bool bUmgOverlay = OverlayWidget != nullptr;
	if (!bUmgOverlay && P->Mode == EHolypawPawnMode::Battle)
	{
		const float CX = W * 0.5f;
		const float CY = Canvas->SizeY * 0.22f;
		FCanvasTileItem Dim(FVector2D(0.f, Canvas->SizeY * 0.12f), FVector2D(W, Canvas->SizeY * 0.5f), FLinearColor(0.04f, 0.02f, 0.06f, 0.55f));
		Dim.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(Dim);
		DrawLabel(CX - 280.f, CY, TEXT("YOU"), FLinearColor(1.f, 0.85f, 0.92f), 1.2f);
		DrawLabel(CX + 80.f, CY, P->GetBattleEnemy() ? P->GetBattleEnemy()->DisplayName.ToString() : TEXT("Hostile"), FLinearColor(1.f, 0.55f, 0.62f), 1.35f);
		DrawLabel(CX - 40.f, CY + 8.f, TEXT("VS"), FLinearColor(1.f, 0.9f, 0.5f), 1.6f);
		DrawLabel(CX - 280.f, CY + 36.f, FString::Printf(TEXT("HP %d/%d"), P->HP, P->HPMax), FLinearColor(0.9f, 0.95f, 1.f), 1.05f);
		if (P->GetBattleEnemy())
		{
			AHostilePet* E = P->GetBattleEnemy();
			DrawLabel(CX + 80.f, CY + 36.f,
				FString::Printf(TEXT("%s  %s  HP %d/%d"), *HolypawCatalog::RankLabel(E->Rank), *HolypawCatalog::SpecialLabel(E->Special), FMath::Max(0, E->HP), E->HPMax),
				FLinearColor(1.f, 0.8f, 0.8f), 1.0f);
			if (E->GetDef().Faction == EHolypawFaction::PolyMill)
			{
				DrawLabel(CX + 80.f, CY + 60.f, TEXT("Poly Mill — cheap polyester"), FLinearColor(0.85f, 0.7f, 0.45f), 0.95f);
			}
		}
		DrawLabel(CX - 300.f, CY + 88.f, P->GetBattleLog(), FLinearColor(0.95f, 0.9f, 1.f), 1.12f);
		DrawLabel(64.f, Canvas->SizeY - 260.f, TEXT("1  Soft Slap"), FLinearColor(1.f, 1.f, 1.f), 1.2f);
		DrawLabel(64.f, Canvas->SizeY - 228.f, TEXT("2  Cuddle Beam (12 FP)"), FLinearColor(0.85f, 0.8f, 1.f), 1.2f);
		DrawLabel(64.f, Canvas->SizeY - 196.f, TEXT("3  Party Assault"), FLinearColor(0.7f, 0.95f, 0.85f), 1.2f);
		DrawLabel(64.f, Canvas->SizeY - 164.f, TEXT("4  Flee"), FLinearColor(1.f, 0.7f, 0.75f), 1.2f);
		DrawLabel(64.f, Canvas->SizeY - 132.f, TEXT("5  Guard"), FLinearColor(0.85f, 0.9f, 1.f), 1.2f);
		DrawLabel(64.f, Canvas->SizeY - 100.f, TEXT("6  Hymn (8 FP)"), FLinearColor(1.f, 0.85f, 0.55f), 1.2f);
	}

	if (!bUmgOverlay && P->IsSkillsOpen() && P->Skills)
	{
		DrawLabel(W * 0.5f - 200.f, 210.f, HolypawCatalog::SkillTreeName(P->Skills->ActiveTree), FLinearColor(1.f, 0.8f, 0.9f), 1.5f);
		DrawLabel(W * 0.5f - 200.f, 242.f, TEXT("Tab cycles Hug / Miracle / Party trees"), FLinearColor(0.8f, 0.75f, 0.9f), 0.95f);
		int32 I = 1;
		for (const FSkillDef& S : P->Skills->GetTreeSkills(P->Skills->ActiveTree))
		{
			const bool Owned = P->Skills->HasSkill(S.Id);
			const bool Can = P->Skills->CanBuy(S.Id, P->Affection->AP);
			FString Line = FString::Printf(TEXT("%d  %s  (%d AP)%s%s"),
				I,
				*S.DisplayName.ToString(),
				S.Cost,
				Owned ? TEXT("  OWNED") : TEXT(""),
				(!Owned && Can) ? TEXT("  [buy]") : TEXT(""));
			DrawLabel(W * 0.5f - 240.f, 268.f + I * 30.f, Line, Owned ? FLinearColor(0.7f, 0.55f, 1.f) : (Can ? FLinearColor(1.f, 0.8f, 0.9f) : FLinearColor(0.55f, 0.5f, 0.6f)), 1.05f);
			DrawLabel(W * 0.5f - 220.f, 268.f + I * 30.f + 16.f, S.Description.ToString(), FLinearColor(0.7f, 0.65f, 0.78f), 0.8f);
			++I;
		}
		DrawLabel(W * 0.5f - 180.f, 268.f + I * 30.f + 20.f, TEXT("Press 1-6 to buy   K to close"), FLinearColor(0.8f, 0.75f, 0.9f), 1.0f);
	}

	if (!bUmgOverlay && P->IsPartyOpen() && P->Party)
	{
		DrawLabel(W * 0.5f - 120.f, 240.f, TEXT("Fluffy Party"), FLinearColor(1.f, 0.8f, 0.9f), 1.5f);
		if (P->Party->Members.Num() == 0)
		{
			DrawLabel(W * 0.5f - 220.f, 300.f, TEXT("Empty. Recruit wild fluffies with E."), FLinearColor(0.75f, 0.7f, 0.85f), 1.1f);
		}
		else
		{
			int32 I = 0;
			for (const FPartyMember& M : P->Party->Members)
			{
				DrawLabel(W * 0.5f - 200.f, 300.f + I * 32.f,
					FString::Printf(TEXT("%d  %s   %s   ATK %d"), I + 1, *M.DisplayName.ToString(), *M.Rarity, M.Attack),
					FLinearColor(0.95f, 0.85f, 1.f), 1.15f);
				++I;
			}
		}
		DrawLabel(W * 0.5f - 80.f, Canvas->SizeY - 160.f, TEXT("P to close"), FLinearColor(0.8f, 0.75f, 0.9f), 1.0f);
	}

	if (P->IsMapOpen())
	{
		DrawLabel(W * 0.5f - 160.f, 230.f, TEXT("Survey Map"), FLinearColor(1.f, 0.85f, 0.7f), 1.5f);
		int32 I = 0;
		for (const FString& Line : P->GetMapLines())
		{
		DrawLabel(W * 0.5f - 220.f, 270.f + I * 24.f, Line, FLinearColor(0.92f, 0.88f, 1.f), 0.95f);
			++I;
		}
		DrawLabel(W * 0.5f - 90.f, 270.f + I * 24.f + 12.f, TEXT("N to close"), FLinearColor(0.8f, 0.75f, 0.9f), 1.0f);
	}

	if (P->IsCodexOpen())
	{
		DrawLabel(W * 0.5f - 200.f, 200.f, TEXT("Villain Codex"), FLinearColor(1.f, 0.55f, 0.62f), 1.55f);
		DrawLabel(W * 0.5f - 200.f, 236.f,
			FString::Printf(TEXT("Seen %d   Fell %d   of %d"), P->GetCodexSeenCount(), P->GetCodexDefeatedCount(), P->GetCodexTotal()),
			FLinearColor(0.95f, 0.82f, 0.7f), 1.05f);
		const TArray<FString> Entries = P->GetCodexLines();
		const int32 ColH = 20;
		for (int32 I = 0; I < Entries.Num(); ++I)
		{
			const bool Left = I < ColH;
			const float X = Left ? (W * 0.5f - 430.f) : (W * 0.5f + 20.f);
			const int32 Row = Left ? I : (I - ColH);
			DrawLabel(X, 270.f + Row * 22.f, Entries[I], FLinearColor(0.92f, 0.86f, 0.95f), 0.82f);
		}
		DrawLabel(W * 0.5f - 160.f, Canvas->SizeY - 80.f, TEXT("V or Esc to close   ???? = not yet met"), FLinearColor(0.8f, 0.75f, 0.9f), 1.0f);
	}

	if (P->IsJournalOpen())
	{
		DrawLabel(W * 0.5f - 180.f, 200.f, TEXT("Bear Faith Journal"), FLinearColor(1.f, 0.85f, 0.65f), 1.5f);
		int32 I = 0;
		for (const FString& Line : P->GetJournalLines())
		{
			DrawLabel(W * 0.5f - 360.f, 250.f + I * 24.f, Line, FLinearColor(0.95f, 0.9f, 0.82f), 0.95f);
			++I;
		}
		DrawLabel(W * 0.5f - 90.f, Canvas->SizeY - 80.f, TEXT("J or Esc to close"), FLinearColor(0.8f, 0.75f, 0.9f), 1.0f);
	}
}
