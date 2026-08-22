#pragma once

#include "CoreMinimal.h"

class UObject;
class AActor;
class UAudioComponent;
enum class EHolypawZone : uint8;

namespace HolypawAudio
{
	void PlayCue(const UObject* WorldContext, FName Cue);
	void PlayTheme(const UObject* WorldContext, AActor* Owner, TObjectPtr<UAudioComponent>& Comp, EHolypawZone Zone, bool bInterior);
	void PlayHymn(const UObject* WorldContext, AActor* Owner, TObjectPtr<UAudioComponent>& Comp);
	void StopTheme(UAudioComponent* Comp);
}
