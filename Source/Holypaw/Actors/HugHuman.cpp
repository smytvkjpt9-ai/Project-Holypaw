#include "Actors/HugHuman.h"
#include "Character/HolypawCharacter.h"

AHugHuman::AHugHuman()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHugHuman::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetWorldScale3D(FVector(0.45f, 0.35f, 1.15f));
	SetSolidColor(FLinearColor(0.95f, 0.78f, 0.7f));
}

FText AHugHuman::GetPrompt() const
{
	return FText::Format(NSLOCTEXT("Holypaw", "Hug", "E  Hug {0}  ({1}%)"),
		PersonName,
		FText::AsNumber(FMath::FloorToInt(ConvertProgress)));
}

bool AHugHuman::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn)
	{
		return false;
	}
	return InstigatorPawn->HugPerson(this);
}
