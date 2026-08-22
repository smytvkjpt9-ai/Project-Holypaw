#include "HolypawWorldBuilder.h"
#include "Holypaw.h"
#include "HolypawGameInstance.h"
#include "Save/HolypawSaveGame.h"
#include "Audio/HolypawAudio.h"
#include "Character/HolypawCharacter.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void AHolypawWorldBuilder::DressCity(const EHolypawZone Zone)
{
	if (DressedCities.Contains(Zone))
	{
		return;
	}
	switch (Zone)
	{
	case EHolypawZone::RibbonCity:
		BuildRibbonDistricts();
		break;
	case EHolypawZone::Tidewell:
		BuildTidewellDistricts();
		break;
	case EHolypawZone::Hearthfold:
		BuildHearthfoldDistricts();
		break;
	case EHolypawZone::Emberfen:
		BuildEmberfenDistricts();
		break;
	case EHolypawZone::Snowveil:
		BuildSnowveilDistricts();
		break;
	case EHolypawZone::LanternAngeles:
		BuildLanternAngelesDistricts();
		break;
	case EHolypawZone::Mossgate:
		BuildMossgateDistricts();
		break;
	case EHolypawZone::PalmaDusk:
		BuildPalmaDuskDistricts();
		break;
	case EHolypawZone::CherryLoom:
		BuildCherryLoomDistricts();
		break;
	case EHolypawZone::Quiltland:
		BuildQuiltlandDistricts();
		break;
	case EHolypawZone::DustMesa:
		BuildDustMesaDistricts();
		break;
	case EHolypawZone::Clockhaven:
		BuildClockhavenDistricts();
		break;
	case EHolypawZone::VelvetSeine:
		BuildVelvetSeineDistricts();
		break;
	case EHolypawZone::MarbleForum:
		BuildMarbleForumDistricts();
		break;
	case EHolypawZone::IvorySpire:
		BuildIvorySpireDistricts();
		break;
	case EHolypawZone::SandHymn:
		BuildSandHymnDistricts();
		break;
	case EHolypawZone::CapePlush:
		BuildCapePlushDistricts();
		break;
	case EHolypawZone::SavannahBell:
		BuildSavannahBellDistricts();
		break;
	case EHolypawZone::CarnivalBahia:
		BuildCarnivalBahiaDistricts();
		break;
	case EHolypawZone::AndesLoom:
		BuildAndesLoomDistricts();
		break;
	case EHolypawZone::SilkDelta:
		BuildSilkDeltaDistricts();
		break;
	case EHolypawZone::SpiceHarbor:
		BuildSpiceHarborDistricts();
		break;
	case EHolypawZone::CoralChoir:
		BuildCoralChoirDistricts();
		break;
	case EHolypawZone::AuroraBorough:
		BuildAuroraBoroughDistricts();
		break;
	case EHolypawZone::TundraParish:
		BuildTundraParishDistricts();
		break;
	case EHolypawZone::FeltIceCamp:
		BuildFeltIceCampDistricts();
		break;
	default:
		return;
	}
	DressedCities.Add(Zone);
	UE_LOG(LogHolypaw, Log, TEXT("Holypaw stream dressed %s (%d/26)"), HolypawCatalog::ZoneDisplayName(Zone), DressedCities.Num());
}

void AHolypawWorldBuilder::RequestDress(const EHolypawZone Zone)
{
	if (!HolypawCatalog::IsCityZone(Zone))
	{
		return;
	}
	DressCity(Zone);
}

void AHolypawWorldBuilder::TickWorldStream()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Pawn)
	{
		return;
	}
	const FVector2D P(Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y);
	EHolypawZone Best = EHolypawZone::ForestCottage;
	float BestDist = TNumericLimits<float>::Max();
	for (const FHolypawCity& C : HolypawCatalog::GetCities())
	{
		if (DressedCities.Contains(C.Zone))
		{
			continue;
		}
		const float Dist = FVector2D::Distance(P, C.Pos);
		const float Ring = FMath::Max(C.Radius * 1.55f, 14000.f);
		if (Dist < Ring && Dist < BestDist)
		{
			BestDist = Dist;
			Best = C.Zone;
		}
	}
	if (HolypawCatalog::IsCityZone(Best))
	{
		DressCity(Best);
	}
	RefreshCityTheme();
}

void AHolypawWorldBuilder::RefreshCityTheme()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Pawn)
	{
		return;
	}
	if (const UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		if (GI->Settings && GI->Settings->bMuted)
		{
			HolypawAudio::StopTheme(ThemeComp.Get());
			return;
		}
	}
	const EHolypawZone Z = ResolveZone(Pawn->GetActorLocation());
	const bool bIndoor = IsPlayerIndoors(Pawn->GetActorLocation());
	if (Z == ThemeZone && bIndoor == bThemeInterior && ThemeComp && ThemeComp->IsPlaying())
	{
		return;
	}
	const bool bDoor = bIndoor && !bThemeInterior;
	ThemeZone = Z;
	bThemeInterior = bIndoor;
	HolypawAudio::PlayTheme(this, this, ThemeComp, Z, bIndoor);
	if (bDoor)
	{
		HolypawAudio::PlayCue(this, TEXT("Door"));
	}
}
