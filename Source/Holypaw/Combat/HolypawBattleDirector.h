#pragma once

#include "CoreMinimal.h"
#include "HolypawTypes.h"

/** Turn timing and outgoing modifiers. Battle still lives on the teddy overlay (keys 1–6). */
namespace HolypawBattleDirector
{
	constexpr float LullabyChance = 0.28f;
	constexpr float HitStopCrit = 0.95f;
	constexpr float HitStopNormal = 0.7f;

	struct FOutgoing
	{
		int32 Damage = 0;
		bool bStaggered = false;
		bool bFrostConsumed = false;
	};

	bool RollLullaby();
	float HitStopSeconds(bool bCrit, bool bStaggered);
	FOutgoing ApplyOutgoing(int32 Dmg, int32 FrostTurns, EVillainSpecial Special, bool bBeam, bool bCrit, FString& InOutLog);
}
