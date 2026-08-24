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
	if (!InputComponent)
	{
		return;
	}
	InputComponent->BindAxis(TEXT("CameraZoom"), this, &AHolypawPlayerController::CameraZoomAxis);
	InputComponent->BindAction(TEXT("ZoomIn"), IE_Pressed, this, &AHolypawPlayerController::ZoomIn);
	InputComponent->BindAction(TEXT("ZoomOut"), IE_Pressed, this, &AHolypawPlayerController::ZoomOut);
}

void AHolypawPlayerController::CameraZoomAxis(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		ApplyCameraZoom(-Value * 60.f);
	}
}

void AHolypawPlayerController::ZoomIn()
{
	ApplyCameraZoom(-90.f);
}

void AHolypawPlayerController::ZoomOut()
{
	ApplyCameraZoom(90.f);
}

void AHolypawPlayerController::ApplyCameraZoom(float DeltaArm)
{
	if (AHolypawCharacter* HolypawPawn = Cast<AHolypawCharacter>(GetPawn()))
	{
		HolypawPawn->AdjustCameraZoom(DeltaArm);
	}
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
