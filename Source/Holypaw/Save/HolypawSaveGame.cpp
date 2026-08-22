#include "Save/HolypawSaveGame.h"

FString UHolypawSaveGame::MakeSummaryLine() const
{
	if (!bOccupied)
	{
		return TEXT("empty porch — start a coup");
	}
	const FString Mission = MissionTitle.IsEmpty() ? TEXT("Wake") : MissionTitle;
	const FString Zone = ZoneName.IsEmpty() ? TEXT("Stuffed Park") : ZoneName;
	return FString::Printf(TEXT("%s  ·  %s  ·  Hearts %d  ·  %dm"),
		*Zone, *Mission, TotalHearts, FMath::FloorToInt(PlaySeconds / 60.f));
}
