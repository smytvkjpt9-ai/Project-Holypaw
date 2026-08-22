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
		const float Salt = Human.ParadeSalt;
		Human.ChapelGoal += FVector((FMath::Fmod(Salt, 1.f) - 0.5f) * 220.f, (FMath::Fmod(Salt * 1.7f, 1.f) - 0.5f) * 160.f, 0.f);
		Human.PlazaGoal += FVector((FMath::Fmod(Salt * 2.1f, 1.f) - 0.5f) * 180.f, (FMath::Fmod(Salt * 0.8f, 1.f) - 0.5f) * 140.f, 0.f);
	}

	FVector ParadeGoal(const AHugHuman& Human, const float Hour, const int32 Hearts)
	{
		const bool bNight = Hour < 5.f || Hour >= 20.f;
		const bool bDusk = Hour >= 17.f && Hour < 20.f;
		if (Human.ParadeKick > 0.f && Human.bBeliever)
		{
			return bDusk ? Human.ChapelGoal : Human.PlazaGoal;
		}
		if (!Human.bBeliever)
		{
			if (bNight)
			{
				return Human.HomeLocation;
			}
			const float Orbit = HolypawFaith::ParadeOrbit(Hearts, false);
			const float Ang = Human.BounceT * 0.28f + Human.ParadeSalt;
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
		if (Human.bTendsStall && HolypawFaith::ShopsOpen(Hearts))
		{
			return Human.HomeLocation;
		}
		if (!HolypawFaith::BelieversParade(Hearts))
		{
			const float Orbit = HolypawFaith::ParadeOrbit(Hearts, true);
			const float Ang = Human.BounceT * 0.55f + Human.ParadeSalt;
			return Human.HomeLocation + FVector(FMath::Cos(Ang) * Orbit, FMath::Sin(Ang) * Orbit, 0.f);
		}
		const float Phase = FMath::Fmod(Human.BounceT * 0.14f + Human.ParadeSalt, 4.f);
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

	void WalkToward(AHugHuman& Human, FVector Goal, const float DeltaSeconds, const float Speed)
	{
		Goal.Z = Human.HomeLocation.Z;
		const FVector Here = Human.GetActorLocation();
		const FVector Next = FMath::VInterpConstantTo(Here, Goal, DeltaSeconds, Speed);
		const FVector Step = Next - Here;
		Human.SetActorLocation(Next);
		if (Step.SizeSquared2D() > 4.f)
		{
			const float Yaw = FMath::RadiansToDegrees(FMath::Atan2(Step.Y, Step.X));
			Human.SetActorRotation(FMath::RInterpTo(Human.GetActorRotation(), FRotator(0.f, Yaw, 0.f), DeltaSeconds, 8.f));
		}
	}

	void TickHuman(AHugHuman& Human, const float DeltaSeconds, const float Hour)
	{
		if (Human.IsKnelt())
		{
			return;
		}
		EnsureAnchors(Human);
		if (Human.ParadeKick > 0.f)
		{
			Human.ParadeKick = FMath::Max(0.f, Human.ParadeKick - DeltaSeconds);
		}
		const int32 Hearts = Human.bHomeZoneReady
			? HolypawFaith::HeartsAt(&Human, Human.HomeZone)
			: HolypawFaith::HeartsHere(&Human, Human.GetActorLocation());
		const FVector Goal = ParadeGoal(Human, Hour, Hearts);
		float Speed = 160.f;
		const bool bNight = Hour < 5.f || Hour >= 20.f;
		const bool bDusk = Hour >= 17.f && Hour < 20.f;
		if (Human.bBeliever)
		{
			if (Human.ParadeKick > 0.f)
			{
				Speed = 280.f;
			}
			else if (bNight)
			{
				Speed = 220.f;
			}
			else if (bDusk)
			{
				Speed = HolypawFaith::ChoirOwnsDusk(Hearts) ? 300.f : 240.f;
			}
			else if (HolypawFaith::BelieversParade(Hearts))
			{
				Speed = 240.f;
			}
			else
			{
				Speed = 180.f;
			}
		}
		else if (bNight)
		{
			Speed = 200.f;
		}
		WalkToward(Human, Goal, DeltaSeconds, Speed);
	}
}
