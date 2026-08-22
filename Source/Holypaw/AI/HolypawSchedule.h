#pragma once

#include "CoreMinimal.h"

class AHugHuman;

/** Day / dusk / night beats for city humans. Kneeling campaign-end humans stay put.
 *  Believers clap-walk plaza loops once Hearts open the city. Dusk hymn pulls them to chapel. */
namespace HolypawSchedule
{
	void EnsureAnchors(AHugHuman& Human);
	void TickHuman(AHugHuman& Human, float DeltaSeconds, float Hour);
	FVector ParadeGoal(const AHugHuman& Human, float Hour, int32 Hearts);
}
