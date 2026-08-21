#include "HolypawGameInstance.h"
#include "Save/HolypawSaveGame.h"
#include "Save/HolypawSaveCodec.h"
#include "Character/HolypawCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UHolypawGameInstance::Init()
{
	Super::Init();
	LoadOrCreateSettings();
	RefreshSlotCache();
}

UHolypawGameInstance* UHolypawGameInstance::Get(const UObject* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}
	const UWorld* World = WorldContext->GetWorld();
	return World ? Cast<UHolypawGameInstance>(World->GetGameInstance()) : nullptr;
}

FString UHolypawGameInstance::SlotName(int32 Index)
{
	return FString::Printf(TEXT("HolypawSlot%d"), FMath::Clamp(Index, 0, SlotCount - 1));
}

void UHolypawGameInstance::LoadOrCreateSettings()
{
	if (USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(TEXT("HolypawSettings"), 0))
	{
		Settings = Cast<UHolypawSettingsSave>(Loaded);
	}
	if (!Settings)
	{
		Settings = Cast<UHolypawSettingsSave>(UGameplayStatics::CreateSaveGameObject(UHolypawSettingsSave::StaticClass()));
	}
}

void UHolypawGameInstance::SaveSettings()
{
	if (!Settings)
	{
		LoadOrCreateSettings();
	}
	if (Settings)
	{
		UGameplayStatics::SaveGameToSlot(Settings, TEXT("HolypawSettings"), 0);
	}
}

void UHolypawGameInstance::CycleMute()
{
	if (!Settings)
	{
		LoadOrCreateSettings();
	}
	if (Settings)
	{
		Settings->bMuted = !Settings->bMuted;
		SaveSettings();
	}
}

void UHolypawGameInstance::InvalidateSlotCache()
{
	bSlotCacheReady = false;
}

UHolypawSaveGame* UHolypawGameInstance::PeekSlot(int32 Index) const
{
	return Cast<UHolypawSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName(Index), 0));
}

void UHolypawGameInstance::RefreshSlotCache() const
{
	SlotCache.SetNum(SlotCount);
	for (int32 I = 0; I < SlotCount; ++I)
	{
		if (const UHolypawSaveGame* Save = PeekSlot(I))
		{
			SlotCache[I] = Save->MakeSummaryLine();
		}
		else
		{
			SlotCache[I] = TEXT("empty porch — start a coup");
		}
	}
	bSlotCacheReady = true;
}

bool UHolypawGameInstance::SlotOccupied(int32 Index) const
{
	const UHolypawSaveGame* Save = PeekSlot(Index);
	return Save && Save->bOccupied;
}

FString UHolypawGameInstance::SlotSummary(int32 Index) const
{
	if (!bSlotCacheReady)
	{
		RefreshSlotCache();
	}
	Index = FMath::Clamp(Index, 0, SlotCount - 1);
	return SlotCache.IsValidIndex(Index) ? SlotCache[Index] : TEXT("empty porch — start a coup");
}

bool UHolypawGameInstance::SavePawnToSlot(AHolypawCharacter* Pawn, int32 Index)
{
	if (!Pawn)
	{
		return false;
	}
	UHolypawSaveGame* Save = Cast<UHolypawSaveGame>(UGameplayStatics::CreateSaveGameObject(UHolypawSaveGame::StaticClass()));
	if (!Save)
	{
		return false;
	}
	HolypawSaveCodec::Capture(Pawn, *Save);
	Save->PlaySeconds = PlaySeconds;
	ActiveSlot = FMath::Clamp(Index, 0, SlotCount - 1);
	const bool bOk = UGameplayStatics::SaveGameToSlot(Save, SlotName(ActiveSlot), 0);
	if (bOk)
	{
		InvalidateSlotCache();
	}
	return bOk;
}

bool UHolypawGameInstance::LoadSlotOntoPawn(AHolypawCharacter* Pawn, int32 Index)
{
	UHolypawSaveGame* Save = PeekSlot(Index);
	if (!Save || !Save->bOccupied || !Pawn)
	{
		return false;
	}
	ActiveSlot = FMath::Clamp(Index, 0, SlotCount - 1);
	PlaySeconds = Save->PlaySeconds;
	HolypawSaveCodec::Apply(Pawn, *Save);
	bSessionStarted = true;
	return true;
}

void UHolypawGameInstance::StartNewGame(AHolypawCharacter* Pawn, int32 Index)
{
	ActiveSlot = FMath::Clamp(Index, 0, SlotCount - 1);
	PlaySeconds = 0.f;
	bSessionStarted = true;
	if (Pawn)
	{
		HolypawSaveCodec::ResetWorldFaith(Pawn);
		Pawn->ResetForNewGame();
		SavePawnToSlot(Pawn, ActiveSlot);
	}
}
