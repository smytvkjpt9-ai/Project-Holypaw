#include "Combat/HolypawBattleDirector.h"
#include "Combat/HolypawBattleMath.h"

namespace HolypawBattleDirector
{
	bool RollLullaby()
	{
		return FMath::FRand() < LullabyChance;
	}

	float HitStopSeconds(const bool bCrit, const bool bStaggered)
	{
		return (bCrit || bStaggered) ? HitStopCrit : HitStopNormal;
	}

	float FleeChance(const bool bHaloStep, const bool bBlocksFlee, const bool bBoss)
	{
		if (bBlocksFlee || bBoss)
		{
			return FleeBoss + (bHaloStep ? 0.08f : 0.f);
		}
		return FleeBase + (bHaloStep ? 0.15f : 0.f);
	}

	int32 AbilityFpCost(const FName Kind)
	{
		if (const FHolypawAbilityDef* A = HolypawCatalog::FindAbility(Kind))
		{
			return A->FpCost;
		}
		return 0;
	}

	int32 AbilityStitch(const FName Kind)
	{
		if (const FHolypawAbilityDef* A = HolypawCatalog::FindAbility(Kind))
		{
			return A->Stitch;
		}
		return 0;
	}

	FOutgoing ApplyOutgoing(int32 Dmg, const int32 FrostTurns, const EVillainSpecial Special, const bool bBeam, const bool bCrit, FString& InOutLog)
	{
		FOutgoing O;
		if (FrostTurns > 0)
		{
			Dmg = HolypawBattle::ScaleForFrost(Dmg, FrostTurns);
			O.bFrostConsumed = true;
			InOutLog += TEXT(" Frost slows the paw.");
		}

		const int32 BeforeArmor = Dmg;
		Dmg = HolypawBattle::ScaleForArmor(Dmg, Special, bBeam);
		if (Dmg < BeforeArmor)
		{
			InOutLog += bBeam
				? TEXT(" Beam slips the plates.")
				: TEXT(" Armor plates dull it.");
		}

		O.bStaggered = HolypawBattle::ShouldStagger(Dmg, bCrit);
		if (O.bStaggered)
		{
			InOutLog += TEXT(" Staggered!");
		}
		O.Damage = Dmg;
		return O;
	}

	FIncomingResult ApplyIncoming(const FIncomingRequest& Req)
	{
		FIncomingResult R;
		R.HymnShieldLeft = Req.HymnShield;
		int32 Dmg = Req.BaseDamage;
		if (Req.bStaggered)
		{
			Dmg = FMath::Max(1, Dmg / 2);
			R.Extra = TEXT("  Staggers. Special fumbles.");
			R.bClearStagger = true;
		}
		else
		{
			switch (Req.Special)
			{
			case EVillainSpecial::DrainFaith:
			{
				const int32 Drain = FMath::Min(6, Req.PlayerFP);
				if (Drain > 0)
				{
					R.FpDrain = Drain;
					R.Extra = FString::Printf(TEXT("  -%d FP."), Drain);
				}
				break;
			}
			case EVillainSpecial::StealMiracle:
			{
				const float Stolen = FMath::Min(10.f, Req.MiracleCharge);
				R.MiracleStolen = Stolen;
				R.Extra = TEXT("  Miracle Charge nipped.");
				break;
			}
			case EVillainSpecial::DoubleStrike:
				Dmg += Req.EnemyAttack / 2 + FMath::RandRange(0, 2);
				R.Extra = TEXT("  Twice!");
				break;
			case EVillainSpecial::FrostBite:
				Dmg += 2;
				R.FrostTurns = 3;
				R.Extra = TEXT("  Seams go numb. (frost)");
				break;
			case EVillainSpecial::PoisonThread:
				Dmg += 3;
				R.PoisonTurns = 3;
				R.Extra = TEXT("  Poison thread itches.");
				break;
			case EVillainSpecial::Rage:
				if (Req.EnemyHP * 2 <= Req.EnemyHPMax)
				{
					Dmg = FMath::FloorToInt(Dmg * 1.5f);
					R.Extra = TEXT("  RAGE.");
				}
				break;
			case EVillainSpecial::ThreadCut:
				R.bPartyCut = true;
				R.Extra = TEXT("  Party ribbons snipped.");
				break;
			case EVillainSpecial::FaithBurn:
				Dmg += FMath::Max(0, Req.PlayerFP / 8);
				R.Extra = TEXT("  Faith flares against you.");
				break;
			default:
				break;
			}
		}

		if (Req.Rank == EVillainRank::WorldBoss && Req.BattleTurn >= 4)
		{
			Dmg += 4;
			R.Extra += TEXT("  The air unravels.");
		}
		if (Req.bPhaseTwo)
		{
			Dmg += 2 + Req.EnemyAttack / 5;
			R.Extra += TEXT("  Phase two.");
		}
		if (Req.bFaithArmor)
		{
			Dmg = FMath::Max(1, Dmg - 2);
		}
		if (Req.bFluffShield)
		{
			Dmg = FMath::Max(1, Dmg - 3);
		}
		if (Req.bGuarding)
		{
			Dmg = HolypawBattle::ScaleForGuard(Dmg, Req.bSeamGuard);
			R.bClearGuard = true;
			R.Extra += TEXT("  Guarded.");
		}
		if (Req.HymnShield > 0)
		{
			Dmg = HolypawBattle::ScaleForHymnShield(Dmg);
			R.HymnShieldLeft = Req.HymnShield - 1;
			R.Extra += TEXT("  Hymn shield.");
		}
		R.Damage = Dmg;
		return R;
	}
}
