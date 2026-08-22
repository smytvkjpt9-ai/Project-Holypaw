#include "HolypawWorldBuilder.h"
#include "Actors/HolypawShrine.h"
#include "Look/HolypawLook.h"
#include "Actors/FaithStall.h"
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
	PlaceCube(Origin + FVector(-120.f, 204.f, 150.f), FVector(0.42f, 0.04f, 0.42f), HolypawLook::GoldWarm, MakeName(TEXT("RoomPaneWarm")), FRotator::ZeroRotator, false);
	PlaceCube(Origin + FVector(120.f, 204.f, 150.f), FVector(0.42f, 0.04f, 0.42f), HolypawLook::GoldWarm, MakeName(TEXT("RoomPaneWarmB")), FRotator::ZeroRotator, false);
}

void AHolypawWorldBuilder::DressInnRoom(const FVector& Origin)
{
	const FLinearColor Wall(0.78f, 0.52f, 0.42f);
	const FLinearColor Floor(0.62f, 0.44f, 0.32f);
	const FLinearColor Trim(0.42f, 0.28f, 0.2f);
	DressRoomShell(Origin, Wall, Floor, Trim);
	PlaceBed(Origin + FVector(-110.f, 90.f, 8.f), 0.f, FLinearColor(0.92f, 0.62f, 0.55f), TEXT("InnBed"));
	PlaceTable(Origin + FVector(120.f, 70.f, 8.f), 0.f);
	PlaceChair(Origin + FVector(120.f, 8.f, 8.f), 180.f);
	PlaceChair(Origin + FVector(70.f, 70.f, 8.f), 90.f);
	PlaceRug(Origin + FVector(20.f, -20.f, 16.f), FLinearColor(0.72f, 0.38f, 0.42f), FVector(1.6f, 1.2f, 0.06f));
	PlaceShelf(Origin + FVector(-40.f, -160.f, 8.f), 0.f);
	PlaceCrate(Origin + FVector(150.f, -140.f, 8.f));
	PlaceCube(Origin + FVector(120.f, 70.f, 108.f), FVector(0.16f, 0.16f, 0.42f), HolypawLook::GoldWarm, MakeName(TEXT("InnLamp")));
	PlacePrimitive(SphereMesh, Origin + FVector(120.f, 70.f, 132.f), FVector(0.18f, 0.18f, 0.18f), HolypawLook::Lantern, MakeName(TEXT("InnLampGlow")), FRotator::ZeroRotator, false);
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
	PlacePrimitive(SphereMesh, Origin + FVector(0.f, 130.f, 168.f), FVector(0.22f, 0.22f, 0.22f), HolypawLook::GoldWarm, MakeName(TEXT("ChapelOrb")), FRotator::ZeroRotator, false);
	PlaceBench(Origin + FVector(-90.f, -20.f, 8.f), 0.f); // ChapelPew
	PlaceBench(Origin + FVector(90.f, -20.f, 8.f), 0.f);
	PlaceBench(Origin + FVector(0.f, -100.f, 8.f), 0.f);
	PlaceRug(Origin + FVector(0.f, 40.f, 16.f), HolypawLook::RoseDeep, FVector(1.2f, 2.2f, 0.05f));
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
	PlaceCube(Origin + FVector(-80.f, 80.f, 118.f), FVector(1.4f, 0.08f, 0.08f), HolypawLook::Mint, MakeName(TEXT("LoomThreadB")));
	PlaceTable(Origin + FVector(110.f, 40.f, 8.f), 0.f);
	PlaceChair(Origin + FVector(110.f, -20.f, 8.f), 180.f);
	PlaceShelf(Origin + FVector(140.f, -150.f, 8.f), 0.f);
	PlaceRug(Origin + FVector(-20.f, -40.f, 16.f), FLinearColor(0.78f, 0.62f, 0.42f), FVector(1.1f, 1.1f, 0.06f));
	PlaceCrate(Origin + FVector(40.f, 150.f, 8.f));
	PlaceCrate(Origin + FVector(-20.f, 150.f, 8.f));
	PlacePrimitive(SphereMesh, Origin + FVector(140.f, -150.f, 92.f), FVector(0.16f, 0.16f, 0.16f), HolypawLook::Rose, MakeName(TEXT("RibbonSpool")), FRotator::ZeroRotator, false);
	PlacePrimitive(SphereMesh, Origin + FVector(140.f, -150.f, 118.f), FVector(0.16f, 0.16f, 0.16f), HolypawLook::Mint, MakeName(TEXT("RibbonSpoolB")), FRotator::ZeroRotator, false);
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
	PlaceCube(Base + FVector(304.f, -50.f, 148.f), FVector(0.07f, 0.07f, 0.07f), HolypawLook::Gold, MakeName(TEXT("DoorKnob")));
	PlaceTable(Base + FVector(80.f, -140.f, 8.f), 0.f);
	PlaceHearth(Base + FVector(160.f, -180.f, 8.f));
	PlaceShelf(Base + FVector(-40.f, -200.f, 8.f), 0.f);
	PlaceRug(Base + FVector(20.f, 20.f, 16.f), FLinearColor(0.72f, 0.42f, 0.38f), FVector(1.8f, 1.4f, 0.06f));
	PlaceChair(Base + FVector(80.f, -80.f, 8.f), 180.f);
	PlaceCrate(Base + FVector(-180.f, -160.f, 8.f));
	PlaceCube(Base + FVector(-40.f, 180.f, 200.f), FVector(2.2f, 0.1f, 0.18f), Trim, MakeName(TEXT("LoftRail")));
	PlaceCube(Base + FVector(40.f, 40.f, 175.f), FVector(0.9f, 0.12f, 0.9f), FLinearColor(0.55f, 0.82f, 0.95f), MakeName(TEXT("CottagePane")));
	SpawnLamp(this, Base + FVector(0.f, 0.f, 210.f), FLinearColor(1.f, 0.84f, 0.6f), 2400.f);
}

void AHolypawWorldBuilder::DressShopRoom(const FVector& Origin)
{
	const FLinearColor Wall(0.95f, 0.78f, 0.42f);
	const FLinearColor Floor(0.78f, 0.62f, 0.38f);
	const FLinearColor Trim(0.72f, 0.48f, 0.22f);
	DressRoomShell(Origin, Wall, Floor, Trim);
	PlaceCube(Origin + FVector(0.f, 90.f, 58.f), FVector(2.2f, 0.55f, 0.75f), FLinearColor(0.62f, 0.42f, 0.28f), MakeName(TEXT("ShopCounter")));
	PlaceCube(Origin + FVector(-130.f, 40.f, 90.f), FVector(0.45f, 0.35f, 1.5f), FLinearColor(0.55f, 0.4f, 0.32f), MakeName(TEXT("JarShelf")));
	PlaceCube(Origin + FVector(-130.f, 40.f, 70.f), FVector(0.28f, 0.28f, 0.28f), FLinearColor(0.85f, 0.55f, 0.9f), MakeName(TEXT("FaithJar")));
	PlaceCube(Origin + FVector(-130.f, 40.f, 110.f), FVector(0.28f, 0.28f, 0.28f), FLinearColor(0.95f, 0.72f, 0.45f), MakeName(TEXT("FaithJar")));
	PlaceCube(Origin + FVector(120.f, 20.f, 48.f), FVector(0.7f, 0.55f, 0.55f), FLinearColor(0.98f, 0.82f, 0.55f), MakeName(TEXT("BunBasket")));
	PlaceCube(Origin + FVector(40.f, -70.f, 28.f), FVector(1.1f, 0.8f, 0.08f), FLinearColor(0.92f, 0.45f, 0.5f), MakeName(TEXT("ShopRug")));
	PlaceCube(Origin + FVector(140.f, 110.f, 80.f), FVector(0.22f, 0.22f, 0.7f), FLinearColor(0.95f, 0.55f, 0.72f), MakeName(TEXT("RibbonSpool")));
	SpawnLamp(this, Origin + FVector(0.f, 20.f, 188.f), FLinearColor(1.f, 0.88f, 0.55f), 3000.f);
}

void AHolypawWorldBuilder::DressOpenStall(const FVector& Origin)
{
	PlaceCube(Origin + FVector(0.f, 36.f, 10.f), FVector(1.7f, 1.15f, 0.1f), FLinearColor(0.78f, 0.62f, 0.38f), MakeName(TEXT("KioskDeck")));
	PlaceCube(Origin + FVector(0.f, 88.f, 52.f), FVector(1.8f, 0.45f, 0.7f), FLinearColor(0.62f, 0.42f, 0.28f), MakeName(TEXT("ShopCounter")));
	PlaceCube(Origin + FVector(0.f, 18.f, 142.f), FVector(2.05f, 1.45f, 0.08f), FLinearColor(0.95f, 0.55f, 0.62f), MakeName(TEXT("KioskAwning")));
	PlaceCube(Origin + FVector(-72.f, -22.f, 78.f), FVector(0.12f, 0.12f, 1.35f), FLinearColor(0.42f, 0.3f, 0.22f), MakeName(TEXT("AwningPole")));
	PlaceCube(Origin + FVector(72.f, -22.f, 78.f), FVector(0.12f, 0.12f, 1.35f), FLinearColor(0.42f, 0.3f, 0.22f), MakeName(TEXT("AwningPole")));
	PlaceCube(Origin + FVector(68.f, 18.f, 40.f), FVector(0.55f, 0.45f, 0.45f), FLinearColor(0.98f, 0.82f, 0.55f), MakeName(TEXT("BunBasket")));
	PlaceCube(Origin + FVector(-52.f, 52.f, 68.f), FVector(0.22f, 0.22f, 0.55f), FLinearColor(0.95f, 0.55f, 0.72f), MakeName(TEXT("RibbonSpool")));
}

void AHolypawWorldBuilder::DressMillHall(const FVector& Origin)
{
	const FLinearColor Wall(0.58f, 0.54f, 0.5f);
	const FLinearColor Floor(0.42f, 0.4f, 0.38f);
	const FLinearColor Trim(0.72f, 0.28f, 0.38f);
	PlaceCube(Origin + FVector(0.f, 0.f, 8.f), FVector(7.2f, 5.2f, 0.16f), Floor, MakeName(TEXT("MillFloor")));
	PlaceCube(Origin + FVector(360.f, 0.f, 140.f), FVector(0.2f, 5.2f, 2.7f), Wall, MakeName(TEXT("MillBack")));
	PlaceCube(Origin + FVector(0.f, 260.f, 140.f), FVector(7.2f, 0.2f, 2.7f), Wall, MakeName(TEXT("MillNorth")));
	PlaceCube(Origin + FVector(0.f, -260.f, 140.f), FVector(7.2f, 0.2f, 2.7f), Wall, MakeName(TEXT("MillSouth")));
	PlaceCube(Origin + FVector(-360.f, -150.f, 140.f), FVector(0.2f, 2.2f, 2.7f), Wall, MakeName(TEXT("MillWestL")));
	PlaceCube(Origin + FVector(-360.f, 150.f, 140.f), FVector(0.2f, 2.2f, 2.7f), Wall, MakeName(TEXT("MillWestR")));
	PlaceCube(Origin + FVector(-360.f, 0.f, 250.f), FVector(0.2f, 0.9f, 0.55f), Trim, MakeName(TEXT("MillLintel")));
	PlaceCube(Origin + FVector(0.f, 0.f, 280.f), FVector(7.4f, 5.4f, 0.12f), Wall * 0.8f, MakeName(TEXT("MillCeiling")));
	PlaceCube(Origin + FVector(-40.f, 80.f, 42.f), FVector(4.4f, 0.7f, 0.22f), FLinearColor(0.35f, 0.34f, 0.32f), MakeName(TEXT("MillConveyor")));
	PlaceCube(Origin + FVector(80.f, -70.f, 55.f), FVector(0.9f, 0.9f, 0.9f), FLinearColor(0.62f, 0.58f, 0.48f), MakeName(TEXT("PolyVat")));
	PlaceCube(Origin + FVector(180.f, -70.f, 55.f), FVector(0.9f, 0.9f, 0.9f), FLinearColor(0.62f, 0.58f, 0.48f), MakeName(TEXT("PolyVat")));
	PlaceCube(Origin + FVector(120.f, 120.f, 40.f), FVector(1.1f, 0.8f, 0.55f), FLinearColor(0.78f, 0.72f, 0.55f), MakeName(TEXT("PolyBale")));
	PlaceCube(Origin + FVector(-80.f, -120.f, 36.f), FVector(1.1f, 0.8f, 0.55f), FLinearColor(0.78f, 0.72f, 0.55f), MakeName(TEXT("PolyBale")));
	PlaceCube(Origin + FVector(200.f, 0.f, 90.f), FVector(0.35f, 1.6f, 1.4f), FLinearColor(0.32f, 0.3f, 0.28f), MakeName(TEXT("MillPress")));
	SpawnLamp(this, Origin + FVector(0.f, 0.f, 210.f), FLinearColor(0.95f, 0.72f, 0.42f), 2200.f);
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
	// Interior only — porch and yard stay outdoor-bright.
	if (FMath::Abs(C.X) < 110.f && FMath::Abs(C.Y) < 95.f && C.Z > -10.f && C.Z < 210.f)
	{
		return true;
	}
	const FVector Mill(RibbonCity.X + 5200.f, RibbonCity.Y - 800.f, WorldPos.Z);
	if (FMath::Abs(WorldPos.X - Mill.X) < 360.f && FMath::Abs(WorldPos.Y - Mill.Y) < 260.f)
	{
		return true;
	}
	for (TActorIterator<AFaithStall> StallIt(GetWorld()); StallIt; ++StallIt)
	{
		const AFaithStall* Stall = *StallIt;
		if (!Stall)
		{
			continue;
		}
		if (Stall->bOpenAir)
		{
			continue;
		}
		const FVector D = WorldPos - Stall->GetActorLocation();
		if (FMath::Abs(D.X) < 200.f && FMath::Abs(D.Y) < 200.f && D.Z > -40.f && D.Z < 250.f)
		{
			return true;
		}
	}
	return false;
}

void AHolypawWorldBuilder::PlaceBed(const FVector& Origin, const float Yaw, const FLinearColor& Quilt, const TCHAR* Tag)
{
	const FRotator Turn(0.f, Yaw, 0.f);
	auto At = [&](const FVector& Local)
	{
		return Origin + Turn.RotateVector(Local);
	};
	PlaceCube(At(FVector(0.f, 0.f, 18.f)), FVector(2.2f, 1.35f, 0.18f), HolypawLook::Wood, MakeName(Tag ? Tag : TEXT("Bed")), Turn);
	PlaceCube(At(FVector(0.f, 0.f, 32.f)), FVector(2.0f, 1.2f, 0.16f), HolypawLook::Cream, MakeName(TEXT("Mattress")), Turn, false);
	PlaceCube(At(FVector(0.f, 0.f, 42.f)), FVector(1.7f, 1.05f, 0.08f), Quilt, MakeName(TEXT("Quilt")), Turn, false);
	PlaceCube(At(FVector(-80.f, 0.f, 52.f)), FVector(0.55f, 0.95f, 0.22f), HolypawLook::Cream, MakeName(TEXT("Pillow")), Turn, false);
	PlaceCube(At(FVector(-100.f, 0.f, 38.f)), FVector(0.12f, 1.35f, 0.55f), HolypawLook::Wood, MakeName(TEXT("Headboard")), Turn);
	PlaceCube(At(FVector(95.f, 0.f, 22.f)), FVector(0.10f, 1.2f, 0.22f), HolypawLook::Wood, MakeName(TEXT("Footboard")), Turn);
}

void AHolypawWorldBuilder::PlaceChair(const FVector& Origin, const float Yaw)
{
	const FRotator Turn(0.f, Yaw, 0.f);
	auto At = [&](const FVector& Local) { return Origin + Turn.RotateVector(Local); };
	PlaceCube(At(FVector(0.f, 0.f, 28.f)), FVector(0.48f, 0.48f, 0.10f), HolypawLook::Wood, MakeName(TEXT("Seat")), Turn);
	PlaceCube(At(FVector(-16.f, 0.f, 52.f)), FVector(0.08f, 0.48f, 0.42f), HolypawLook::Wood, MakeName(TEXT("ChairBack")), Turn);
	PlaceCube(At(FVector(-16.f, 16.f, 12.f)), FVector(0.08f, 0.08f, 0.24f), HolypawLook::Wood, MakeName(TEXT("ChairLeg")), Turn);
	PlaceCube(At(FVector(-16.f, -16.f, 12.f)), FVector(0.08f, 0.08f, 0.24f), HolypawLook::Wood, MakeName(TEXT("ChairLeg")), Turn);
	PlaceCube(At(FVector(16.f, 16.f, 12.f)), FVector(0.08f, 0.08f, 0.24f), HolypawLook::Wood, MakeName(TEXT("ChairLeg")), Turn);
	PlaceCube(At(FVector(16.f, -16.f, 12.f)), FVector(0.08f, 0.08f, 0.24f), HolypawLook::Wood, MakeName(TEXT("ChairLeg")), Turn);
	PlaceCube(At(FVector(0.f, 0.f, 32.f)), FVector(0.40f, 0.40f, 0.05f), HolypawLook::Rose, MakeName(TEXT("Cushion")), Turn, false);
}

void AHolypawWorldBuilder::PlaceTable(const FVector& Origin, const float Yaw)
{
	const FRotator Turn(0.f, Yaw, 0.f);
	auto At = [&](const FVector& Local) { return Origin + Turn.RotateVector(Local); };
	PlaceCube(At(FVector(0.f, 0.f, 52.f)), FVector(1.15f, 0.85f, 0.08f), HolypawLook::Wood, MakeName(TEXT("TableTop")), Turn);
	PlaceCube(At(FVector(-40.f, -28.f, 24.f)), FVector(0.10f, 0.10f, 0.48f), HolypawLook::Wood, MakeName(TEXT("TableLeg")), Turn);
	PlaceCube(At(FVector(-40.f, 28.f, 24.f)), FVector(0.10f, 0.10f, 0.48f), HolypawLook::Wood, MakeName(TEXT("TableLeg")), Turn);
	PlaceCube(At(FVector(40.f, -28.f, 24.f)), FVector(0.10f, 0.10f, 0.48f), HolypawLook::Wood, MakeName(TEXT("TableLeg")), Turn);
	PlaceCube(At(FVector(40.f, 28.f, 24.f)), FVector(0.10f, 0.10f, 0.48f), HolypawLook::Wood, MakeName(TEXT("TableLeg")), Turn);
	PlacePrimitive(SphereMesh, At(FVector(18.f, 8.f, 64.f)), FVector(0.14f, 0.14f, 0.16f), HolypawLook::Cream, MakeName(TEXT("Mug")), Turn, false);
	PlaceCube(At(FVector(-16.f, -6.f, 58.f)), FVector(0.22f, 0.16f, 0.04f), HolypawLook::GoldWarm, MakeName(TEXT("Plate")), Turn, false);
}

void AHolypawWorldBuilder::PlaceShelf(const FVector& Origin, const float Yaw)
{
	const FRotator Turn(0.f, Yaw, 0.f);
	auto At = [&](const FVector& Local) { return Origin + Turn.RotateVector(Local); };
	PlaceCube(At(FVector(0.f, 0.f, 70.f)), FVector(0.18f, 0.85f, 1.35f), HolypawLook::Wood, MakeName(TEXT("Shelf")), Turn);
	PlaceCube(At(FVector(12.f, 0.f, 40.f)), FVector(0.22f, 0.78f, 0.05f), HolypawLook::Wood, MakeName(TEXT("ShelfBoard")), Turn);
	PlaceCube(At(FVector(12.f, 0.f, 78.f)), FVector(0.22f, 0.78f, 0.05f), HolypawLook::Wood, MakeName(TEXT("ShelfBoard")), Turn);
	PlaceCube(At(FVector(12.f, 0.f, 116.f)), FVector(0.22f, 0.78f, 0.05f), HolypawLook::Wood, MakeName(TEXT("ShelfBoard")), Turn);
	PlacePrimitive(SphereMesh, At(FVector(18.f, -18.f, 50.f)), FVector(0.12f, 0.12f, 0.16f), HolypawLook::Rose, MakeName(TEXT("Jar")), FRotator::ZeroRotator, false);
	PlacePrimitive(SphereMesh, At(FVector(18.f, 12.f, 88.f)), FVector(0.12f, 0.12f, 0.16f), HolypawLook::Mint, MakeName(TEXT("Jar")), FRotator::ZeroRotator, false);
	PlacePrimitive(SphereMesh, At(FVector(18.f, -8.f, 126.f)), FVector(0.12f, 0.12f, 0.16f), HolypawLook::Gold, MakeName(TEXT("Jar")), FRotator::ZeroRotator, false);
}

void AHolypawWorldBuilder::PlaceRug(const FVector& Origin, const FLinearColor& Color, const FVector& Scale)
{
	PlaceCube(Origin, Scale, Color, MakeName(TEXT("Rug")), FRotator::ZeroRotator, false);
}

void AHolypawWorldBuilder::PlaceHearth(const FVector& Origin)
{
	PlaceCube(Origin + FVector(0.f, 0.f, 28.f), FVector(1.15f, 0.55f, 0.55f), FLinearColor(0.38f, 0.28f, 0.24f), MakeName(TEXT("CottageHearth")));
	PlaceCube(Origin + FVector(0.f, 0.f, 18.f), FVector(1.35f, 0.72f, 0.12f), FLinearColor(0.48f, 0.36f, 0.30f), MakeName(TEXT("HearthStone")));
	PlaceCube(Origin + FVector(0.f, 8.f, 42.f), FVector(0.55f, 0.18f, 0.28f), FLinearColor(1.f, 0.45f, 0.18f), MakeName(TEXT("HearthGlow")), FRotator::ZeroRotator, false);
	HolypawLook::SpawnGlow(GetWorld(), this, Origin + FVector(0.f, 0.f, 48.f), FLinearColor(1.f, 0.55f, 0.22f), 1400.f, 360.f);
}

void AHolypawWorldBuilder::PlaceCrate(const FVector& Origin)
{
	PlaceCube(Origin + FVector(0.f, 0.f, 28.f), FVector(0.55f, 0.48f, 0.48f), FLinearColor(0.52f, 0.38f, 0.24f), MakeName(TEXT("Crate")));
	PlaceCube(Origin + FVector(0.f, 0.f, 52.f), FVector(0.58f, 0.50f, 0.06f), HolypawLook::Wood, MakeName(TEXT("CrateLid")));
}

void AHolypawWorldBuilder::PlacePlanter(const FVector& Origin)
{
	PlaceCube(Origin + FVector(0.f, 0.f, 16.f), FVector(0.72f, 0.42f, 0.28f), HolypawLook::Wood, MakeName(TEXT("Planter")));
	PlacePrimitive(SphereMesh, Origin + FVector(-12.f, 0.f, 36.f), FVector(0.18f, 0.16f, 0.16f), HolypawLook::Rose, MakeName(TEXT("Bloom")), FRotator::ZeroRotator, false);
	PlacePrimitive(SphereMesh, Origin + FVector(12.f, 4.f, 34.f), FVector(0.16f, 0.14f, 0.14f), HolypawLook::Mint, MakeName(TEXT("Bloom")), FRotator::ZeroRotator, false);
	PlacePrimitive(SphereMesh, Origin + FVector(0.f, -6.f, 38.f), FVector(0.14f, 0.14f, 0.16f), HolypawLook::Gold, MakeName(TEXT("Bloom")), FRotator::ZeroRotator, false);
}

void AHolypawWorldBuilder::PlaceBench(const FVector& Origin, const float Yaw)
{
	const FRotator Turn(0.f, Yaw, 0.f);
	auto At = [&](const FVector& Local) { return Origin + Turn.RotateVector(Local); };
	PlaceCube(At(FVector(0.f, 0.f, 28.f)), FVector(1.35f, 0.42f, 0.10f), HolypawLook::Wood, MakeName(TEXT("Bench")), Turn);
	PlaceCube(At(FVector(0.f, -14.f, 48.f)), FVector(1.35f, 0.08f, 0.32f), HolypawLook::Wood, MakeName(TEXT("BenchBack")), Turn);
	PlaceCube(At(FVector(-50.f, 0.f, 12.f)), FVector(0.10f, 0.36f, 0.22f), HolypawLook::Wood, MakeName(TEXT("BenchLeg")), Turn);
	PlaceCube(At(FVector(50.f, 0.f, 12.f)), FVector(0.10f, 0.36f, 0.22f), HolypawLook::Wood, MakeName(TEXT("BenchLeg")), Turn);
}

void AHolypawWorldBuilder::PlaceWell(const FVector& Origin)
{
	PlacePrimitive(CylMesh ? CylMesh : CubeMesh, Origin + FVector(0.f, 0.f, 28.f), FVector(0.85f, 0.85f, 0.48f), FLinearColor(0.52f, 0.50f, 0.48f), MakeName(TEXT("Well")));
	PlaceCube(Origin + FVector(0.f, 0.f, 8.f), FVector(1.15f, 1.15f, 0.12f), FLinearColor(0.42f, 0.40f, 0.38f), MakeName(TEXT("WellBase")));
	PlaceCube(Origin + FVector(-40.f, 0.f, 70.f), FVector(0.10f, 0.10f, 0.55f), HolypawLook::Wood, MakeName(TEXT("WellPost")));
	PlaceCube(Origin + FVector(40.f, 0.f, 70.f), FVector(0.10f, 0.10f, 0.55f), HolypawLook::Wood, MakeName(TEXT("WellPost")));
	PlaceCube(Origin + FVector(0.f, 0.f, 100.f), FVector(1.05f, 0.22f, 0.12f), HolypawLook::Roof, MakeName(TEXT("WellRoof")), FRotator(0.f, 0.f, 0.f));
	PlacePrimitive(SphereMesh, Origin + FVector(0.f, 0.f, 22.f), FVector(0.42f, 0.42f, 0.08f), HolypawLook::Water, MakeName(TEXT("WellWater")), FRotator::ZeroRotator, false);
}

void AHolypawWorldBuilder::PlaceAwningStall(const FVector& Origin)
{
	PlaceCube(Origin + FVector(-70.f, -40.f, 55.f), FVector(0.12f, 0.12f, 1.05f), HolypawLook::Wood, MakeName(TEXT("StallPost")));
	PlaceCube(Origin + FVector(70.f, -40.f, 55.f), FVector(0.12f, 0.12f, 1.05f), HolypawLook::Wood, MakeName(TEXT("StallPost")));
	PlaceCube(Origin + FVector(-70.f, 40.f, 55.f), FVector(0.12f, 0.12f, 1.05f), HolypawLook::Wood, MakeName(TEXT("StallPost")));
	PlaceCube(Origin + FVector(70.f, 40.f, 55.f), FVector(0.12f, 0.12f, 1.05f), HolypawLook::Wood, MakeName(TEXT("StallPost")));
	PlaceCube(Origin + FVector(0.f, 0.f, 48.f), FVector(1.55f, 1.15f, 0.10f), HolypawLook::Wood, MakeName(TEXT("Stall")));
	PlaceCube(Origin + FVector(0.f, 0.f, 118.f), FVector(1.85f, 1.35f, 0.08f), HolypawLook::GoldWarm, MakeName(TEXT("Awning")), FRotator(12.f, 0.f, 0.f), false);
	PlacePrimitive(SphereMesh, Origin + FVector(-28.f, 8.f, 62.f), FVector(0.16f, 0.16f, 0.14f), HolypawLook::Rose, MakeName(TEXT("Bun")), FRotator::ZeroRotator, false);
	PlacePrimitive(SphereMesh, Origin + FVector(0.f, -6.f, 64.f), FVector(0.14f, 0.18f, 0.16f), HolypawLook::Mint, MakeName(TEXT("Jar")), FRotator::ZeroRotator, false);
	PlacePrimitive(SphereMesh, Origin + FVector(26.f, 10.f, 62.f), FVector(0.16f, 0.14f, 0.14f), HolypawLook::Gold, MakeName(TEXT("Ribbon")), FRotator::ZeroRotator, false);
	PlaceCrate(Origin + FVector(90.f, -20.f, 0.f));
}

void AHolypawWorldBuilder::PlaceCampKit(const FVector& Origin)
{
	PlaceCube(Origin + FVector(-36.f, 0.f, 78.f), FVector(0.12f, 2.05f, 1.55f), FLinearColor(0.62f, 0.42f, 0.28f), MakeName(TEXT("Tent")), FRotator(0.f, 0.f, 38.f));
	PlaceCube(Origin + FVector(36.f, 0.f, 78.f), FVector(0.12f, 2.05f, 1.55f), FLinearColor(0.58f, 0.38f, 0.26f), MakeName(TEXT("Tent")), FRotator(0.f, 0.f, -38.f));
	PlaceCube(Origin + FVector(0.f, -88.f, 62.f), FVector(1.15f, 0.10f, 1.15f), FLinearColor(0.55f, 0.36f, 0.24f), MakeName(TEXT("TentBack")));
	PlaceRug(Origin + FVector(0.f, 8.f, 6.f), FLinearColor(0.42f, 0.28f, 0.20f), FVector(1.1f, 1.4f, 0.05f));
	PlaceCube(Origin + FVector(-8.f, 16.f, 16.f), FVector(0.85f, 0.42f, 0.16f), HolypawLook::RoseDeep, MakeName(TEXT("Bedroll")), FRotator::ZeroRotator, false);
	PlacePrimitive(SphereMesh, Origin + FVector(80.f, 0.f, 28.f), FVector(0.42f, 0.42f, 0.22f), FLinearColor(1.f, 0.48f, 0.16f), MakeName(TEXT("Fire")), FRotator::ZeroRotator, false);
	PlacePrimitive(SphereMesh, Origin + FVector(80.f, 0.f, 48.f), FVector(0.32f, 0.32f, 0.28f), FLinearColor(1.f, 0.72f, 0.28f), MakeName(TEXT("FireGlow")), FRotator::ZeroRotator, false);
	PlacePrimitive(CylMesh ? CylMesh : CubeMesh, Origin + FVector(56.f, 36.f, 14.f), FVector(0.18f, 0.18f, 0.55f), HolypawLook::Wood, MakeName(TEXT("Log")), FRotator(0.f, 40.f, 82.f));
	PlacePrimitive(CylMesh ? CylMesh : CubeMesh, Origin + FVector(108.f, -28.f, 14.f), FVector(0.18f, 0.18f, 0.55f), HolypawLook::Wood, MakeName(TEXT("Log")), FRotator(0.f, -30.f, 82.f));
	PlacePrimitive(CylMesh ? CylMesh : CubeMesh, Origin + FVector(92.f, 40.f, 14.f), FVector(0.16f, 0.16f, 0.48f), HolypawLook::Wood, MakeName(TEXT("Log")), FRotator(0.f, 10.f, 82.f));
	PlacePrimitive(SphereMesh, Origin + FVector(80.f, 0.f, 62.f), FVector(0.18f, 0.18f, 0.16f), FLinearColor(0.38f, 0.32f, 0.30f), MakeName(TEXT("Pot")), FRotator::ZeroRotator, false);
	HolypawLook::SpawnGlow(GetWorld(), this, Origin + FVector(80.f, 0.f, 48.f), FLinearColor(1.f, 0.55f, 0.22f), 2200.f, 520.f);
}

void AHolypawWorldBuilder::PlaceYardFence(const FVector& Origin)
{
	for (int32 I = 0; I < 7; ++I)
	{
		const float Y = -180.f + I * 60.f;
		PlaceCube(Origin + FVector(-240.f, Y, 28.f), FVector(0.08f, 0.08f, 0.55f), HolypawLook::Wood, MakeName(TEXT("FencePost")));
	}
	PlaceCube(Origin + FVector(-240.f, 0.f, 42.f), FVector(0.06f, 3.6f, 0.06f), HolypawLook::Wood, MakeName(TEXT("FenceRail")));
	PlaceCube(Origin + FVector(-240.f, 0.f, 22.f), FVector(0.06f, 3.6f, 0.06f), HolypawLook::Wood, MakeName(TEXT("FenceRail")));
	for (int32 I = 0; I < 5; ++I)
	{
		const float X = -200.f + I * 70.f;
		PlaceCube(Origin + FVector(X, -220.f, 28.f), FVector(0.08f, 0.08f, 0.55f), HolypawLook::Wood, MakeName(TEXT("FencePost")));
	}
	PlaceCube(Origin + FVector(-60.f, -220.f, 42.f), FVector(2.8f, 0.06f, 0.06f), HolypawLook::Wood, MakeName(TEXT("FenceRail")));
}

void AHolypawWorldBuilder::PlaceClothesline(const FVector& Origin)
{
	PlaceCube(Origin + FVector(-70.f, 0.f, 55.f), FVector(0.08f, 0.08f, 1.05f), HolypawLook::Wood, MakeName(TEXT("LinePost")));
	PlaceCube(Origin + FVector(70.f, 0.f, 55.f), FVector(0.08f, 0.08f, 1.05f), HolypawLook::Wood, MakeName(TEXT("LinePost")));
	PlaceCube(Origin + FVector(0.f, 0.f, 108.f), FVector(1.45f, 0.03f, 0.03f), HolypawLook::Cream, MakeName(TEXT("Clothesline")), FRotator::ZeroRotator, false);
	PlaceCube(Origin + FVector(-24.f, 0.f, 92.f), FVector(0.22f, 0.04f, 0.32f), HolypawLook::Rose, MakeName(TEXT("Laundry")), FRotator::ZeroRotator, false);
	PlaceCube(Origin + FVector(18.f, 0.f, 90.f), FVector(0.28f, 0.04f, 0.36f), HolypawLook::Mint, MakeName(TEXT("Laundry")), FRotator::ZeroRotator, false);
	PlaceCube(Origin + FVector(48.f, 0.f, 94.f), FVector(0.16f, 0.04f, 0.22f), HolypawLook::Cream, MakeName(TEXT("Laundry")), FRotator::ZeroRotator, false);
}

void AHolypawWorldBuilder::DressCityPlaza(const FVector2D& Center, const float Z)
{
	PlaceBench(FVector(Center.X + 160.f, Center.Y - 80.f, Z), -20.f);
	PlaceBench(FVector(Center.X - 80.f, Center.Y + 200.f, Z), 110.f);
	PlacePlanter(FVector(Center.X + 220.f, Center.Y + 40.f, Z));
	PlacePlanter(FVector(Center.X - 200.f, Center.Y - 40.f, Z));
	PlaceWell(FVector(Center.X + 40.f, Center.Y + 40.f, Z));
	PlaceCrate(FVector(Center.X + 280.f, Center.Y - 160.f, Z));
	PlaceCrate(FVector(Center.X + 320.f, Center.Y - 140.f, Z));
}
