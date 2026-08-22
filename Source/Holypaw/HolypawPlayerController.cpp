#include "HolypawPlayerController.h"
#include "Character/HolypawCharacter.h"
#include "GameFramework/InputSettings.h"

AHolypawPlayerController::AHolypawPlayerController()
{
	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AHolypawPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
}

void AHolypawPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AHolypawPlayerController::OnEscape()
{
	// Jump is already on the pawn. Escape closes UI if we bind it separately later.
	if (AHolypawCharacter* HolypawPawn = Cast<AHolypawCharacter>(GetPawn()))
	{
		if (HolypawPawn->IsSkillsOpen())
		{
			HolypawPawn->ToggleSkills();
		}
		else if (HolypawPawn->IsMapOpen())
		{
			HolypawPawn->ToggleMap();
		}
		else if (HolypawPawn->IsJournalOpen())
		{
			HolypawPawn->ToggleJournal();
		}
	}
}
