#include "UI/HolypawHUD.h"
#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"
#include "Components/SkillTreeComponent.h"
#include "Components/PartyComponent.h"
#include "Actors/HostilePet.h"
#include "HolypawTypes.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

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
	}

	if (!P->GetPrompt().IsEmpty())
	{
		DrawLabel(W * 0.5f - 180.f, Canvas->SizeY - 140.f, P->GetPrompt().ToString(), FLinearColor(1.f, 0.85f, 0.95f), 1.2f);
	}

	if (P->GetToastAlpha() > 0.f)
	{
		DrawLabel(W * 0.5f - 220.f, 190.f, P->GetToast(), FLinearColor(1.f, 0.95f, 0.75f), 1.25f);
	}

	DrawLabel(W - 460.f, Canvas->SizeY - 48.f, TEXT("WASD move  Mouse look  E interact  K skills  P party  M miracle  N map"), FLinearColor(0.75f, 0.7f, 0.9f), 0.85f);

	if (P->Mode == EHolypawPawnMode::Battle)
	{
		DrawLabel(W * 0.5f - 200.f, Canvas->SizeY * 0.28f, P->GetBattleEnemy() ? P->GetBattleEnemy()->DisplayName.ToString() : TEXT("Hostile"), FLinearColor(1.f, 0.55f, 0.62f), 1.8f);
		DrawLabel(W * 0.5f - 260.f, Canvas->SizeY * 0.28f + 48.f, P->GetBattleLog(), FLinearColor(0.95f, 0.9f, 1.f), 1.15f);
		if (P->GetBattleEnemy())
		{
			DrawLabel(W * 0.5f - 200.f, Canvas->SizeY * 0.28f + 84.f,
				FString::Printf(TEXT("Enemy %d/%d"), FMath::Max(0, P->GetBattleEnemy()->HP), P->GetBattleEnemy()->HPMax),
				FLinearColor(1.f, 0.8f, 0.8f), 1.1f);
		}
		DrawLabel(64.f, Canvas->SizeY - 220.f, TEXT("1  Soft Slap"), FLinearColor(1.f, 1.f, 1.f), 1.3f);
		DrawLabel(64.f, Canvas->SizeY - 180.f, TEXT("2  Cuddle Beam (12 FP)"), FLinearColor(0.85f, 0.8f, 1.f), 1.3f);
		DrawLabel(64.f, Canvas->SizeY - 140.f, TEXT("3  Party Assault"), FLinearColor(0.7f, 0.95f, 0.85f), 1.3f);
		DrawLabel(64.f, Canvas->SizeY - 100.f, TEXT("4  Flee"), FLinearColor(1.f, 0.7f, 0.75f), 1.3f);
	}

	if (P->IsSkillsOpen() && P->Skills)
	{
		DrawLabel(W * 0.5f - 160.f, 230.f, TEXT("Affection Skill Tree"), FLinearColor(1.f, 0.8f, 0.9f), 1.5f);
		int32 I = 1;
		for (const FSkillDef& S : P->Skills->GetCatalog())
		{
			const bool Owned = P->Skills->HasSkill(S.Id);
			const bool Can = P->Skills->CanBuy(S.Id, P->Affection->AP);
			FString Line = FString::Printf(TEXT("%d  %s  (%d AP)%s%s"),
				I,
				*S.DisplayName.ToString(),
				S.Cost,
				Owned ? TEXT("  OWNED") : TEXT(""),
				(!Owned && Can) ? TEXT("  [buy]") : TEXT(""));
			DrawLabel(W * 0.5f - 220.f, 270.f + I * 28.f, Line, Owned ? FLinearColor(0.7f, 0.55f, 1.f) : (Can ? FLinearColor(1.f, 0.8f, 0.9f) : FLinearColor(0.55f, 0.5f, 0.6f)), 1.05f);
			++I;
		}
		DrawLabel(W * 0.5f - 180.f, 270.f + I * 28.f + 12.f, TEXT("Press 1-6 to buy   K to close"), FLinearColor(0.8f, 0.75f, 0.9f), 1.0f);
	}

	if (P->IsPartyOpen() && P->Party)
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
			DrawLabel(W * 0.5f - 220.f, 280.f + I * 30.f, Line, FLinearColor(0.92f, 0.88f, 1.f), 1.12f);
			++I;
		}
		DrawLabel(W * 0.5f - 90.f, 280.f + I * 30.f + 16.f, TEXT("N to close"), FLinearColor(0.8f, 0.75f, 0.9f), 1.0f);
	}
}
