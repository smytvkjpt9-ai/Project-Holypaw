#include "AI/HolypawSchedule.h"
#include "Actors/HugHuman.h"
#include "Actors/HolypawShrine.h"
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
		if (!Human.GetWorld())
		{
			return;
		}
		float BestChapel = TNumericLimits<float>::Max();
		float BestInn = TNumericLimits<float>::Max();
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
		}
	}

	void TickHuman(AHugHuman& Human, const float DeltaSeconds, const float Hour)
	{
		if (Human.IsKnelt())
		{
			return;
		}
		EnsureAnchors(Human);
		FVector Goal = Human.HomeLocation;
		float Speed = 1.4f;
		const bool bNight = Hour < 5.f || Hour >= 20.f;
		const bool bDusk = Hour >= 17.f && Hour < 20.f;
		if (Human.bBeliever)
		{
			if (bNight)
			{
				Goal = Human.InnGoal;
				Speed = 2.1f;
			}
			else if (bDusk)
			{
				Goal = Human.ChapelGoal;
				Speed = 2.4f;
			}
			else
			{
				const float Orbit = 160.f;
				const float Ang = Human.BounceT * 0.55f;
				Goal = Human.HomeLocation + FVector(FMath::Cos(Ang) * Orbit, FMath::Sin(Ang) * Orbit, 0.f);
			}
		}
		else
		{
			if (bNight)
			{
				Goal = Human.HomeLocation;
				Speed = 1.8f;
			}
			else
			{
				const float Orbit = 90.f;
				const float Ang = Human.BounceT * 0.28f;
				Goal = Human.HomeLocation + FVector(FMath::Cos(Ang) * Orbit, FMath::Sin(Ang) * Orbit, 0.f);
			}
		}
		Goal.Z = Human.HomeLocation.Z;
		Human.SetActorLocation(FMath::VInterpTo(Human.GetActorLocation(), Goal, DeltaSeconds, Speed));
	}
}
