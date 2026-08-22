#include "Actors/HolypawPickup.h"
#include "Character/HolypawCharacter.h"
#include "Look/HolypawLook.h"

AHolypawPickup::AHolypawPickup()
{
	PrimaryActorTick.bCanEverTick = true;
	Label = NSLOCTEXT("Holypaw", "Pickup", "Keepsake");
}

void AHolypawPickup::BeginPlay()
{
	Super::BeginPlay();
	if (SphereMesh)
	{
		Mesh->SetStaticMesh(SphereMesh);
	}
	SetActorScale3D(FVector(0.35f, 0.35f, 0.35f));
	Home = GetActorLocation();
	if (ItemId == TEXT("hymnSheet"))
	{
		SetSolidColor(HolypawLook::GoldWarm);
	}
	else if (ItemId == TEXT("wheatEar"))
	{
		SetSolidColor(FLinearColor(0.88f, 0.74f, 0.32f));
	}
	else if (ItemId == TEXT("peatBead"))
	{
		SetSolidColor(HolypawLook::RoseDeep);
	}
	else if (ItemId == TEXT("auroraThread"))
	{
		SetSolidColor(HolypawLook::Powder);
	}
	else
	{
		SetSolidColor(HolypawLook::Rose);
	}
}

void AHolypawPickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const float T = GetWorld()->GetTimeSeconds();
	AddActorWorldRotation(FRotator(0.f, 70.f * DeltaSeconds, 0.f));
	SetActorLocation(Home + FVector(0.f, 0.f, FMath::Sin(T * 3.2f) * 10.f));
}

FText AHolypawPickup::GetPrompt() const
{
	return FText::Format(NSLOCTEXT("Holypaw", "PickPrompt", "E  Pocket {0}"), Label);
}

bool AHolypawPickup::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn || ItemId.IsNone())
	{
		return false;
	}
	if (InstigatorPawn->GetItemCount(ItemId) > 0)
	{
		InstigatorPawn->Toast(TEXT("Pockets already hold that. Leave it for the next you."));
		return true;
	}
	InstigatorPawn->AddItem(ItemId, 1);
	InstigatorPawn->Toast(FString::Printf(TEXT("Pocketed %s."), *Label.ToString()));
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	return true;
}
