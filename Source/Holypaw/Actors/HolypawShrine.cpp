#include "Actors/HolypawShrine.h"
#include "Character/HolypawCharacter.h"

AHolypawShrine::AHolypawShrine()
{
	PrimaryActorTick.bCanEverTick = false;
	PlaceName = NSLOCTEXT("Holypaw", "Shrine", "Shrine");
}

void AHolypawShrine::BeginPlay()
{
	Super::BeginPlay();
	switch (Kind)
	{
	case EHolypawShrineKind::Inn:
		SetSolidColor(FLinearColor(0.78f, 0.52f, 0.42f));
		break;
	case EHolypawShrineKind::Chapel:
		SetSolidColor(FLinearColor(0.92f, 0.82f, 0.55f));
		break;
	case EHolypawShrineKind::Workshop:
		SetSolidColor(FLinearColor(0.62f, 0.48f, 0.72f));
		break;
	case EHolypawShrineKind::Wish:
		SetSolidColor(FLinearColor(0.45f, 0.72f, 0.88f));
		break;
	default:
		SetSolidColor(FLinearColor(0.55f, 0.52f, 0.48f));
		break;
	}
}

FText AHolypawShrine::GetPrompt() const
{
	switch (Kind)
	{
	case EHolypawShrineKind::Inn:
		return FText::Format(NSLOCTEXT("Holypaw", "Inn", "E  Walk in · rest at {0} (heal + save)"), PlaceName);
	case EHolypawShrineKind::Chapel:
		return FText::Format(NSLOCTEXT("Holypaw", "Chapel", "E  Walk in · hymn at {0} (Miracle Charge)"), PlaceName);
	case EHolypawShrineKind::Workshop:
		return NSLOCTEXT("Holypaw", "Workshop", "E  Walk in · cloth loft (hymn ribbon, 8 AP)");
	case EHolypawShrineKind::Wish:
		return NSLOCTEXT("Holypaw", "Wish", "E  Plaza fountain — toss a thought (+FP)");
	default:
		return NSLOCTEXT("Holypaw", "Crate", "E  Harbor crate (maybe scrap)");
	}
}

bool AHolypawShrine::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn)
	{
		return false;
	}
	return InstigatorPawn->UseShrine(Kind);
}
