#include "HolypawWorldBuilder.h"
#include "Actors/HolypawMillBanner.h"
#include "Faith/HolypawFaithSim.h"
#include "HolypawGameInstance.h"
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
	const bool bCity = HolypawCatalog::IsCityZone(Zone);
	const bool bWantHymn = GI->IsDusk() && bCity && HolypawFaith::DuskHymnUnlocked(Hearts);
	if (GI->Settings && GI->Settings->bMuted)
	{
		HolypawAudio::StopTheme(HymnComp.Get());
		HolypawAudio::StopTheme(ThemeComp.Get());
		return;
	}

	if (bWantHymn)
	{
		if (!bHymnPlaying)
		{
			HolypawAudio::PlayHymn(this, this, HymnComp);
			bHymnPlaying = true;
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
