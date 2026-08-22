#pragma once

#include "CoreMinimal.h"

class UObject;
class AActor;
class UAudioComponent;
enum class EHolypawZone : uint8;

namespace HolypawAudio
{
	void PlayCue(const UObject* WorldContext, FName Cue);
	void PlayAbility(const UObject* WorldContext, FName Kind, bool bCrit, bool bStagger);
	void PlayTheme(const UObject* WorldContext, AActor* Owner, TObjectPtr<UAudioComponent>& Comp, EHolypawZone Zone, bool bInterior);
	void StopTheme(UAudioComponent* Comp);
	void SetCombat(const UObject* WorldContext, bool bActive, bool bBoss, bool bPhaseTwo = false);
	void NotifyJumped(const UObject* WorldContext);
	void ApplyMute(const UObject* WorldContext);
	void RestartMix(const UObject* WorldContext);
}
