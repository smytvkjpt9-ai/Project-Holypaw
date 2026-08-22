#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HolypawAudioRig.generated.h"

class UAudioComponent;
class USceneComponent;

UCLASS()
class HOLYPAW_API AHolypawAudioRig : public AActor
{
	GENERATED_BODY()

public:
	AHolypawAudioRig();

	UPROPERTY()
	TObjectPtr<USceneComponent> Root;

	UPROPERTY()
	TObjectPtr<UAudioComponent> MusicA;

	UPROPERTY()
	TObjectPtr<UAudioComponent> MusicB;

	UPROPERTY()
	TObjectPtr<UAudioComponent> CombatComp;

	UPROPERTY()
	TObjectPtr<UAudioComponent> Ambient;

	UPROPERTY()
	TObjectPtr<UAudioComponent> Mill;
};
