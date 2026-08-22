#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HolypawJournalWidget.generated.h"

UCLASS()
class HOLYPAW_API UHolypawJournalWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};
