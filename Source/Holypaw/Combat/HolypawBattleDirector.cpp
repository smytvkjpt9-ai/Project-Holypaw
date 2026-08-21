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
}
