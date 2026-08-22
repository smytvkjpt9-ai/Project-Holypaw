#pragma once

#include "CoreMinimal.h"
#include "HolypawTypes.h"

/** Turn timing, outgoing mods, incoming specials. Overlay keys stay 1–6. */
namespace HolypawBattleDirector
{
	constexpr float LullabyChance = 0.28f;
	constexpr float HitStopCrit = 0.95f;
	constexpr float HitStopNormal = 0.7f;
	constexpr float FleeBase = 0.55f;
	constexpr float FleeBoss = 0.12f;

	struct FOutgoing
	{
		int32 Damage = 0;
		bool bStaggered = false;
		bool bFrostConsumed = false;
	};

	struct FIncomingRequest
	{
		int32 BaseDamage = 0;
		EVillainSpecial Special = EVillainSpecial::Rip;
		EVillainRank Rank = EVillainRank::Minion;
		int32 EnemyAttack = 0;
		int32 EnemyHP = 1;
		int32 EnemyHPMax = 1;
		bool bPhaseTwo = false;
		int32 BattleTurn = 0;
		bool bStaggered = false;
		bool bGuarding = false;
		bool bSeamGuard = false;
		int32 HymnShield = 0;
		bool bFaithArmor = false;
		bool bFluffShield = false;
		int32 PlayerFP = 0;
		float MiracleCharge = 0.f;
		EHolypawVillain VillainId = EHolypawVillain::ScrapDog;
	};

	struct FIncomingResult
	{
		int32 Damage = 0;
		FString Extra;
		int32 FrostTurns = 0;
		int32 PoisonTurns = 0;
		bool bPartyCut = false;
		bool bClearGuard = false;
		int32 HymnShieldLeft = 0;
		int32 FpDrain = 0;
		float MiracleStolen = 0.f;
		bool bClearStagger = false;
		int32 MillTurns = 0;
	};

	bool RollLullaby();
	float HitStopSeconds(bool bCrit, bool bStaggered);
	float FleeChance(bool bHaloStep, bool bBlocksFlee, bool bBoss);
	int32 AbilityFpCost(FName Kind);
	int32 AbilityStitch(FName Kind);
	FOutgoing ApplyOutgoing(int32 Dmg, int32 FrostTurns, EVillainSpecial Special, bool bBeam, bool bCrit, FString& InOutLog);
	FIncomingResult ApplyIncoming(const FIncomingRequest& Req);
}
