#include "Save/HolypawSaveCodec.h"
#include "Save/HolypawSaveGame.h"
#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"
#include "Components/SkillTreeComponent.h"
#include "Components/PartyComponent.h"
#include "Components/MissionComponent.h"
#include "Actors/HugHuman.h"
#include "HolypawWorldBuilder.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"

namespace HolypawSaveCodec
{
	void Capture(AHolypawCharacter* Pawn, UHolypawSaveGame& Out)
	{
		if (!Pawn)
		{
			return;
		}
		Out.SaveVersion = 1;
		Out.bOccupied = true;
		Out.SavedAt = FDateTime::UtcNow();
		Out.Location = Pawn->GetActorLocation();
		Out.Rotation = Pawn->GetActorRotation();
		Out.ZoneName = HolypawCatalog::ZoneDisplayName(Pawn->CurrentZone);
		Out.HP = Pawn->HP;
		Out.HPMax = Pawn->HPMax;
		Out.Attack = Pawn->Attack;
		Out.WalkSpeed = Pawn->WalkSpeed;
		Out.bHaloVisible = Pawn->HaloMesh && !Pawn->HaloMesh->bHiddenInGame;

		if (Pawn->Affection)
		{
			Out.AP = Pawn->Affection->AP;
			Out.FP = Pawn->Affection->FP;
			Out.MiracleCharge = Pawn->Affection->MiracleCharge;
			Out.Level = Pawn->Affection->Level;
		}
		if (Pawn->Party)
		{
			Out.Party = Pawn->Party->Members;
		}
		if (Pawn->Skills)
		{
			Out.ActiveTree = Pawn->Skills->ActiveTree;
			Out.Skills.Reset();
			for (const FSkillDef& Def : Pawn->Skills->GetCatalog())
			{
				FHolypawSkillRecord Rec;
				Rec.Id = Def.Id;
				Rec.bOwned = Pawn->Skills->HasSkill(Def.Id);
				Out.Skills.Add(Rec);
			}
		}
		if (Pawn->Story)
		{
			Out.MissionIndex = Pawn->Story->CurrentIndex;
			Out.Recruits = Pawn->Story->Recruits;
			Out.Kills = Pawn->Story->Kills;
			Out.Converts = Pawn->Story->Converts;
			Out.Miracles = Pawn->Story->Miracles;
			Out.bCampaignComplete = Pawn->Story->bCampaignComplete;
			Out.ZonesVisited = Pawn->Story->ZonesVisited;
			Out.BossesFell = Pawn->Story->BossesFell;
			Out.MissionTitle = Pawn->Story->GetCurrent().Title.ToString();
			Out.TotalHearts = Pawn->Story->Converts;
		}
		Out.SeenVillains = Pawn->GetSeenVillains();
		Out.DefeatedVillains = Pawn->GetDefeatedVillains();
		Out.CityHearts = Pawn->GetHeartRecords();
		Out.UnlockedTravel = Pawn->GetUnlockedTravel();

		Out.Humans.Reset();
		if (UWorld* World = Pawn->GetWorld())
		{
			for (TActorIterator<AHugHuman> It(World); It; ++It)
			{
				AHugHuman* H = *It;
				if (!H)
				{
					continue;
				}
				FHolypawHumanRecord Rec;
				Rec.PersonName = H->PersonName.ToString();
				Rec.ConvertProgress = H->ConvertProgress;
				Rec.bBeliever = H->bBeliever;
				Rec.bKnelt = H->IsKnelt();
				Out.Humans.Add(Rec);
			}
		}
	}

	void Apply(AHolypawCharacter* Pawn, const UHolypawSaveGame& In)
	{
		if (!Pawn)
		{
			return;
		}
		ResetWorldFaith(Pawn);

		Pawn->HP = In.HP;
		Pawn->HPMax = In.HPMax;
		Pawn->Attack = In.Attack;
		Pawn->WalkSpeed = In.WalkSpeed;
		if (UCharacterMovementComponent* Move = Pawn->GetCharacterMovement())
		{
			Move->MaxWalkSpeed = Pawn->WalkSpeed;
		}
		if (Pawn->HaloMesh)
		{
			Pawn->HaloMesh->SetHiddenInGame(!In.bHaloVisible);
		}
		if (Pawn->Affection)
		{
			Pawn->Affection->AP = In.AP;
			Pawn->Affection->FP = In.FP;
			Pawn->Affection->MiracleCharge = In.MiracleCharge;
			Pawn->Affection->Level = In.Level;
		}
		if (Pawn->Party)
		{
			Pawn->Party->Members = In.Party;
		}
		if (Pawn->Skills)
		{
			Pawn->Skills->ActiveTree = In.ActiveTree;
			TArray<FName> OwnedIds;
			for (const FHolypawSkillRecord& Rec : In.Skills)
			{
				if (Rec.bOwned)
				{
					OwnedIds.Add(Rec.Id);
				}
			}
			Pawn->Skills->ReplaceOwned(OwnedIds);
		}
		if (Pawn->Story)
		{
			Pawn->Story->CurrentIndex = In.MissionIndex;
			Pawn->Story->Recruits = In.Recruits;
			Pawn->Story->Kills = In.Kills;
			Pawn->Story->Converts = In.Converts;
			Pawn->Story->Miracles = In.Miracles;
			Pawn->Story->bCampaignComplete = In.bCampaignComplete;
			Pawn->Story->ZonesVisited = In.ZonesVisited;
			Pawn->Story->BossesFell = In.BossesFell;
		}
		Pawn->SetCodex(In.SeenVillains, In.DefeatedVillains);
		Pawn->SetHeartRecords(In.CityHearts);
		Pawn->SetUnlockedTravel(In.UnlockedTravel);

		FVector Loc = In.Location;
		if (Loc.IsNearlyZero())
		{
			for (TActorIterator<AHolypawWorldBuilder> It(Pawn->GetWorld()); It; ++It)
			{
				Loc = It->GetCottageSpawn();
				break;
			}
		}
		Pawn->SetActorLocation(Loc);
		Pawn->SetActorRotation(In.Rotation);

		if (UWorld* World = Pawn->GetWorld())
		{
			for (TActorIterator<AHugHuman> It(World); It; ++It)
			{
				AHugHuman* H = *It;
				if (!H)
				{
					continue;
				}
				const FString Name = H->PersonName.ToString();
				const FHolypawHumanRecord* Rec = In.Humans.FindByPredicate([&](const FHolypawHumanRecord& R)
				{
					return R.PersonName == Name;
				});
				if (Rec)
				{
					H->RestoreFaith(Rec->ConvertProgress, Rec->bBeliever, Rec->bKnelt);
				}
			}
		}

		if (In.bCampaignComplete)
		{
			Pawn->CompleteBearFaith();
		}
	}

	void ResetWorldFaith(AHolypawCharacter* Pawn)
	{
		if (!Pawn || !Pawn->GetWorld())
		{
			return;
		}
		for (TActorIterator<AHugHuman> It(Pawn->GetWorld()); It; ++It)
		{
			if (AHugHuman* H = *It)
			{
				H->ResetFaith();
			}
		}
	}
}
