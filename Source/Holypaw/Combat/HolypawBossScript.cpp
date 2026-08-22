#include "Combat/HolypawBossScript.h"

namespace HolypawBossScript
{
	FBossIncoming ApplyIncoming(const EHolypawVillain Id, const bool bPhaseTwo, const int32 Turn)
	{
		FBossIncoming R;
		if (!bPhaseTwo && Turn < 3)
		{
			return R;
		}
		switch (Id)
		{
		case EHolypawVillain::SilkMagistrate:
			R.BonusDamage = bPhaseTwo ? 4 : 2;
			R.Extra = bPhaseTwo
				? TEXT("  Shears adjourn the stuffing.")
				: TEXT("  Court ribbon snaps taut.");
			break;
		case EHolypawVillain::BrineWarden:
			R.BonusDamage = bPhaseTwo ? 3 : 1;
			R.Extra = TEXT("  Salt law soaks the seams.");
			if (bPhaseTwo)
			{
				R.MillTurns = 2;
			}
			break;
		case EHolypawVillain::HarvestOverseer:
			R.BonusDamage = 2 + (Turn >= 5 ? 2 : 0);
			R.Extra = TEXT("  Thresh bell counts your stuffing.");
			break;
		case EHolypawVillain::BogKing:
			R.BonusDamage = bPhaseTwo ? 5 : 2;
			R.Extra = TEXT("  Peat stands up with him.");
			if (bPhaseTwo)
			{
				R.MillTurns = 3;
			}
			break;
		case EHolypawVillain::AuroraWarden:
			R.BonusDamage = bPhaseTwo ? 4 : 1;
			R.Extra = TEXT("  Sky-spear pins a paw.");
			break;
		case EHolypawVillain::VelvetTyrant:
			R.BonusDamage = bPhaseTwo ? 6 : 3;
			R.Extra = bPhaseTwo
				? TEXT("  The mountain unbuttons you.")
				: TEXT("  Velvet weight.");
			break;
		case EHolypawVillain::Unmaker:
			R.BonusDamage = 3 + FMath::Min(6, Turn);
			R.MillTurns = bPhaseTwo ? 3 : 1;
			R.Extra = TEXT("  It remembers your stuffing by name.");
			break;
		case EHolypawVillain::MiracleEater:
			R.BonusDamage = bPhaseTwo ? 5 : 2;
			R.MillTurns = 2;
			R.Extra = TEXT("  Choir bite. Polyester aftertaste.");
			break;
		default:
			if (bPhaseTwo)
			{
				R.BonusDamage = 2;
				R.Extra = TEXT("  Boss kit.");
			}
			break;
		}
		return R;
	}
}
