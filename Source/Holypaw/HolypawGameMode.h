#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HolypawGameMode.generated.h"

UCLASS()
class HOLYPAW_API AHolypawGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHolypawGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

protected:
	void EnsureWorldBuilder();
};
