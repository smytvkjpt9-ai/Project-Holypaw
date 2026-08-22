#include "HolypawWorldBuilder.h"
#include "Actors/HolypawShrine.h"
#include "Look/HolypawLook.h"
#include "Engine/PointLight.h"
#include "Components/PointLightComponent.h"
#include "EngineUtils.h"

namespace
{
	void SpawnLamp(AHolypawWorldBuilder* Builder, const FVector& Loc, const FLinearColor& Color, const float Intensity)
	{
		HolypawLook::SpawnGlow(Builder ? Builder->GetWorld() : nullptr, Builder, Loc, Color, Intensity, 640.f);
	}
}

void AHolypawWorldBuilder::DressRoomShell(const FVector& Origin, const FLinearColor& Wall, const FLinearColor& Floor, const FLinearColor& Trim)
{
	// Walkable 420x420 room. Door gap on -Y. Engine cube is 100uu.
	PlaceCube(Origin + FVector(0.f, 0.f, 8.f), FVector(4.2f, 4.2f, 0.14f), Floor, MakeName(TEXT("RoomFloor")));
	PlaceCube(Origin + FVector(0.f, 210.f, 128.f), FVector(4.2f, 0.18f, 2.48f), Wall, MakeName(TEXT("RoomBack")));
	PlaceCube(Origin + FVector(-210.f, 0.f, 128.f), FVector(0.18f, 4.2f, 2.48f), Wall, MakeName(TEXT("RoomLeft")));
	PlaceCube(Origin + FVector(210.f, 0.f, 128.f), FVector(0.18f, 4.2f, 2.48f), Wall, MakeName(TEXT("RoomRight")));
	PlaceCube(Origin + FVector(-145.f, -210.f, 128.f), FVector(1.35f, 0.18f, 2.48f), Wall, MakeName(TEXT("RoomFrontL")));
	PlaceCube(Origin + FVector(145.f, -210.f, 128.f), FVector(1.35f, 0.18f, 2.48f), Wall, MakeName(TEXT("RoomFrontR")));
	PlaceCube(Origin + FVector(0.f, -210.f, 232.f), FVector(1.55f, 0.18f, 0.42f), Trim, MakeName(TEXT("RoomLintel")));
	PlaceCube(Origin + FVector(0.f, 0.f, 256.f), FVector(4.3f, 4.3f, 0.1f), Wall * 0.85f, MakeName(TEXT("RoomCeiling")));
	PlaceCube(Origin + FVector(-70.f, -214.f, 118.f), FVector(0.18f, 0.16f, 2.2f), Trim, MakeName(TEXT("DoorJambL")));
	PlaceCube(Origin + FVector(70.f, -214.f, 118.f), FVector(0.18f, 0.16f, 2.2f), Trim, MakeName(TEXT("DoorJambR")));
	PlaceCube(Origin + FVector(-92.f, -228.f, 90.f), FVector(0.12f, 0.7f, 1.7f), Trim * 0.7f, MakeName(TEXT("DoorLeaf")));
	PlaceCube(Origin + FVector(-120.f, 212.f, 150.f), FVector(0.7f, 0.08f, 0.7f), HolypawLook::Glass, MakeName(TEXT("RoomPane")));
	PlaceCube(Origin + FVector(120.f, 212.f, 150.f), FVector(0.7f, 0.08f, 0.7f), HolypawLook::Glass, MakeName(TEXT("RoomPaneB")));
	PlaceCube(Origin + FVector(0.f, 0.f, 248.f), FVector(0.35f, 0.35f, 0.18f), HolypawLook::GoldWarm, MakeName(TEXT("RoomFixture")));
}

void AHolypawWorldBuilder::DressInnRoom(const FVector& Origin)
{
	const FLinearColor Wall(0.78f, 0.52f, 0.42f);
	const FLinearColor Floor(0.62f, 0.44f, 0.32f);
	const FLinearColor Trim(0.42f, 0.28f, 0.2f);
	DressRoomShell(Origin, Wall, Floor, Trim);
	PlaceCube(Origin + FVector(-110.f, 90.f, 42.f), FVector(1.8f, 1.05f, 0.42f), FLinearColor(0.82f, 0.58f, 0.66f), MakeName(TEXT("InnBed")));
	PlaceCube(Origin + FVector(-160.f, 90.f, 68.f), FVector(0.55f, 0.9f, 0.22f), FLinearColor(0.95f, 0.88f, 0.92f), MakeName(TEXT("InnPillow")));
	PlaceCube(Origin + FVector(-110.f, 90.f, 58.f), FVector(1.5f, 0.85f, 0.08f), FLinearColor(0.92f, 0.72f, 0.55f), MakeName(TEXT("InnQuilt")));
	PlaceCube(Origin + FVector(120.f, 70.f, 55.f), FVector(0.85f, 0.55f, 0.7f), FLinearColor(0.48f, 0.32f, 0.24f), MakeName(TEXT("InnDesk")));
	PlaceCube(Origin + FVector(120.f, 70.f, 98.f), FVector(0.18f, 0.18f, 0.45f), FLinearColor(1.f, 0.86f, 0.45f), MakeName(TEXT("InnLamp")));
	PlaceCube(Origin + FVector(40.f, -40.f, 28.f), FVector(1.1f, 0.7f, 0.08f), FLinearColor(0.72f, 0.38f, 0.42f), MakeName(TEXT("InnRug")));
	PlaceCube(Origin + FVector(130.f, -80.f, 48.f), FVector(0.45f, 0.45f, 0.55f), FLinearColor(0.55f, 0.38f, 0.28f), MakeName(TEXT("InnChair")));
	PlaceCube(Origin + FVector(-40.f, -80.f, 70.f), FVector(0.9f, 0.35f, 1.1f), FLinearColor(0.58f, 0.4f, 0.32f), MakeName(TEXT("InnDresser")));
	PlaceCube(Origin + FVector(0.f, 200.f, 150.f), FVector(0.7f, 0.08f, 0.7f), FLinearColor(0.55f, 0.82f, 0.95f), MakeName(TEXT("InnWindow")));
	SpawnLamp(this, Origin + FVector(0.f, 20.f, 190.f), FLinearColor(1.f, 0.82f, 0.58f), 2800.f);
}

void AHolypawWorldBuilder::DressChapelRoom(const FVector& Origin)
{
	const FLinearColor Wall(0.92f, 0.82f, 0.55f);
	const FLinearColor Floor(0.78f, 0.7f, 0.52f);
	const FLinearColor Trim(0.95f, 0.75f, 0.35f);
	DressRoomShell(Origin, Wall, Floor, Trim);
	PlaceCube(Origin + FVector(0.f, 130.f, 70.f), FVector(1.4f, 0.7f, 0.95f), FLinearColor(0.95f, 0.82f, 0.45f), MakeName(TEXT("ChapelAltar")));
	PlaceCube(Origin + FVector(0.f, 130.f, 125.f), FVector(0.35f, 0.18f, 0.7f), FLinearColor(0.95f, 0.55f, 0.72f), MakeName(TEXT("ChapelCloth")));
	PlaceCube(Origin + FVector(-90.f, -20.f, 42.f), FVector(1.5f, 0.45f, 0.42f), FLinearColor(0.62f, 0.42f, 0.32f), MakeName(TEXT("ChapelPew")));
	PlaceCube(Origin + FVector(90.f, -20.f, 42.f), FVector(1.5f, 0.45f, 0.42f), FLinearColor(0.62f, 0.42f, 0.32f), MakeName(TEXT("ChapelPew")));
	PlaceCube(Origin + FVector(0.f, -90.f, 42.f), FVector(1.6f, 0.45f, 0.42f), FLinearColor(0.58f, 0.4f, 0.3f), MakeName(TEXT("ChapelPew")));
	PlaceCube(Origin + FVector(-140.f, 80.f, 48.f), FVector(0.55f, 0.55f, 0.55f), FLinearColor(0.55f, 0.72f, 0.92f), MakeName(TEXT("ChapelFont")));
	PlaceCube(Origin + FVector(-180.f, 0.f, 150.f), FVector(0.08f, 0.7f, 0.9f), FLinearColor(0.82f, 0.42f, 0.72f), MakeName(TEXT("ChapelGlass")));
	PlaceCube(Origin + FVector(180.f, 0.f, 150.f), FVector(0.08f, 0.7f, 0.9f), FLinearColor(0.42f, 0.62f, 0.95f), MakeName(TEXT("ChapelGlass")));
	PlaceCube(Origin + FVector(0.f, 210.f, 210.f), FVector(0.9f, 0.12f, 0.5f), Trim, MakeName(TEXT("ChapelCap")));
	SpawnLamp(this, Origin + FVector(0.f, 40.f, 200.f), FLinearColor(1.f, 0.9f, 0.62f), 3200.f);
}

void AHolypawWorldBuilder::DressWorkshopRoom(const FVector& Origin)
{
	const FLinearColor Wall(0.72f, 0.48f, 0.68f);
	const FLinearColor Floor(0.55f, 0.4f, 0.38f);
	const FLinearColor Trim(0.45f, 0.28f, 0.42f);
	DressRoomShell(Origin, Wall, Floor, Trim);
	PlaceCube(Origin + FVector(-80.f, 80.f, 70.f), FVector(1.6f, 0.25f, 1.1f), FLinearColor(0.48f, 0.32f, 0.28f), MakeName(TEXT("WorkshopLoom")));
	PlaceCube(Origin + FVector(-80.f, 80.f, 90.f), FVector(1.4f, 0.08f, 0.08f), FLinearColor(0.92f, 0.55f, 0.72f), MakeName(TEXT("LoomThread")));
	PlaceCube(Origin + FVector(110.f, 40.f, 55.f), FVector(1.1f, 0.7f, 0.7f), FLinearColor(0.52f, 0.38f, 0.3f), MakeName(TEXT("WorkBench")));
	PlaceCube(Origin + FVector(140.f, -70.f, 80.f), FVector(0.5f, 0.35f, 1.4f), FLinearColor(0.58f, 0.42f, 0.35f), MakeName(TEXT("RibbonShelf")));
	PlaceCube(Origin + FVector(140.f, -70.f, 70.f), FVector(0.28f, 0.28f, 0.28f), FLinearColor(0.95f, 0.45f, 0.55f), MakeName(TEXT("RibbonSpool")));
	PlaceCube(Origin + FVector(140.f, -70.f, 110.f), FVector(0.28f, 0.28f, 0.28f), FLinearColor(0.45f, 0.78f, 0.72f), MakeName(TEXT("RibbonSpool")));
	PlaceCube(Origin + FVector(-20.f, -50.f, 28.f), FVector(0.9f, 0.9f, 0.08f), FLinearColor(0.78f, 0.62f, 0.42f), MakeName(TEXT("WorkshopRug")));
	PlaceCube(Origin + FVector(40.f, 140.f, 48.f), FVector(0.7f, 0.45f, 0.5f), FLinearColor(0.42f, 0.32f, 0.28f), MakeName(TEXT("ScrapBin")));
	SpawnLamp(this, Origin + FVector(20.f, 10.f, 188.f), FLinearColor(0.95f, 0.72f, 1.f), 2600.f);
}

void AHolypawWorldBuilder::DressInterior(const FVector& Origin, const EHolypawShrineKind Kind)
{
	if (Kind == EHolypawShrineKind::Chapel)
	{
		DressChapelRoom(Origin);
		return;
	}
	if (Kind == EHolypawShrineKind::Workshop)
	{
		DressWorkshopRoom(Origin);
		return;
	}
	DressInnRoom(Origin);
}

void AHolypawWorldBuilder::DressCottageRooms(const FVector& Origin, const float GroundZ)
{
	const FVector Base(Origin.X, Origin.Y, GroundZ);
	const FLinearColor Wall(0.72f, 0.48f, 0.36f);
	const FLinearColor Trim(0.38f, 0.24f, 0.18f);
	PlaceCube(Base + FVector(-270.f, 0.f, 160.f), FVector(0.18f, 5.2f, 3.15f), Wall, MakeName(TEXT("CottageWallW")));
	PlaceCube(Base + FVector(0.f, -260.f, 160.f), FVector(5.4f, 0.18f, 3.15f), Wall, MakeName(TEXT("CottageWallS")));
	PlaceCube(Base + FVector(0.f, 260.f, 160.f), FVector(5.4f, 0.18f, 3.15f), Wall, MakeName(TEXT("CottageWallN")));
	PlaceCube(Base + FVector(270.f, -160.f, 160.f), FVector(0.18f, 2.0f, 3.15f), Wall, MakeName(TEXT("CottageWallE_L")));
	PlaceCube(Base + FVector(270.f, 160.f, 160.f), FVector(0.18f, 2.0f, 3.15f), Wall, MakeName(TEXT("CottageWallE_R")));
	PlaceCube(Base + FVector(270.f, 0.f, 268.f), FVector(0.18f, 1.3f, 0.7f), Trim, MakeName(TEXT("CottageLintel")));
	PlaceCube(Base + FVector(292.f, -78.f, 150.f), FVector(0.12f, 0.85f, 2.2f), Trim, MakeName(TEXT("CottageDoor")));
	PlaceCube(Base + FVector(80.f, -140.f, 70.f), FVector(1.6f, 0.55f, 0.7f), FLinearColor(0.55f, 0.38f, 0.28f), MakeName(TEXT("CottageCounter")));
	PlaceCube(Base + FVector(160.f, -140.f, 78.f), FVector(0.7f, 0.55f, 0.85f), FLinearColor(0.42f, 0.32f, 0.28f), MakeName(TEXT("CottageStove")));
	PlaceCube(Base + FVector(-40.f, -180.f, 110.f), FVector(0.45f, 0.28f, 1.3f), FLinearColor(0.5f, 0.36f, 0.28f), MakeName(TEXT("CottageShelf")));
	PlaceCube(Base + FVector(20.f, 20.f, 32.f), FVector(1.4f, 1.1f, 0.08f), FLinearColor(0.72f, 0.42f, 0.38f), MakeName(TEXT("CottageRug")));
	PlaceCube(Base + FVector(-40.f, 180.f, 200.f), FVector(2.2f, 0.1f, 0.18f), Trim, MakeName(TEXT("LoftRail")));
	PlaceCube(Base + FVector(40.f, 40.f, 175.f), FVector(0.9f, 0.12f, 0.9f), FLinearColor(0.55f, 0.82f, 0.95f), MakeName(TEXT("CottagePane")));
	SpawnLamp(this, Base + FVector(0.f, 0.f, 210.f), FLinearColor(1.f, 0.84f, 0.6f), 2400.f);
}

bool AHolypawWorldBuilder::IsPlayerIndoors(const FVector& WorldPos) const
{
	if (!GetWorld())
	{
		return false;
	}
	for (TActorIterator<AHolypawShrine> It(GetWorld()); It; ++It)
	{
		const AHolypawShrine* S = *It;
		if (!S)
		{
			continue;
		}
		if (S->Kind != EHolypawShrineKind::Inn && S->Kind != EHolypawShrineKind::Chapel && S->Kind != EHolypawShrineKind::Workshop)
		{
			continue;
		}
		const FVector D = WorldPos - S->GetActorLocation();
		if (FMath::Abs(D.X) < 200.f && FMath::Abs(D.Y) < 200.f && D.Z > -40.f && D.Z < 250.f)
		{
			return true;
		}
	}
	const FVector Cottage(CottageSpawn.X, CottageSpawn.Y, CottageSpawn.Z);
	const FVector C = WorldPos - Cottage;
	if (FMath::Abs(C.X) < 280.f && FMath::Abs(C.Y) < 250.f && C.Z > -80.f && C.Z < 280.f)
	{
		return true;
	}
	return false;
}
