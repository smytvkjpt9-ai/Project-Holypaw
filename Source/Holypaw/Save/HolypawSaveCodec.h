#pragma once

#include "CoreMinimal.h"

class AHolypawCharacter;
class UHolypawSaveGame;

namespace HolypawSaveCodec
{
	void Capture(AHolypawCharacter* Pawn, UHolypawSaveGame& Out);
	void Apply(AHolypawCharacter* Pawn, const UHolypawSaveGame& In);
	void ResetWorldFaith(AHolypawCharacter* Pawn);
}
