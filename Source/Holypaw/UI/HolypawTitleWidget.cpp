#include "UI/HolypawTitleWidget.h"
#include "HolypawGameInstance.h"
#include "Save/HolypawSaveGame.h"
#include "Character/HolypawCharacter.h"
#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"
#include "Layout/SlateLayoutTransform.h"
#include "Brushes/SlateColorBrush.h"

void UHolypawTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UHolypawTitleWidget::PaintText(FSlateWindowElementList& OutDrawElements, int32 LayerId, const FGeometry& AllottedGeometry,
	const FVector2D& Pos, const FString& Text, const FLinearColor& Color, float Scale) const
{
	FSlateFontInfo Font = FCoreStyle::GetDefaultFontStyle("Bold", FMath::Max(10, FMath::RoundToInt(16.f * Scale)));
	const FPaintGeometry Geo = AllottedGeometry.ToPaintGeometry(
		FVector2f(1100.f, 44.f),
		FSlateLayoutTransform(FVector2f(Pos.X, Pos.Y)));
	FSlateDrawElement::MakeText(OutDrawElements, LayerId, Geo, Text, Font, ESlateDrawEffect::None, Color);
}

void UHolypawTitleWidget::PaintPanel(FSlateWindowElementList& OutDrawElements, int32 LayerId, const FGeometry& AllottedGeometry,
	const FVector2D& Pos, const FVector2D& Size, const FLinearColor& Color) const
{
	static FSlateColorBrush White(FLinearColor::White);
	const FPaintGeometry Geo = AllottedGeometry.ToPaintGeometry(
		FVector2f(Size.X, Size.Y),
		FSlateLayoutTransform(FVector2f(Pos.X, Pos.Y)));
	FSlateDrawElement::MakeBox(OutDrawElements, LayerId, Geo, &White, ESlateDrawEffect::None, Color);
}

int32 UHolypawTitleWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const AHolypawCharacter* P = Cast<AHolypawCharacter>(GetOwningPlayerPawn());
	if (!P || (P->Mode != EHolypawPawnMode::Title && P->Mode != EHolypawPawnMode::Pause))
	{
		return Layer;
	}

	const UHolypawGameInstance* GI = UHolypawGameInstance::Get(P);
	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const float CX = Size.X * 0.5f;

	PaintPanel(OutDrawElements, Layer, AllottedGeometry, FVector2D(0.f, 0.f), Size, FLinearColor(0.08f, 0.04f, 0.10f, 0.82f));
	PaintPanel(OutDrawElements, Layer + 1, AllottedGeometry, FVector2D(CX - 420.f, 90.f), FVector2D(840.f, Size.Y - 180.f), FLinearColor(0.14f, 0.08f, 0.16f, 0.92f));

	if (P->Mode == EHolypawPawnMode::Pause)
	{
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 90.f, 130.f), TEXT("Paused"), FLinearColor(1.f, 0.82f, 0.9f), 1.8f);
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 280.f, 210.f), TEXT("The humans freeze. They were mid-opinion."), FLinearColor(0.9f, 0.82f, 1.f), 1.05f);
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 260.f, 280.f), TEXT("Enter / Esc   resume"), FLinearColor(1.f, 0.92f, 0.8f), 1.15f);
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 260.f, 320.f), TEXT("F5            save this slot"), FLinearColor(0.85f, 0.9f, 1.f), 1.15f);
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 260.f, 360.f), TEXT("F8            title (progress stays saved)"), FLinearColor(0.85f, 0.9f, 1.f), 1.15f);
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 260.f, 400.f), TEXT("F6            mute"), FLinearColor(0.85f, 0.9f, 1.f), 1.15f);
		if (GI && GI->Settings)
		{
			PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 260.f, 460.f),
				GI->Settings->bMuted ? TEXT("Audio  MUTED") : TEXT("Audio  ON  ·  music / ambient / combat buses"),
				FLinearColor(1.f, 0.85f, 0.55f), 1.0f);
		}
		return Layer + 3;
	}

	PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 280.f, 120.f), TEXT("The Fluffy Ascendancy"), FLinearColor(1.f, 0.78f, 0.88f), 1.7f);
	PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 300.f, 168.f), TEXT("A handmade teddy. A polyester empire. A hug coup."), FLinearColor(0.92f, 0.84f, 1.f), 1.0f);

	const int32 Cursor = GI ? GI->TitleCursor : 0;
	for (int32 I = 0; I < UHolypawGameInstance::SlotCount; ++I)
	{
		const bool Sel = (I == Cursor);
		const FString Line = FString::Printf(TEXT("%d   Slot %d   %s"), I + 1, I + 1, GI ? *GI->SlotSummary(I) : TEXT("..."));
		PaintPanel(OutDrawElements, Layer + 1, AllottedGeometry,
			FVector2D(CX - 360.f, 230.f + I * 78.f), FVector2D(720.f, 64.f),
			Sel ? FLinearColor(0.42f, 0.22f, 0.38f, 0.95f) : FLinearColor(0.18f, 0.10f, 0.20f, 0.8f));
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 340.f, 248.f + I * 78.f), Line,
			Sel ? FLinearColor(1.f, 0.92f, 0.75f) : FLinearColor(0.85f, 0.78f, 0.9f), 1.05f);
	}

	PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 340.f, 480.f), TEXT("1-3 select   Enter continue   N new   L load"), FLinearColor(0.95f, 0.88f, 1.f), 1.0f);
	PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 340.f, 512.f), TEXT("F6 mute   F5 saves after you start   Cottage porch waits behind this menu"), FLinearColor(0.75f, 0.7f, 0.85f), 0.9f);
	if (GI && GI->Settings)
	{
		PaintText(OutDrawElements, Layer + 2, AllottedGeometry, FVector2D(CX - 340.f, 548.f),
			GI->Settings->bMuted ? TEXT("Mute ON") : TEXT("Mute OFF"), FLinearColor(1.f, 0.85f, 0.55f), 0.95f);
	}
	return Layer + 3;
}
