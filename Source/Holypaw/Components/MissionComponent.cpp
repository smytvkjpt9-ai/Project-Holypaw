#include "Components/MissionComponent.h"
#include "Character/HolypawCharacter.h"

UHolypawMissionComponent::UHolypawMissionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FMissionDef UHolypawMissionComponent::GetCurrent() const
{
	const TArray<FMissionDef>& All = HolypawCatalog::GetMissions();
	if (All.IsValidIndex(CurrentIndex))
	{
		return All[CurrentIndex];
	}
	return All.Last();
}

bool UHolypawMissionComponent::HasZone(EHolypawZone Zone) const
{
	return ZonesVisited.Contains(Zone);
}

bool UHolypawMissionComponent::HasBoss(EHolypawVillain Id) const
{
	return BossesFell.Contains(Id);
}

int32 UHolypawMissionComponent::RiteCount() const
{
	int32 N = 0;
	if (HasBoss(EHolypawVillain::BrineWarden)) { ++N; }
	if (HasBoss(EHolypawVillain::HarvestOverseer)) { ++N; }
	if (HasBoss(EHolypawVillain::BogKing)) { ++N; }
	if (HasBoss(EHolypawVillain::AuroraWarden)) { ++N; }
	return N;
}

bool UHolypawMissionComponent::IsComplete(EHolypawMission Id) const
{
	switch (Id)
	{
	case EHolypawMission::Wake: return true;
	case EHolypawMission::FirstFriend: return Recruits >= 1;
	case EHolypawMission::FirstRip: return Kills >= 1;
	case EHolypawMission::FirstMiracle: return Miracles >= 1;
	case EHolypawMission::RibbonGates: return HasZone(EHolypawZone::RibbonCity);
	case EHolypawMission::ConvertThree: return Converts >= 3;
	case EHolypawMission::PolyCourt: return HasBoss(EHolypawVillain::SilkMagistrate);
	case EHolypawMission::OutlandRoads:
		return HasZone(EHolypawZone::Tidewell) && HasZone(EHolypawZone::Hearthfold)
			&& HasZone(EHolypawZone::Emberfen) && HasZone(EHolypawZone::Snowveil);
	case EHolypawMission::FourRites: return RiteCount() >= 4;
	case EHolypawMission::VelvetCrown: return HasBoss(EHolypawVillain::VelvetTyrant);
	case EHolypawMission::Unmake: return HasBoss(EHolypawVillain::Unmaker);
	case EHolypawMission::BearFaith: return bCampaignComplete;
	default: return false;
	}
}

void UHolypawMissionComponent::TryAdvance(AHolypawCharacter* Pawn)
{
	const TArray<FMissionDef>& All = HolypawCatalog::GetMissions();
	while (All.IsValidIndex(CurrentIndex) && IsComplete(All[CurrentIndex].Id) && !bCampaignComplete)
	{
		if (All[CurrentIndex].Id == EHolypawMission::BearFaith)
		{
			break;
		}
		if (Pawn)
		{
			Pawn->Toast(FString::Printf(TEXT("Mission complete: %s"), *All[CurrentIndex].Title.ToString()));
		}
		++CurrentIndex;
		if (!All.IsValidIndex(CurrentIndex))
		{
			CurrentIndex = All.Num() - 1;
			break;
		}
		if (Pawn)
		{
			Pawn->Toast(FString::Printf(TEXT("Next: %s"), *All[CurrentIndex].Title.ToString()));
		}
	}
}

void UHolypawMissionComponent::NotifyZone(EHolypawZone Zone)
{
	ZonesVisited.AddUnique(Zone);
	if (AHolypawCharacter* P = Cast<AHolypawCharacter>(GetOwner()))
	{
		TryAdvance(P);
	}
}

void UHolypawMissionComponent::NotifyRecruit()
{
	++Recruits;
	if (AHolypawCharacter* P = Cast<AHolypawCharacter>(GetOwner()))
	{
		TryAdvance(P);
	}
}

void UHolypawMissionComponent::NotifyKill(EHolypawVillain Id, EVillainRank Rank)
{
	++Kills;
	if (Rank == EVillainRank::Boss || Rank == EVillainRank::WorldBoss)
	{
		BossesFell.AddUnique(Id);
	}
	if (AHolypawCharacter* P = Cast<AHolypawCharacter>(GetOwner()))
	{
		TryAdvance(P);
	}
}

void UHolypawMissionComponent::NotifyConvert()
{
	++Converts;
	if (AHolypawCharacter* P = Cast<AHolypawCharacter>(GetOwner()))
	{
		TryAdvance(P);
	}
}

void UHolypawMissionComponent::NotifyMiracle(EHolypawZone Zone)
{
	++Miracles;
	if (HasBoss(EHolypawVillain::Unmaker) && (Zone == EHolypawZone::Highlands || Zone == EHolypawZone::Snow))
	{
		bCampaignComplete = true;
	}
	if (AHolypawCharacter* P = Cast<AHolypawCharacter>(GetOwner()))
	{
		TryAdvance(P);
		if (bCampaignComplete)
		{
			P->CompleteBearFaith();
		}
	}
}

TArray<FString> UHolypawMissionComponent::GetJournalLines() const
{
	TArray<FString> Lines;
	const TArray<FMissionDef>& All = HolypawCatalog::GetMissions();
	for (int32 I = 0; I < All.Num(); ++I)
	{
		const FMissionDef& M = All[I];
		FString Mark = TEXT(" ");
		if (I < CurrentIndex || (M.Id != EHolypawMission::BearFaith && IsComplete(M.Id)))
		{
			Mark = TEXT("x");
		}
		else if (I == CurrentIndex)
		{
			Mark = TEXT(">");
		}
		Lines.Add(FString::Printf(TEXT("%s  %s"), *Mark, *M.Title.ToString()));
	}
	const FMissionDef Cur = GetCurrent();
	Lines.Add(TEXT(""));
	Lines.Add(Cur.Brief.ToString());
	Lines.Add(Cur.Hint.ToString());
	Lines.Add(FString::Printf(TEXT("Recruits %d  Converts %d  Kills %d  Miracles %d  Rites %d/4"),
		Recruits, Converts, Kills, Miracles, RiteCount()));
	if (bCampaignComplete)
	{
		Lines.Add(TEXT("The Bear Faith holds. The Poly Mill's cheap empire is stuffing on the floor."));
	}
	return Lines;
}
