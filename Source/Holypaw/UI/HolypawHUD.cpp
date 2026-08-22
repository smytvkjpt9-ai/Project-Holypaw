#include "UI/HolypawHUD.h"
#include "UI/HolypawBattleWidget.h"
#include "UI/HolypawTitleWidget.h"
#include "UI/HolypawPauseWidget.h"
#include "UI/HolypawMapWidget.h"
#include "UI/HolypawJournalWidget.h"
#include "UI/HolypawTalkWidget.h"
#include "UI/HolypawShopWidget.h"
#include "UI/HolypawPlayHudWidget.h"
#include "Character/HolypawCharacter.h"
#include "HolypawTypes.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"

void AHolypawHUD::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return;
	}

	PlayHudWidget = CreateWidget<UHolypawPlayHudWidget>(PC);
	if (PlayHudWidget)
	{
		PlayHudWidget->AddToViewport(10);
	}
	OverlayWidget = CreateWidget<UHolypawBattleWidget>(PC);
	if (OverlayWidget)
	{
		OverlayWidget->AddToViewport(20);
	}
	MapWidget = CreateWidget<UHolypawMapWidget>(PC);
	if (MapWidget)
	{
		MapWidget->AddToViewport(30);
	}
	JournalWidget = CreateWidget<UHolypawJournalWidget>(PC);
	if (JournalWidget)
	{
		JournalWidget->AddToViewport(31);
	}
	TalkWidget = CreateWidget<UHolypawTalkWidget>(PC);
	if (TalkWidget)
	{
		TalkWidget->AddToViewport(32);
	}
	ShopWidget = CreateWidget<UHolypawShopWidget>(PC);
	if (ShopWidget)
	{
		ShopWidget->AddToViewport(33);
	}
	TitleWidget = CreateWidget<UHolypawTitleWidget>(PC);
	if (TitleWidget)
	{
		TitleWidget->AddToViewport(40);
	}
	PauseWidget = CreateWidget<UHolypawPauseWidget>(PC);
	if (PauseWidget)
	{
		PauseWidget->AddToViewport(41);
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
	if (!P || !Canvas)
	{
		return;
	}

	// Codex stays on canvas until it gets its own stitched book. Product surfaces live in UMG.
	if (!P->IsCodexOpen())
	{
		return;
	}

	const float W = Canvas->SizeX;
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
