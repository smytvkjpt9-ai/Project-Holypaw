#include "Actors/WildFluffy.h"
#include "Character/HolypawCharacter.h"

AWildFluffy::AWildFluffy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWildFluffy::BeginPlay()
{
	Super::BeginPlay();
	Home = GetActorLocation();
	Mesh->SetWorldScale3D(FVector(0.55f, 0.4f, 0.4f));
	SetSolidColor(Type.Color);
	WanderT = FMath::FRandRange(0.5f, 2.f);
}

FText AWildFluffy::GetPrompt() const
{
	if (bRecruited)
	{
		return FText::GetEmpty();
	}
	return FText::Format(NSLOCTEXT("Holypaw", "Recruit", "E  Recruit {0}"), Type.DisplayName);
}

bool AWildFluffy::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn || bRecruited)
	{
		return false;
	}
	return InstigatorPawn->RecruitFluffy(this);
}

void AWildFluffy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bRecruited)
	{
		return;
	}
	WanderT -= DeltaSeconds;
	if (WanderT <= 0.f)
	{
		WanderT = FMath::FRandRange(1.5f, 3.5f);
		Vel = FVector(FMath::FRandRange(-80.f, 80.f), FMath::FRandRange(-80.f, 80.f), 0.f);
	}
	FVector Next = GetActorLocation() + Vel * DeltaSeconds;
	if (FVector::Dist2D(Next, Home) > 350.f)
	{
		Vel *= -1.f;
	}
	else
	{
		SetActorLocation(Next);
	}
}
