#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HolypawPlayerController.generated.h"

UCLASS()
class HOLYPAW_API AHolypawPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHolypawPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void OnEscape();
};
