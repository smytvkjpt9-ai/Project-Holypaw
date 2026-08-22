#include "Combat/HolypawBattleMath.h"

namespace HolypawBattle
{
	bool RollCrit()
	{
		return FMath::FRand() < CritChance;
	}

	int32 ScaleForArmor(int32 Dmg, const EVillainSpecial Special, const bool bBeam)
	{
		if (Special != EVillainSpecial::ArmorPlates)
		{
			return Dmg;
		}
		const float Keep = bBeam ? 0.92f : 0.72f;
		return FMath::Max(1, FMath::FloorToInt(Dmg * Keep));
	}

	int32 ScaleForFrost(int32 Dmg, const int32 FrostTurns)
	{
		if (FrostTurns <= 0)
		{
			return Dmg;
		}
		return FMath::Max(1, FMath::FloorToInt(Dmg * 0.7f));
	}

	int32 ScaleForHymnShield(int32 Dmg)
	{
		return FMath::Max(1, Dmg / 2);
	}

	int32 ScaleForGuard(int32 Dmg, const bool bSeamGuard)
	{
		const float Keep = bSeamGuard ? 0.25f : 0.45f;
		return FMath::Max(1, FMath::FloorToInt(Dmg * Keep));
	}

	int32 SlapComboBonus(const int32 Combo)
	{
		if (Combo < 2)
		{
			return 0;
		}
		return FMath::Min(8, Combo * 2);
	}

	bool ShouldStagger(const int32 Dmg, const bool bCrit)
	{
		return bCrit || Dmg >= 16;
	}

	FString FormatStatus(const int32 SlapCombo, const int32 PoisonTurns, const int32 FrostTurns, const int32 HymnShield, const bool bStaggered)
	{
		TArray<FString> Bits;
		if (SlapCombo >= 2)
		{
			Bits.Add(FString::Printf(TEXT("Combo x%d"), SlapCombo));
		}
		if (PoisonTurns > 0)
		{
			Bits.Add(FString::Printf(TEXT("Poison %d"), PoisonTurns));
		}
		if (FrostTurns > 0)
		{
			Bits.Add(FString::Printf(TEXT("Frost %d"), FrostTurns));
		}
		if (HymnShield > 0)
		{
			Bits.Add(FString::Printf(TEXT("Hymn shield %d"), HymnShield));
		}
		if (bStaggered)
		{
			Bits.Add(TEXT("Staggered"));
		}
		FString Out;
		for (int32 I = 0; I < Bits.Num(); ++I)
		{
			if (I > 0)
			{
				Out += TEXT("  ·  ");
			}
			Out += Bits[I];
		}
		return Out;
	}

	FString BossPhaseLine(const EHolypawVillain Id, const FString& DisplayName)
	{
		switch (Id)
		{
		case EHolypawVillain::SilkMagistrate:
			return TEXT("Silk Magistrate adjourns into shears. Phase two!");
		case EHolypawVillain::BrineWarden:
			return TEXT("Brine Warden floods the dock with salt law. Phase two!");
		case EHolypawVillain::HarvestOverseer:
			return TEXT("Harvest Overseer rings a mill bell. Phase two!");
		case EHolypawVillain::BogKing:
			return TEXT("Bog King stands up. The peat stands up with him. Phase two!");
		case EHolypawVillain::AuroraWarden:
			return TEXT("Aurora Warden pins the sky to a spear. Phase two!");
		case EHolypawVillain::VelvetTyrant:
			return TEXT("Velvet Tyrant unbuttons the mountain. Phase two!");
		case EHolypawVillain::Unmaker:
			return TEXT("The Unmaker remembers your stuffing by name. Phase two!");
		case EHolypawVillain::MiracleEater:
			return TEXT("Miracle Eater bites the choir in half. Phase two!");
		default:
			return DisplayName + TEXT(" rips a seam in the sky. Phase two!");
		}
	}
}
