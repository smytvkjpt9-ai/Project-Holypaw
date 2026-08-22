#pragma once

#include "CoreMinimal.h"
#include "HolypawTypes.h"

/** Pure combat helpers. Timing and outgoing mods live in HolypawBattleDirector. */
namespace HolypawBattle
{
	constexpr float CritChance = 0.12f;

	bool RollCrit();
	int32 ScaleForArmor(int32 Dmg, EVillainSpecial Special, bool bBeam);
	int32 ScaleForFrost(int32 Dmg, int32 FrostTurns);
	int32 ScaleForHymnShield(int32 Dmg);
	int32 ScaleForGuard(int32 Dmg, bool bSeamGuard);
	int32 SlapComboBonus(int32 Combo);
	bool ShouldStagger(int32 Dmg, bool bCrit);
	FString FormatStatus(int32 SlapCombo, int32 PoisonTurns, int32 FrostTurns, int32 HymnShield, bool bStaggered);
	FString BossPhaseLine(EHolypawVillain Id, const FString& DisplayName);
}
