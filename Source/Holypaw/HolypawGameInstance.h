#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HolypawTypes.h"
#include "HolypawGameInstance.generated.h"

class UHolypawSaveGame;
class UHolypawSettingsSave;
class AHolypawCharacter;
class USoundWaveProcedural;

UCLASS()
class HOLYPAW_API UHolypawGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	static UHolypawGameInstance* Get(const UObject* WorldContext);

	static constexpr int32 SlotCount = 3;
	static FString SlotName(int32 Index);

	UPROPERTY()
	int32 ActiveSlot = 0;

	UPROPERTY()
	int32 TitleCursor = 0;

	UPROPERTY()
	bool bSessionStarted = false;

	UPROPERTY()
	float PlaySeconds = 0.f;

	UPROPERTY()
	TObjectPtr<UHolypawSettingsSave> Settings;

	UHolypawSaveGame* PeekSlot(int32 Index) const;
	bool SlotOccupied(int32 Index) const;
	FString SlotSummary(int32 Index) const;
	void InvalidateSlotCache();

	bool SavePawnToSlot(AHolypawCharacter* Pawn, int32 Index);
	bool LoadSlotOntoPawn(AHolypawCharacter* Pawn, int32 Index);
	void StartNewGame(AHolypawCharacter* Pawn, int32 Index);
	void SaveSettings();
	void CycleMute();
	void KeepWave(class USoundWaveProcedural* Wave);
	float GetWorldHour() const;
	FString GetClockLabel() const;
	bool IsDusk() const;
	bool IsNight() const;

protected:
	void LoadOrCreateSettings();
	void RefreshSlotCache() const;

	mutable bool bSlotCacheReady = false;
	mutable TArray<FString> SlotCache;

	UPROPERTY()
	TArray<TObjectPtr<USoundWaveProcedural>> LiveWaves;
};
