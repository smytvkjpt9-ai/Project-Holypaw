#include "Actors/HostilePet.h"
#include "Character/HolypawCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AHostilePet::AHostilePet()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHostilePet::BeginPlay()
{
	Super::BeginPlay();
	Home = GetActorLocation();
	HP = HPMax;
	Mesh->SetWorldScale3D(FVector(0.7f, 0.5f, 0.55f));
	SetSolidColor(FLinearColor(0.45f, 0.4f, 0.42f));
	WanderVel = FVector(FMath::FRandRange(-60.f, 60.f), FMath::FRandRange(-60.f, 60.f), 0.f);
}

void AHostilePet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bDefeated)
	{
		return;
	}

	AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!Pawn || Pawn->IsBusy())
	{
		return;
	}

	const float Dist = FVector::Dist(GetActorLocation(), Pawn->GetActorLocation());
	if (Dist < 140.f && Pawn->GetInvuln() <= 0.f)
	{
		Pawn->StartBattle(this);
		return;
	}

	if (Dist < AggroRange)
	{
		const FVector Dir = (Pawn->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		AddActorWorldOffset(Dir * 280.f * DeltaSeconds);
	}
	else
	{
		FVector Next = GetActorLocation() + WanderVel * DeltaSeconds;
		if (FVector::Dist2D(Next, Home) > 500.f)
		{
			WanderVel *= -1.f;
		}
		else
		{
			SetActorLocation(Next);
		}
	}
}

void AHostilePet::Defeat(bool bGrantRewards)
{
	bDefeated = true;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	if (bGrantRewards)
	{
		if (AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
		{
			Pawn->GrantKillRewards();
		}
		GetWorldTimerManager().SetTimer(RespawnTimer, this, &AHostilePet::RespawnLater, 25.f, false);
	}
}

void AHostilePet::RespawnLater()
{
	bDefeated = false;
	HP = HPMax;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	const FVector Jitter(FMath::FRandRange(-200.f, 200.f), FMath::FRandRange(-200.f, 200.f), 0.f);
	SetActorLocation(Home + Jitter);
}
