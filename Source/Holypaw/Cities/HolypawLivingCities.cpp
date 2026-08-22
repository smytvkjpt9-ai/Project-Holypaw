#include "HolypawWorldBuilder.h"

void AHolypawWorldBuilder::BuildQuiltlandDistricts()
{
	const FVector2D Gate = Quiltland + FVector2D(40.f, 20.f);
	const FVector2D Patch = Quiltland + FVector2D(-720.f, 280.f);
	const FVector2D Choir = Quiltland + FVector2D(80.f, 920.f);
	const FVector2D Stoop = Quiltland + FVector2D(780.f, -180.f);
	const FVector2D Path = Quiltland + FVector2D(-120.f, -760.f);

	PlaceSign(Gate, NSLOCTEXT("Holypaw", "QuiltGate", "Pine Gate  |  patchwork pines, ribbons as civic policy"));
	PlaceSign(Patch, NSLOCTEXT("Holypaw", "QuiltPatch", "Patch Walk  |  squares that remember fingers"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "QuiltChoir", "Pine Choir  |  hymns hung on needles"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "QuiltStoop", "Ribbon Stoop  |  wishes come back plaid"));
	PlaceSign(Path, NSLOCTEXT("Holypaw", "QuiltPathN", "Needle Path  |  Mossgate south, Dust Mesa is a long beige"));

	PlaceShrine(Gate + FVector2D(200.f, 50.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "PineInn", "Pine Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "PineChap", "Pine Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "RibbonFontQ", "Ribbon Font"));
	PlaceShrine(Path + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "QuiltCrate", "Patch Crate"));
	PlacePickup(Patch + FVector2D(50.f, -40.f), TEXT("pinePatch"), NSLOCTEXT("Holypaw", "PatchPick", "pine patch"));

	for (int32 I = 0; I < 6; ++I)
	{
		const float X = Patch.X + (I % 3) * 140.f;
		const float Y = Patch.Y + (I / 3) * 110.f;
		const float Z = SampleHeight(X, Y);
		const FLinearColor C = (I % 2 == 0) ? FLinearColor(0.72f, 0.42f, 0.48f) : FLinearColor(0.42f, 0.62f, 0.48f);
		PlaceCube(FVector(X, Y, Z + 18.f), FVector(1.1f, 1.1f, 0.12f), C, MakeName(TEXT("QuiltSq")));
	}
	PlaceCube(FVector(Gate.X, Gate.Y, SampleHeight(Gate.X, Gate.Y) + 160.f), FVector(0.25f, 2.2f, 3.1f), FLinearColor(0.38f, 0.52f, 0.4f), MakeName(TEXT("PinePost")));
}

void AHolypawWorldBuilder::BuildDustMesaDistricts()
{
	const FVector2D Rim = DustMesa + FVector2D(20.f, 40.f);
	const FVector2D Canyon = DustMesa + FVector2D(-800.f, 200.f);
	const FVector2D Choir = DustMesa + FVector2D(100.f, 880.f);
	const FVector2D Stoop = DustMesa + FVector2D(720.f, -220.f);
	const FVector2D Notch = DustMesa + FVector2D(-80.f, -700.f);

	PlaceSign(Rim, NSLOCTEXT("Holypaw", "MesaRim", "Bead Rim  |  canyons full of lost inventory"));
	PlaceSign(Canyon, NSLOCTEXT("Holypaw", "MesaCanyon", "Canyon Walk  |  identical cacti are mill interns"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "MesaChoir", "Dust Choir  |  hymns that dry mid-clap"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "MesaStoop", "Shade Stoop  |  wishes come back warm"));
	PlaceSign(Notch, NSLOCTEXT("Holypaw", "MesaNotch", "Intern Notch  |  Andes Loom further south if the beige argues"));

	PlaceShrine(Rim + FVector2D(180.f, 60.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "BeadInn", "Bead Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "DustChap", "Dust Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "ShadeFont", "Shade Font"));
	PlaceShrine(Notch + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "MesaCrate", "Bead Crate"));
	PlacePickup(Canyon + FVector2D(40.f, -50.f), TEXT("lostBead"), NSLOCTEXT("Holypaw", "BeadPick", "lost bead"));

	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Canyon.X + I * 130.f;
		const float Y = Canyon.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 90.f), FVector(0.22f, 0.22f, 1.8f), FLinearColor(0.42f, 0.62f, 0.32f), MakeName(TEXT("CactusIntern")));
		PlaceCube(FVector(X, Y, Z + 8.f), FVector(0.9f, 0.5f, 0.12f), FLinearColor(0.82f, 0.62f, 0.38f), MakeName(TEXT("BeadShelf")));
	}
	PlaceCube(FVector(Rim.X, Rim.Y, SampleHeight(Rim.X, Rim.Y) + 40.f), FVector(2.8f, 1.2f, 0.35f), FLinearColor(0.78f, 0.55f, 0.32f), MakeName(TEXT("MesaRim")));
}

void AHolypawWorldBuilder::BuildClockhavenDistricts()
{
	const FVector2D Square = Clockhaven + FVector2D(60.f, -20.f);
	const FVector2D Bells = Clockhaven + FVector2D(-780.f, 320.f);
	const FVector2D Choir = Clockhaven + FVector2D(120.f, 980.f);
	const FVector2D Stoop = Clockhaven + FVector2D(820.f, -160.f);
	const FVector2D Cobble = Clockhaven + FVector2D(-100.f, -820.f);

	PlaceSign(Square, NSLOCTEXT("Holypaw", "ClockSq", "Bell Square  |  teatime that applauds on delay"));
	PlaceSign(Bells, NSLOCTEXT("Holypaw", "ClockBells", "Cog Walk  |  gears that refuse to be identical"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "ClockChoir", "Fog Choir  |  hymns in wet wool"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "ClockStoop", "Tea Stoop  |  wishes steeped, not stamped"));
	PlaceSign(Cobble, NSLOCTEXT("Holypaw", "ClockCobble", "Cobble Notch  |  Velvet Seine south if you like pastry coups"));

	PlaceShrine(Square + FVector2D(220.f, 70.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "TeaInn", "Tea Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "FogChap", "Fog Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "TeaFont", "Tea Font"));
	PlaceShrine(Cobble + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "CogCrate", "Cog Crate"));
	PlacePickup(Bells + FVector2D(40.f, -40.f), TEXT("clockCog"), NSLOCTEXT("Holypaw", "CogPick", "clock cog"));
	PlaceStall(Square + FVector2D(140.f, -90.f));

	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Bells.X + I * 150.f;
		const float Y = Bells.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 180.f), FVector(0.35f, 0.35f, 3.2f), FLinearColor(0.58f, 0.62f, 0.72f), MakeName(TEXT("BellTower")));
		PlaceCube(FVector(X, Y, Z + 330.f), FVector(0.7f, 0.7f, 0.28f), FLinearColor(0.92f, 0.78f, 0.42f), MakeName(TEXT("BellHat")));
	}
	PlaceCube(FVector(Square.X, Square.Y, SampleHeight(Square.X, Square.Y) + 90.f), FVector(1.4f, 1.4f, 1.4f), FLinearColor(0.62f, 0.68f, 0.78f), MakeName(TEXT("ClockFace")));
}

void AHolypawWorldBuilder::BuildVelvetSeineDistricts()
{
	const FVector2D Quay = VelvetSeine + FVector2D(80.f, 40.f);
	const FVector2D Bakery = VelvetSeine + FVector2D(-700.f, 240.f);
	const FVector2D Choir = VelvetSeine + FVector2D(60.f, 940.f);
	const FVector2D Stoop = VelvetSeine + FVector2D(760.f, -200.f);
	const FVector2D Bridge = VelvetSeine + FVector2D(-40.f, -780.f);

	PlaceSign(Quay, NSLOCTEXT("Holypaw", "SeineQuay", "Ribbon Quay  |  river that blesses strangers"));
	PlaceSign(Bakery, NSLOCTEXT("Holypaw", "SeineBake", "Blessing Walk  |  buns with no faces on purpose"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "SeineChoir", "Velvet Choir  |  hymns that flake like pastry"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "SeineStoop", "Bridge Stoop  |  wishes buttered"));
	PlaceSign(Bridge, NSLOCTEXT("Holypaw", "SeineBridge", "Hat Bridge  |  Marble Forum if the stone gets smug"));

	PlaceShrine(Quay + FVector2D(200.f, 60.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "PastryInn", "Pastry Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "VelvetChap", "Velvet Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "ButterFont", "Butter Font"));
	PlaceShrine(Bridge + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "SeineCrate", "Quay Crate"));
	PlacePickup(Bakery + FVector2D(50.f, -40.f), TEXT("facelessBun"), NSLOCTEXT("Holypaw", "BunPickS", "faceless bun"));
	PlaceStall(Bakery + FVector2D(120.f, 40.f));

	for (int32 I = 0; I < 6; ++I)
	{
		const float X = Bridge.X + I * 90.f;
		const float Y = Bridge.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 12.f + I * 4.f), FVector(1.4f, 3.2f, 0.14f), FLinearColor(0.72f, 0.52f, 0.62f), MakeName(TEXT("RibbonArch")));
	}
	PlaceCube(FVector(Bakery.X, Bakery.Y, SampleHeight(Bakery.X, Bakery.Y) + 70.f), FVector(2.2f, 1.6f, 1.2f), FLinearColor(0.88f, 0.72f, 0.48f), MakeName(TEXT("Oven")));
}

void AHolypawWorldBuilder::BuildMarbleForumDistricts()
{
	const FVector2D Plaza = MarbleForum + FVector2D(40.f, 20.f);
	const FVector2D Hats = MarbleForum + FVector2D(-740.f, 260.f);
	const FVector2D Choir = MarbleForum + FVector2D(80.f, 900.f);
	const FVector2D Stoop = MarbleForum + FVector2D(780.f, -180.f);
	const FVector2D Sit = MarbleForum + FVector2D(-60.f, -740.f);

	PlaceSign(Plaza, NSLOCTEXT("Holypaw", "ForumPlaza", "Hat Plaza  |  columns wearing tiny hats"));
	PlaceSign(Hats, NSLOCTEXT("Holypaw", "ForumHats", "Column Walk  |  nine-year sits, converted growth"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "ForumChoir", "Stone Choir  |  hymns that echo on purpose"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "ForumStoop", "Marble Stoop  |  wishes in Latin nobody remembers"));
	PlaceSign(Sit, NSLOCTEXT("Holypaw", "ForumSit", "Sit Notch  |  Ivory Spire if you need a stamp"));

	PlaceShrine(Plaza + FVector2D(210.f, 70.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "ColumnInn", "Column Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "StoneChap", "Stone Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "LatinFont", "Latin Font"));
	PlaceShrine(Sit + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "HatCrate", "Hat Crate"));
	PlacePickup(Hats + FVector2D(40.f, -40.f), TEXT("columnHat"), NSLOCTEXT("Holypaw", "HatPick", "column hat"));

	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Hats.X + I * 140.f;
		const float Y = Hats.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 140.f), FVector(0.32f, 0.32f, 2.8f), FLinearColor(0.88f, 0.82f, 0.7f), MakeName(TEXT("Column")));
		PlaceCube(FVector(X, Y, Z + 290.f), FVector(0.55f, 0.55f, 0.22f), FLinearColor(0.72f, 0.42f, 0.55f), MakeName(TEXT("TinyHat")));
	}
	PlaceCube(FVector(Plaza.X, Plaza.Y, SampleHeight(Plaza.X, Plaza.Y) + 16.f), FVector(3.4f, 3.4f, 0.12f), FLinearColor(0.82f, 0.76f, 0.62f), MakeName(TEXT("ForumFloor")));
}

void AHolypawWorldBuilder::BuildIvorySpireDistricts()
{
	const FVector2D Court = IvorySpire + FVector2D(50.f, 10.f);
	const FVector2D Archive = IvorySpire + FVector2D(-760.f, 280.f);
	const FVector2D Choir = IvorySpire + FVector2D(90.f, 940.f);
	const FVector2D Stoop = IvorySpire + FVector2D(740.f, -160.f);
	const FVector2D Shore = IvorySpire + FVector2D(-40.f, -780.f);

	PlaceSign(Court, NSLOCTEXT("Holypaw", "IvoryCourt", "Stamp Court  |  old stone, new stuffing, legal because I said so"));
	PlaceSign(Archive, NSLOCTEXT("Holypaw", "IvoryArchive", "Lost Form Walk  |  the mill franchise drowned in paper"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "IvoryChoir", "Ivory Choir  |  hymns filed in triplicate"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "IvoryStoop", "Spire Stoop  |  wishes notarized"));
	PlaceSign(Shore, NSLOCTEXT("Holypaw", "IvoryShore", "North Shore  |  Spice Harbor if you like sneezing politically"));

	PlaceShrine(Court + FVector2D(200.f, 60.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "IvoryInn", "Ivory Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "IvoryChap", "Ivory Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "NotaryFont", "Notary Font"));
	PlaceShrine(Shore + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "IvoryCrate", "Form Crate"));
	PlacePickup(Archive + FVector2D(40.f, -40.f), TEXT("ivoryForm"), NSLOCTEXT("Holypaw", "FormPick", "lost form"));

	PlaceCube(FVector(Court.X, Court.Y, SampleHeight(Court.X, Court.Y) + 280.f), FVector(0.9f, 0.9f, 5.4f), FLinearColor(0.92f, 0.88f, 0.78f), MakeName(TEXT("IvoryNeedle")));
	PlaceCube(FVector(Court.X, Court.Y, SampleHeight(Court.X, Court.Y) + 560.f), FVector(1.6f, 1.6f, 0.4f), FLinearColor(0.98f, 0.94f, 0.82f), MakeName(TEXT("IvoryCap")));
	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Archive.X + I * 130.f;
		const float Y = Archive.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 70.f), FVector(0.8f, 0.35f, 1.1f), FLinearColor(0.86f, 0.8f, 0.68f), MakeName(TEXT("FormShelf")));
	}
}

void AHolypawWorldBuilder::BuildSandHymnDistricts()
{
	const FVector2D Dune = SandHymn + FVector2D(40.f, 30.f);
	const FVector2D Hum = SandHymn + FVector2D(-720.f, 220.f);
	const FVector2D Choir = SandHymn + FVector2D(70.f, 900.f);
	const FVector2D Stoop = SandHymn + FVector2D(760.f, -180.f);
	const FVector2D Shade = SandHymn + FVector2D(-90.f, -720.f);

	PlaceSign(Dune, NSLOCTEXT("Holypaw", "HymnDune", "Noon Dune  |  sand that hums first"));
	PlaceSign(Hum, NSLOCTEXT("Holypaw", "HymnHum", "Hum Walk  |  chocolate theology optional, recommended"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "HymnChoirS", "Dune Choir  |  off-key on purpose, dry"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "HymnStoop", "Shade Stoop  |  wishes that come back warm"));
	PlaceSign(Shade, NSLOCTEXT("Holypaw", "HymnShade", "Cape Path  |  two oceans arguing politely further south"));

	PlaceShrine(Dune + FVector2D(190.f, 50.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "DuneInn", "Dune Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "DuneChap", "Dune Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "NoonFont", "Noon Font"));
	PlaceShrine(Shade + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "DuneCrate", "Hum Crate"));
	PlacePickup(Hum + FVector2D(40.f, -40.f), TEXT("duneNote"), NSLOCTEXT("Holypaw", "NotePick", "dune note"));

	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Hum.X + I * 120.f;
		const float Y = Hum.Y + FMath::Sin(static_cast<float>(I)) * 40.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 50.f + I * 8.f), FVector(1.8f, 1.2f, 0.7f), FLinearColor(0.9f, 0.74f, 0.42f), MakeName(TEXT("DuneRipple")));
	}
	PlaceCube(FVector(Choir.X, Choir.Y, SampleHeight(Choir.X, Choir.Y) + 40.f), FVector(1.5f, 1.5f, 0.2f), FLinearColor(0.92f, 0.78f, 0.48f), MakeName(TEXT("HumPool")));
}

void AHolypawWorldBuilder::BuildCapePlushDistricts()
{
	const FVector2D Look = CapePlush + FVector2D(30.f, 40.f);
	const FVector2D Cliff = CapePlush + FVector2D(-680.f, 180.f);
	const FVector2D Choir = CapePlush + FVector2D(60.f, 860.f);
	const FVector2D Stoop = CapePlush + FVector2D(700.f, -160.f);
	const FVector2D Spray = CapePlush + FVector2D(-70.f, -680.f);

	PlaceSign(Look, NSLOCTEXT("Holypaw", "CapeLook", "Lookout  |  two oceans, one bear"));
	PlaceSign(Cliff, NSLOCTEXT("Holypaw", "CapeCliff", "Argument Cliff  |  water that debates politely"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "CapeChoir", "Spray Choir  |  hymns that salt themselves"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "CapeStoop", "Shell Stoop  |  wishes that come back tidal"));
	PlaceSign(Spray, NSLOCTEXT("Holypaw", "CapeSpray", "Savannah Path  |  bells on acacia if the grass is louder"));

	PlaceShrine(Look + FVector2D(180.f, 50.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "CapeInn", "Cape Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "SprayChap", "Spray Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "TideFontC", "Cape Font"));
	PlaceShrine(Spray + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "CapeCrate", "Shell Crate"));
	PlacePickup(Cliff + FVector2D(40.f, -40.f), TEXT("capeShell"), NSLOCTEXT("Holypaw", "ShellPick", "cape shell"));

	PlaceCube(FVector(Look.X, Look.Y, SampleHeight(Look.X, Look.Y) + 160.f), FVector(0.4f, 0.4f, 3.0f), FLinearColor(0.55f, 0.42f, 0.62f), MakeName(TEXT("LookPost")));
	PlaceCube(FVector(Look.X, Look.Y, SampleHeight(Look.X, Look.Y) + 320.f), FVector(1.1f, 0.6f, 0.35f), FLinearColor(0.72f, 0.62f, 0.82f), MakeName(TEXT("LookGlass")));
	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Cliff.X + I * 110.f;
		const float Y = Cliff.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 20.f + I * 18.f), FVector(1.6f, 2.4f, 0.35f), FLinearColor(0.48f, 0.42f, 0.52f), MakeName(TEXT("CliffStep")));
	}
}

void AHolypawWorldBuilder::BuildSavannahBellDistricts()
{
	const FVector2D GrassPatch = SavannahBell + FVector2D(40.f, 20.f);
	const FVector2D Acacia = SavannahBell + FVector2D(-740.f, 260.f);
	const FVector2D Choir = SavannahBell + FVector2D(80.f, 880.f);
	const FVector2D Stoop = SavannahBell + FVector2D(740.f, -170.f);
	const FVector2D Wind = SavannahBell + FVector2D(-80.f, -720.f);

	PlaceSign(GrassPatch, NSLOCTEXT("Holypaw", "SavGrass", "Gold Grass  |  savannah does not need polyester"));
	PlaceSign(Acacia, NSLOCTEXT("Holypaw", "SavAcacia", "Acacia Walk  |  bells hung without a permit"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "SavChoir", "Wind Choir  |  hymns the weather already knew"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "SavStoop", "Bell Stoop  |  wishes that come back ringing"));
	PlaceSign(Wind, NSLOCTEXT("Holypaw", "SavWind", "Wind Notch  |  Cape Plush if the oceans want a rematch"));

	PlaceShrine(GrassPatch + FVector2D(190.f, 50.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "GrassInn", "Grass Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "WindChap", "Wind Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "BellFontS", "Bell Font"));
	PlaceShrine(Wind + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "SavCrate", "Acacia Crate"));
	PlacePickup(Acacia + FVector2D(40.f, -40.f), TEXT("acaciaBell"), NSLOCTEXT("Holypaw", "BellPick", "acacia bell"));

	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Acacia.X + I * 130.f;
		const float Y = Acacia.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 120.f), FVector(0.18f, 0.18f, 2.4f), FLinearColor(0.42f, 0.32f, 0.2f), MakeName(TEXT("AcaciaTrunk")));
		PlaceCube(FVector(X, Y, Z + 250.f), FVector(1.4f, 1.4f, 0.45f), FLinearColor(0.86f, 0.7f, 0.32f), MakeName(TEXT("AcaciaCrown")));
		PlaceCube(FVector(X + 30.f, Y, Z + 180.f), FVector(0.18f, 0.18f, 0.28f), FLinearColor(0.92f, 0.78f, 0.35f), MakeName(TEXT("HungBell")));
	}
}
