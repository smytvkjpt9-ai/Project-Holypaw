#pragma once

#include "CoreMinimal.h"
#include "UI/HolypawUiIcons.h"

class FSlateWindowElementList;
struct FGeometry;
struct FSlateBrush;

/**
 * Painterly / plush UI bible: rose, mint, gold, powder blue.
 * Panels are felt patches with gold stitch, not leftover HUD strings.
 */
namespace HolypawUi
{
	struct FPalette
	{
		FLinearColor Rose = FLinearColor(0.86f, 0.52f, 0.62f);
		FLinearColor Mint = FLinearColor(0.52f, 0.78f, 0.70f);
		FLinearColor Gold = FLinearColor(0.95f, 0.78f, 0.40f);
		FLinearColor Powder = FLinearColor(0.76f, 0.86f, 0.96f);
		FLinearColor Cream = FLinearColor(0.98f, 0.94f, 0.90f);
		FLinearColor Ink = FLinearColor(0.18f, 0.11f, 0.20f);
		FLinearColor Muted = FLinearColor(0.72f, 0.66f, 0.78f);
		FLinearColor Felt = FLinearColor(0.13f, 0.07f, 0.15f, 0.94f);
		FLinearColor FeltInner = FLinearColor(0.18f, 0.10f, 0.20f, 0.96f);
		FLinearColor Dim = FLinearColor(0.04f, 0.02f, 0.06f, 0.72f);
		FLinearColor Heart = FLinearColor(0.95f, 0.38f, 0.50f);
		FLinearColor Danger = FLinearColor(0.95f, 0.42f, 0.42f);
		FLinearColor Select = FLinearColor(0.42f, 0.22f, 0.36f, 0.96f);
		FLinearColor Idle = FLinearColor(0.16f, 0.09f, 0.18f, 0.82f);
		FLinearColor BarBack = FLinearColor(0.06f, 0.04f, 0.08f, 0.85f);
		FLinearColor HpFill = FLinearColor(0.92f, 0.42f, 0.55f);
		FLinearColor EnemyFill = FLinearColor(0.95f, 0.55f, 0.38f);
		FLinearColor Miracle = FLinearColor(0.95f, 0.74f, 0.38f);
		FLinearColor Sea = FLinearColor(0.28f, 0.42f, 0.58f, 0.55f);
	};

	const FPalette& Colors();
	const FSlateBrush* WhiteBrush();

	FLinearColor HeartsHeat(int32 Hearts);
	FLinearColor WithAlpha(FLinearColor C, float A);

	struct FPaint
	{
		FSlateWindowElementList& Out;
		const FGeometry& Geo;
		int32 Layer = 0;

		FVector2D Canvas() const;

		void Fill(FVector2D Pos, FVector2D Size, FLinearColor Color) const;
		void Text(FVector2D Pos, const FString& S, FLinearColor Color, float Scale, float MaxW = 980.f) const;
		void Text(FVector2D Pos, const FText& T, FLinearColor Color, float Scale, float MaxW = 980.f) const;
		void Line(FVector2D A, FVector2D B, FLinearColor Color, float Thickness = 2.f) const;
		void Poly(const TArray<FVector2f>& Pts, FLinearColor Color, float Thickness = 2.f) const;
		void DashRect(FVector2D Pos, FVector2D Size, FLinearColor Color, float Thickness = 1.7f, float Dash = 7.f) const;
		void CornerKnots(FVector2D Pos, FVector2D Size, FLinearColor Color) const;
		void Panel(FVector2D Pos, FVector2D Size, FLinearColor Fill = FLinearColor::Transparent) const;
		void SoftShadow(FVector2D Pos, FVector2D Size) const;
		void Chip(FVector2D Pos, FVector2D Size, EHolypawUiIcon Icon, const FString& Label, FLinearColor Accent) const;
		void Keycap(FVector2D Pos, const FString& Key) const;
		void Bar(FVector2D Pos, FVector2D Size, float Pct, FLinearColor FillColor, FLinearColor Back = FLinearColor::Transparent) const;
		void Icon(FVector2D Pos, float Size, EHolypawUiIcon Id, FLinearColor Color) const;
		void HeartRow(FVector2D Pos, int32 Hearts, float Size = 15.f, int32 MaxShow = 6) const;
		void Rule(FVector2D Pos, float Width, FLinearColor Color) const;
		void Caption(FVector2D Pos, EHolypawUiIcon Icon, const FString& Title, FLinearColor Accent) const;
		void Footer(FVector2D PanelPos, FVector2D PanelSize, const FString& Hint) const;
		void VerbRow(FVector2D Pos, const FString& Key, EHolypawUiIcon Icon, const FString& Label, bool bHot) const;
		void SlotCard(FVector2D Pos, FVector2D Size, bool bSelected, const FString& Title, const FString& Body) const;
	};

	FVector2D Centered(FVector2D Canvas, FVector2D Panel);
}
