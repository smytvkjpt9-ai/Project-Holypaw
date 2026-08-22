#include "HolypawWorldBuilder.h"
#include "HolypawCatalog.h"
#include "Actors/HolypawMillBanner.h"
#include "Actors/FaithStall.h"
#include "Actors/HugHuman.h"
#include "Actors/HolypawShrine.h"
#include "Actors/Signpost.h"
#include "Faith/HolypawFaithSim.h"
#include "HolypawGameInstance.h"
#include "Save/HolypawSaveGame.h"
#include "Character/HolypawCharacter.h"
#include "Audio/HolypawAudio.h"
#include "Components/AudioComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void AHolypawWorldBuilder::PlaceMillBanner(const FVector2D& XY, const float Yaw)
{
	if (!GetWorld())
	{
		return;
	}
	const float Z = SampleHeight(XY.X, XY.Y);
	FActorSpawnParameters Sp;
	Sp.Owner = this;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (AHolypawMillBanner* Banner = GetWorld()->SpawnActor<AHolypawMillBanner>(
		FVector(XY.X, XY.Y, Z), FRotator(0.f, Yaw, 0.f), Sp))
	{
		Banner->HomeZone = EHolypawZone::RibbonCity;
		MillBanners.Add(Banner);
	}
}

void AHolypawWorldBuilder::PlaceRibbonMillBanners()
{
	if (bRibbonBannersPlaced)
	{
		return;
	}
	bRibbonBannersPlaced = true;

	const FVector2D Plaza = RibbonCity;
	const FVector2D Market = RibbonCity + FVector2D(1800.f, -500.f);
	const FVector2D Mill = RibbonCity + FVector2D(5200.f, -800.f);

	PlaceMillBanner(Plaza + FVector2D(160.f, -140.f), 15.f);
	PlaceMillBanner(Plaza + FVector2D(-90.f, 70.f), -22.f);
	PlaceMillBanner(Plaza + FVector2D(420.f, 80.f), 12.f);
	PlaceMillBanner(Plaza + FVector2D(780.f, -40.f), -8.f);
	PlaceMillBanner(Market + FVector2D(-80.f, 160.f), 90.f);
	PlaceMillBanner(Market + FVector2D(220.f, -40.f), 4.f);
	PlaceMillBanner(Market + FVector2D(480.f, 90.f), -20.f);

	for (int32 I = 0; I < 4; ++I)
	{
		const float T = (I + 1) / 5.f;
		const FVector2D Road = FMath::Lerp(Plaza, Mill, T);
		PlaceMillBanner(Road + FVector2D(40.f, (I % 2 == 0) ? 120.f : -140.f), I * 18.f);
	}

	PlaceMillBanner(Mill + FVector2D(-360.f, -80.f), 90.f);
	PlaceMillBanner(Mill + FVector2D(-280.f, 160.f), 70.f);
	PlaceMillBanner(Mill + FVector2D(80.f, 240.f), 0.f);
	PlaceMillBanner(Mill + FVector2D(200.f, -220.f), 180.f);
}

void AHolypawWorldBuilder::ApplyRibbonLook(const int32 Hearts)
{
	LastRibbonLook = Hearts;
	if (FountainPool)
	{
		FLinearColor Pool(0.55f, 0.75f, 0.92f);
		if (HolypawFaith::MillBannersDown(Hearts))
		{
			Pool = FLinearColor(0.95f, 0.52f, 0.68f);
		}
		else if (HolypawFaith::ShopsOpen(Hearts))
		{
			Pool = FLinearColor(0.78f, 0.62f, 0.88f);
		}
		ColorMesh(FountainPool, Pool);
	}
	if (FountainJet)
	{
		ColorMesh(FountainJet, HolypawFaith::MillBannersDown(Hearts)
			? FLinearColor(1.f, 0.82f, 0.45f)
			: FLinearColor(0.85f, 0.88f, 0.95f));
	}
	const FLinearColor Cloth = HolypawFaith::MillBannersDown(Hearts)
		? FLinearColor(0.98f, 0.58f, 0.72f)
		: FLinearColor(0.85f, 0.42f, 0.58f);
	for (UStaticMeshComponent* Banner : HandmadeBanners)
	{
		if (Banner)
		{
			ColorMesh(Banner, Cloth);
		}
	}
	for (ASignpost* Sign : LivingSigns)
	{
		if (Sign)
		{
			Sign->RefreshFromHearts(Hearts);
		}
	}
}

void AHolypawWorldBuilder::SnapConversionLook()
{
	const int32 RibbonHearts = HolypawFaith::HeartsAt(this, EHolypawZone::RibbonCity);
	for (int32 I = MillBanners.Num() - 1; I >= 0; --I)
	{
		AHolypawMillBanner* Banner = MillBanners[I].Get();
		if (!Banner)
		{
			MillBanners.RemoveAt(I);
			continue;
		}
		Banner->SnapToHearts(RibbonHearts);
	}
	ApplyRibbonLook(RibbonHearts);
	for (TActorIterator<AFaithStall> It(GetWorld()); It; ++It)
	{
		if (AFaithStall* Stall = *It)
		{
			Stall->SnapShutter();
		}
	}
}

void AHolypawWorldBuilder::NotifyConvertPulse(const FVector& At)
{
	const int32 RibbonHearts = HolypawFaith::HeartsAt(this, EHolypawZone::RibbonCity);
	ApplyRibbonLook(RibbonHearts);
	if (!GetWorld())
	{
		return;
	}
	for (TActorIterator<AHugHuman> It(GetWorld()); It; ++It)
	{
		AHugHuman* H = *It;
		if (!H || H->IsKnelt())
		{
			continue;
		}
		const float Dist = FVector::Dist(At, H->GetActorLocation());
		if (H->bBeliever)
		{
			if (Dist < 1800.f)
			{
				H->ClapBurst = FMath::Max(H->ClapBurst, 1.8f);
			}
		}
		else if (Dist < 1100.f)
		{
			H->NoticeConvert(At);
		}
	}
	for (AHolypawMillBanner* Banner : MillBanners)
	{
		if (Banner && FVector::Dist(At, Banner->GetActorLocation()) < 3200.f)
		{
			Banner->BoostDrop(3.2f);
		}
	}
	for (TActorIterator<AFaithStall> It(GetWorld()); It; ++It)
	{
		AFaithStall* Stall = *It;
		if (Stall && FVector::Dist(At, Stall->GetActorLocation()) < 2400.f)
		{
			Stall->SnapShutter();
		}
	}
}

void AHolypawWorldBuilder::TickConversionPulse(const float DeltaSeconds)
{
	UHolypawGameInstance* GI = UHolypawGameInstance::Get(this);
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!GI || !Pawn)
	{
		HolypawAudio::StopTheme(HymnComp.Get());
		return;
	}

	const EHolypawZone Zone = ResolveZone(Pawn->GetActorLocation());
	const int32 Hearts = HolypawFaith::HeartsAt(this, Zone);
	const int32 RibbonHearts = HolypawFaith::HeartsAt(this, EHolypawZone::RibbonCity);
	if (RibbonHearts != LastRibbonLook)
	{
		ApplyRibbonLook(RibbonHearts);
	}
	const bool bCity = HolypawCatalog::IsCityZone(Zone);
	const bool bWantHymn = GI->IsDusk() && bCity && HolypawFaith::DuskHymnUnlocked(Hearts);
	if (FountainJet)
	{
		const float T = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
		const float Rate = (GI->IsDusk() && HolypawFaith::ChoirOwnsDusk(RibbonHearts)) ? 3.2f : 1.7f;
		const float Tall = HolypawFaith::MillBannersDown(RibbonHearts) ? 1.38f
			: (HolypawFaith::ShopsOpen(RibbonHearts) ? 1.16f : 1.f);
		const float Bob = 1.f + 0.1f * FMath::Sin(T * Rate);
		FountainJet->SetWorldScale3D(FVector(0.35f, 0.35f, 1.1f * Tall * Bob));
	}
	if (GI->Settings && GI->Settings->bMuted)
	{
		HolypawAudio::StopTheme(HymnComp.Get());
		HolypawAudio::StopTheme(ThemeComp.Get());
		if (const AHolypawCharacter* Teddy = Cast<AHolypawCharacter>(Pawn))
		{
			ConversionLine = HolypawFaith::HudLine(Teddy->CurrentZone, Teddy->GetCityHearts(Teddy->CurrentZone));
		}
		return;
	}

	if (bWantHymn)
	{
		if (!bHymnPlaying)
		{
			HolypawAudio::PlayHymn(this, this, HymnComp);
			bHymnPlaying = true;
		}
		if (HymnComp)
		{
			float HymnDist = 2800.f;
			for (TActorIterator<AHolypawShrine> It(GetWorld()); It; ++It)
			{
				const AHolypawShrine* S = *It;
				if (!S || S->Kind != EHolypawShrineKind::Chapel)
				{
					continue;
				}
				HymnDist = FMath::Min(HymnDist, FVector::Dist2D(Pawn->GetActorLocation(), S->GetActorLocation()));
			}
			const float Duck = FMath::GetMappedRangeValueClamped(FVector2D(600.f, 8200.f), FVector2D(1.f, 0.16f), HymnDist);
			const float Mix = (GI->Settings) ? GI->Settings->MasterVolume : 1.f;
			HymnComp->SetVolumeMultiplier(Mix * 0.62f * Duck);
		}
		if (ThemeComp)
		{
			ThemeComp->SetVolumeMultiplier(0.22f);
		}
	}
	else if (bHymnPlaying)
	{
		HolypawAudio::StopTheme(HymnComp.Get());
		bHymnPlaying = false;
		if (ThemeComp)
		{
			ThemeComp->SetVolumeMultiplier(IsPlayerIndoors(Pawn->GetActorLocation()) ? 0.7f : 0.55f);
		}
	}

	if (const AHolypawCharacter* Teddy = Cast<AHolypawCharacter>(Pawn))
	{
		ConversionLine = HolypawFaith::HudLine(Teddy->CurrentZone, Teddy->GetCityHearts(Teddy->CurrentZone));
	}

	(void)DeltaSeconds;
}

FString AHolypawWorldBuilder::GetConversionLine() const
{
	return ConversionLine;
}
