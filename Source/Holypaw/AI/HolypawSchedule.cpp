#include "AI/HolypawSchedule.h"
#include "Actors/HugHuman.h"
#include "Actors/HolypawShrine.h"
#include "Actors/FaithStall.h"
#include "Faith/HolypawFaithSim.h"
#include "EngineUtils.h"

namespace HolypawSchedule
{
	void EnsureAnchors(AHugHuman& Human)
	{
		if (Human.bAnchorsReady)
		{
			return;
		}
		Human.bAnchorsReady = true;
		Human.ChapelGoal = Human.HomeLocation + FVector(140.f, 90.f, 0.f);
		Human.InnGoal = Human.HomeLocation + FVector(-90.f, 160.f, 0.f);
		Human.PlazaGoal = Human.HomeLocation + FVector(220.f, -40.f, 0.f);
		Human.MarketGoal = Human.HomeLocation + FVector(-160.f, 180.f, 0.f);
		if (!Human.GetWorld())
		{
			return;
		}
		float BestChapel = TNumericLimits<float>::Max();
		float BestInn = TNumericLimits<float>::Max();
		float BestPlaza = TNumericLimits<float>::Max();
		for (TActorIterator<AHolypawShrine> It(Human.GetWorld()); It; ++It)
		{
			const AHolypawShrine* S = *It;
			if (!S)
			{
				continue;
			}
			const float Dist = FVector::Dist(Human.HomeLocation, S->GetActorLocation());
			if (Dist > 9000.f)
			{
				continue;
			}
			if (S->Kind == EHolypawShrineKind::Chapel && Dist < BestChapel)
			{
				BestChapel = Dist;
				Human.ChapelGoal = S->GetActorLocation() + FVector(0.f, -90.f, 0.f);
			}
			if (S->Kind == EHolypawShrineKind::Inn && Dist < BestInn)
			{
				BestInn = Dist;
				Human.InnGoal = S->GetActorLocation() + FVector(0.f, -90.f, 0.f);
			}
			if (S->Kind == EHolypawShrineKind::Wish && Dist < BestPlaza)
			{
				BestPlaza = Dist;
				Human.PlazaGoal = S->GetActorLocation() + FVector(80.f, 40.f, 0.f);
			}
		}
		float BestStall = TNumericLimits<float>::Max();
		for (TActorIterator<AFaithStall> It(Human.GetWorld()); It; ++It)
		{
			const AFaithStall* Stall = *It;
			if (!Stall)
			{
				continue;
			}
			const float Dist = FVector::Dist(Human.HomeLocation, Stall->GetActorLocation());
			if (Dist < BestStall && Dist < 9000.f)
			{
				BestStall = Dist;
				Human.MarketGoal = Stall->GetActorLocation() + FVector(-70.f, -40.f, 0.f);
			}
		}
	}

	FVector ParadeGoal(const AHugHuman& Human, const float Hour, const int32 Hearts)
	{
		const bool bNight = Hour < 5.f || Hour >= 20.f;
		const bool bDusk = Hour >= 17.f && Hour < 20.f;
		if (!Human.bBeliever)
		{
			if (bNight)
			{
				return Human.HomeLocation;
			}
			const float Orbit = HolypawFaith::ParadeOrbit(Hearts, false);
			const float Ang = Human.BounceT * 0.28f;
			return Human.HomeLocation + FVector(FMath::Cos(Ang) * Orbit, FMath::Sin(Ang) * Orbit, 0.f);
		}
		if (bNight)
		{
			return Human.InnGoal;
		}
		if (bDusk)
		{
			return Human.ChapelGoal;
		}
		if (!HolypawFaith::BelieversParade(Hearts))
		{
			const float Orbit = HolypawFaith::ParadeOrbit(Hearts, true);
			const float Ang = Human.BounceT * 0.55f;
			return Human.HomeLocation + FVector(FMath::Cos(Ang) * Orbit, FMath::Sin(Ang) * Orbit, 0.f);
		}
		const float Phase = FMath::Fmod(Human.BounceT * 0.14f, 4.f);
		if (Phase < 1.f)
		{
			return Human.PlazaGoal;
		}
		if (Phase < 2.f)
		{
			return Human.MarketGoal;
		}
		if (Phase < 3.f)
		{
			return Human.ChapelGoal;
		}
		return Human.HomeLocation;
	}

	void TickHuman(AHugHuman& Human, const float DeltaSeconds, const float Hour)
	{
		if (Human.IsKnelt())
		{
			return;
		}
		EnsureAnchors(Human);
		const int32 Hearts = HolypawFaith::HeartsHere(&Human, Human.GetActorLocation());
		FVector Goal = ParadeGoal(Human, Hour, Hearts);
		float Speed = 1.4f;
		const bool bNight = Hour < 5.f || Hour >= 20.f;
		const bool bDusk = Hour >= 17.f && Hour < 20.f;
		if (Human.bBeliever)
		{
			if (bNight)
			{
				Speed = 2.1f;
			}
			else if (bDusk)
			{
				Speed = HolypawFaith::ChoirOwnsDusk(Hearts) ? 2.8f : 2.4f;
			}
			else if (HolypawFaith::BelieversParade(Hearts))
			{
				Speed = 2.2f;
			}
			else
			{
				Speed = 1.6f;
			}
		}
		else if (bNight)
		{
			Speed = 1.8f;
		}
		Goal.Z = Human.HomeLocation.Z;
		Human.SetActorLocation(FMath::VInterpTo(Human.GetActorLocation(), Goal, DeltaSeconds, Speed));
	}
}
