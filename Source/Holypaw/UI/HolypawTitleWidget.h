#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HolypawTitleWidget.generated.h"

UCLASS()
class HOLYPAW_API UHolypawTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

protected:
	void PaintText(FSlateWindowElementList& OutDrawElements, int32 LayerId, const FGeometry& AllottedGeometry,
		const FVector2D& Pos, const FString& Text, const FLinearColor& Color, float Scale) const;
	void PaintPanel(FSlateWindowElementList& OutDrawElements, int32 LayerId, const FGeometry& AllottedGeometry,
		const FVector2D& Pos, const FVector2D& Size, const FLinearColor& Color) const;
};
