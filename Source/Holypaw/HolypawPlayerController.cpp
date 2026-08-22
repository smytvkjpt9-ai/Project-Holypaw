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
	if (AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(GetPawn()))
	{
		if (Pawn->IsSkillsOpen())
		{
			Pawn->ToggleSkills();
		}
		else if (Pawn->IsMapOpen())
		{
			Pawn->ToggleMap();
		}
		else if (Pawn->IsJournalOpen())
		{
			Pawn->ToggleJournal();
		}
	}
}
