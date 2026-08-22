#include "HolypawGameMode.h"
#include "HolypawWorldBuilder.h"
#include "HolypawPlayerController.h"
#include "Character/HolypawCharacter.h"
#include "UI/HolypawHUD.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"

AHolypawGameMode::AHolypawGameMode()
{
	DefaultPawnClass = AHolypawCharacter::StaticClass();
	PlayerControllerClass = AHolypawPlayerController::StaticClass();
	HUDClass = AHolypawHUD::StaticClass();
}

void AHolypawGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AHolypawGameMode::StartPlay()
{
	EnsureWorldBuilder();
	for (TActorIterator<AHolypawWorldBuilder> It(GetWorld()); It; ++It)
	{
		It->GenerateWorld();
		break;
	}
	Super::StartPlay();
}

void AHolypawGameMode::EnsureWorldBuilder()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	for (TActorIterator<AHolypawWorldBuilder> It(World); It; ++It)
	{
		return;
	}
	FActorSpawnParameters Sp;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	World->SpawnActor<AHolypawWorldBuilder>(FVector::ZeroVector, FRotator::ZeroRotator, Sp);
}

AActor* AHolypawGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		for (TActorIterator<APlayerStart> It(World); It; ++It)
		{
			if (It->PlayerStartTag == FName(TEXT("Cottage")))
			{
				return *It;
			}
		}
		for (TActorIterator<APlayerStart> It(World); It; ++It)
		{
			return *It;
		}
	}
	return Super::FindPlayerStart_Implementation(Player, IncomingName);
}
