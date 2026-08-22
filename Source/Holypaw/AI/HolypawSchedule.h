#pragma once

#include "CoreMinimal.h"

class AHugHuman;

/** Day / dusk / night beats for city humans. Kneeling campaign-end humans stay put. */
namespace HolypawSchedule
{
	void EnsureAnchors(AHugHuman& Human);
	void TickHuman(AHugHuman& Human, float DeltaSeconds, float Hour);
}
