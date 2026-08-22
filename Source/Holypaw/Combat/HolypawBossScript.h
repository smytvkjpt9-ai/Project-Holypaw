#pragma once

#include "CoreMinimal.h"
#include "HolypawTypes.h"

/** Authored phase-two extras for the eight campaign bosses. Overlay keys stay 1–6. */
namespace HolypawBossScript
{
	struct FBossIncoming
	{
		int32 BonusDamage = 0;
		int32 MillTurns = 0;
		FString Extra;
	};

	FBossIncoming ApplyIncoming(EHolypawVillain Id, bool bPhaseTwo, int32 Turn);
}
