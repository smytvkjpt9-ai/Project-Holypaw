#pragma once

#include "CoreMinimal.h"

class FSlateWindowElementList;
struct FGeometry;

enum class EHolypawUiIcon : uint8
{
	Heart,
	Paw,
	Halo,
	Coin,
	Jar,
	Health,
	Lantern,
	Book,
	Speech,
	Stall,
	Slap,
	Beam,
	Party,
	Flee,
	Guard,
	Hymn,
	Stitch,
	Mill,
	Pause,
	Teddy,
	Ribbon,
	Save,
	Mute,
	Check,
	Arrow,
	Bun,
	Cocoa,
	MapPin,
	Key,
	None
};

namespace HolypawUi
{
	void DrawIcon(FSlateWindowElementList& Out, int32 Layer, const FGeometry& Geo,
		FVector2D Pos, float Size, EHolypawUiIcon Id, FLinearColor Color);

	EHolypawUiIcon IconForAbility(FName Id);
	EHolypawUiIcon IconForItem(FName Id);
	FLinearColor IconTint(EHolypawUiIcon Id);
}
