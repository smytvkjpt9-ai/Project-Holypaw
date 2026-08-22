#include "HolypawWorldBuilder.h"

void AHolypawWorldBuilder::BuildCarnivalBahiaDistricts()
{
	const FVector2D Harbor = CarnivalBahia + FVector2D(80.f, 20.f);
	const FVector2D Drums = CarnivalBahia + FVector2D(-720.f, 280.f);
	const FVector2D Choir = CarnivalBahia + FVector2D(90.f, 920.f);
	const FVector2D Stoop = CarnivalBahia + FVector2D(760.f, -180.f);
	const FVector2D Parade = CarnivalBahia + FVector2D(-80.f, -760.f);

	PlaceSign(Harbor, NSLOCTEXT("Holypaw", "BahiaHarbor", "Confetti Harbor  |  drums stuffed with government"));
	PlaceSign(Drums, NSLOCTEXT("Holypaw", "BahiaDrums", "Drum Walk  |  stuffing in the drums, same parade"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "BahiaChoir", "Parade Choir  |  off-key on purpose, louder"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "BahiaStoop", "Float Stoop  |  wishes that come back in sequins"));
	PlaceSign(Parade, NSLOCTEXT("Holypaw", "BahiaParade", "Andes Path  |  wool dyed in weather if the drums rest"));

	PlaceShrine(Harbor + FVector2D(200.f, 60.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "ConfettiInn", "Confetti Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "ParadeChap", "Parade Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "SequinsFont", "Sequins Font"));
	PlaceShrine(Parade + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "BahiaCrate", "Float Crate"));
	PlacePickup(Drums + FVector2D(40.f, -40.f), TEXT("confettiDrum"), NSLOCTEXT("Holypaw", "DrumPick", "confetti drum"));
	PlaceStall(Harbor + FVector2D(140.f, -80.f));

	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Drums.X + I * 130.f;
		const float Y = Drums.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 70.f), FVector(0.7f, 0.7f, 1.1f), FLinearColor(0.95f, 0.42f, 0.38f), MakeName(TEXT("Drum")));
		PlaceCube(FVector(X, Y, Z + 140.f), FVector(0.35f, 0.35f, 0.22f), FLinearColor(0.95f, 0.78f, 0.32f), MakeName(TEXT("ConfettiHat")));
	}
	PlaceCube(FVector(Harbor.X, Harbor.Y, SampleHeight(Harbor.X, Harbor.Y) + 220.f), FVector(0.4f, 0.4f, 4.2f), FLinearColor(0.92f, 0.35f, 0.42f), MakeName(TEXT("CarnivalSpire")));
}

void AHolypawWorldBuilder::BuildAndesLoomDistricts()
{
	const FVector2D Gate = AndesLoom + FVector2D(30.f, 40.f);
	const FVector2D Wool = AndesLoom + FVector2D(-680.f, 200.f);
	const FVector2D Choir = AndesLoom + FVector2D(70.f, 840.f);
	const FVector2D Stoop = AndesLoom + FVector2D(700.f, -160.f);
	const FVector2D Spine = AndesLoom + FVector2D(-60.f, -680.f);

	PlaceSign(Gate, NSLOCTEXT("Holypaw", "AndesGate", "Spine Gate  |  the world wears a sweater now"));
	PlaceSign(Wool, NSLOCTEXT("Holypaw", "AndesWool", "Wool Walk  |  dyed in mountain weather, not beige"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "AndesChoir", "Height Choir  |  hymns that thin in the air"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "AndesStoop", "Weather Stoop  |  wishes that come back knitted"));
	PlaceSign(Spine, NSLOCTEXT("Holypaw", "AndesSpine", "Brochure Notch  |  don't mill this mountain"));

	PlaceShrine(Gate + FVector2D(180.f, 50.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "WoolInn", "Wool Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "HeightChap", "Height Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "WeatherFont", "Weather Font"));
	PlaceShrine(Spine + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "AndesCrate", "Wool Crate"));
	PlacePickup(Wool + FVector2D(40.f, -40.f), TEXT("mountainWool"), NSLOCTEXT("Holypaw", "WoolPick", "mountain wool"));

	PlaceCube(FVector(Gate.X, Gate.Y, SampleHeight(Gate.X, Gate.Y) + 180.f), FVector(0.35f, 2.0f, 3.4f), FLinearColor(0.72f, 0.48f, 0.32f), MakeName(TEXT("SpinePost")));
	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Wool.X + I * 140.f;
		const float Y = Wool.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 50.f), FVector(1.1f, 0.7f, 0.7f), FLinearColor(0.62f, 0.42f, 0.32f), MakeName(TEXT("WoolBale")));
		PlaceCube(FVector(X, Y, Z + 110.f), FVector(0.55f, 0.55f, 0.35f), FLinearColor(0.78f, 0.52f, 0.38f), MakeName(TEXT("DyedSkein")));
	}
}

void AHolypawWorldBuilder::BuildSilkDeltaDistricts()
{
	const FVector2D Mouth = SilkDelta + FVector2D(60.f, 20.f);
	const FVector2D Thread = SilkDelta + FVector2D(-760.f, 260.f);
	const FVector2D Choir = SilkDelta + FVector2D(100.f, 940.f);
	const FVector2D Stoop = SilkDelta + FVector2D(780.f, -170.f);
	const FVector2D Rumor = SilkDelta + FVector2D(-50.f, -780.f);

	PlaceSign(Mouth, NSLOCTEXT("Holypaw", "DeltaMouth", "Moon Mouth  |  thread enough to wrap a rumor"));
	PlaceSign(Thread, NSLOCTEXT("Holypaw", "DeltaThread", "Thread Walk  |  factory rumors, not mill logos"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "DeltaChoir", "River Choir  |  hymns that silt on purpose"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "DeltaStoop", "Silt Stoop  |  wishes that come back wrapped"));
	PlaceSign(Rumor, NSLOCTEXT("Holypaw", "DeltaRumor", "Cherry Path  |  the island claps in pink if the wrap holds"));

	PlaceShrine(Mouth + FVector2D(210.f, 60.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "DeltaInn", "Delta Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "SiltChap", "Silt Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "MoonFont", "Moon Font"));
	PlaceShrine(Rumor + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "DeltaCrate", "Rumor Crate"));
	PlacePickup(Thread + FVector2D(40.f, -40.f), TEXT("moonThread"), NSLOCTEXT("Holypaw", "MoonPick", "moon thread"));

	for (int32 I = 0; I < 6; ++I)
	{
		const float X = Thread.X + I * 110.f;
		const float Y = Thread.Y + (I % 2) * 50.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 40.f), FVector(0.9f, 0.18f, 0.18f), FLinearColor(0.86f, 0.32f, 0.38f), MakeName(TEXT("SilkLine")));
	}
	PlaceCube(FVector(Mouth.X, Mouth.Y, SampleHeight(Mouth.X, Mouth.Y) + 200.f), FVector(1.1f, 1.1f, 3.8f), FLinearColor(0.78f, 0.28f, 0.35f), MakeName(TEXT("DeltaSpire")));
}

void AHolypawWorldBuilder::BuildSpiceHarborDistricts()
{
	const FVector2D Dock = SpiceHarbor + FVector2D(50.f, 30.f);
	const FVector2D Sails = SpiceHarbor + FVector2D(-720.f, 220.f);
	const FVector2D Choir = SpiceHarbor + FVector2D(80.f, 900.f);
	const FVector2D Stoop = SpiceHarbor + FVector2D(740.f, -180.f);
	const FVector2D Pepper = SpiceHarbor + FVector2D(-70.f, -740.f);

	PlaceSign(Dock, NSLOCTEXT("Holypaw", "SpiceDock", "Pepper Dock  |  saffron sails, handmade stamps"));
	PlaceSign(Sails, NSLOCTEXT("Holypaw", "SpiceSails", "Sail Walk  |  identical pepper got a clerk fired"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "SpiceChoir", "Sneeze Choir  |  hymns that clear a sinuses politically"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "SpiceStoop", "Crate Stoop  |  wishes that come back hot"));
	PlaceSign(Pepper, NSLOCTEXT("Holypaw", "SpicePepper", "Delta Path  |  Silk Delta north if you like wrapping moons"));

	PlaceShrine(Dock + FVector2D(190.f, 50.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "PepperInn", "Pepper Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "SaffronChap", "Saffron Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "HotFont", "Hot Font"));
	PlaceShrine(Pepper + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "SpiceCrate", "Pepper Crate"));
	PlacePickup(Sails + FVector2D(40.f, -40.f), TEXT("saffronSail"), NSLOCTEXT("Holypaw", "SailPick", "saffron sail"));
	PlaceStall(Dock + FVector2D(120.f, -70.f));

	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Sails.X + I * 150.f;
		const float Y = Sails.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 90.f), FVector(0.12f, 1.6f, 1.8f), FLinearColor(0.90f, 0.55f, 0.22f), MakeName(TEXT("Sail")));
		PlaceCube(FVector(X, Y, Z + 20.f), FVector(1.4f, 0.6f, 0.22f), FLinearColor(0.42f, 0.32f, 0.22f), MakeName(TEXT("Hull")));
	}
}

void AHolypawWorldBuilder::BuildCoralChoirDistricts()
{
	const FVector2D Lagoon = CoralChoir + FVector2D(40.f, 20.f);
	const FVector2D Reef = CoralChoir + FVector2D(-700.f, 200.f);
	const FVector2D Choir = CoralChoir + FVector2D(70.f, 860.f);
	const FVector2D Stoop = CoralChoir + FVector2D(720.f, -160.f);
	const FVector2D Lyric = CoralChoir + FVector2D(-60.f, -700.f);

	PlaceSign(Lagoon, NSLOCTEXT("Holypaw", "CoralLagoon", "Lagoon  |  sings off-key on purpose"));
	PlaceSign(Reef, NSLOCTEXT("Holypaw", "CoralReef", "Reef Walk  |  lyrics that are just the word bear"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "CoralChoirD", "Choir Pool  |  chart-topping, soggy"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "CoralStoop", "Anemone Stoop  |  wishes that come back tidal"));
	PlaceSign(Lyric, NSLOCTEXT("Holypaw", "CoralLyric", "Cape Path  |  two oceans argue, we harmonize"));

	PlaceShrine(Lagoon + FVector2D(180.f, 50.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "ReefInn", "Reef Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "LagoonChap", "Lagoon Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "AnemoneFont", "Anemone Font"));
	PlaceShrine(Lyric + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "CoralCrate", "Lyric Crate"));
	PlacePickup(Reef + FVector2D(40.f, -40.f), TEXT("reefLyric"), NSLOCTEXT("Holypaw", "LyricPick", "reef lyric"));

	PlaceCube(FVector(Lagoon.X, Lagoon.Y, SampleHeight(Lagoon.X, Lagoon.Y) + 22.f), FVector(2.8f, 2.4f, 0.16f), FLinearColor(0.28f, 0.72f, 0.78f), MakeName(TEXT("LagoonSheet")));
	for (int32 I = 0; I < 5; ++I)
	{
		const float X = Reef.X + I * 120.f;
		const float Y = Reef.Y + (I % 2) * 40.f;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 50.f), FVector(0.45f, 0.45f, 0.8f), FLinearColor(0.95f, 0.48f, 0.55f), MakeName(TEXT("CoralHead")));
	}
}

void AHolypawWorldBuilder::BuildAuroraBoroughDistricts()
{
	const FVector2D Square = AuroraBorough + FVector2D(40.f, 20.f);
	const FVector2D Lights = AuroraBorough + FVector2D(-720.f, 240.f);
	const FVector2D Choir = AuroraBorough + FVector2D(80.f, 880.f);
	const FVector2D Stoop = AuroraBorough + FVector2D(740.f, -170.f);
	const FVector2D Mitten = AuroraBorough + FVector2D(-70.f, -720.f);

	PlaceSign(Square, NSLOCTEXT("Holypaw", "AuroraSq", "Mitten Square  |  civic policy you can wear"));
	PlaceSign(Lights, NSLOCTEXT("Holypaw", "AuroraLights", "Copy Walk  |  lights copy you if you glow"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "AuroraChoirD", "Light Choir  |  hymns that shy, then copy"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "AuroraStoopD", "Glow Stoop  |  wishes that come back in color"));
	PlaceSign(Mitten, NSLOCTEXT("Holypaw", "AuroraMitten", "Parish Path  |  Tundra keeps the cold honest"));

	PlaceShrine(Square + FVector2D(190.f, 50.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "MittenInn", "Mitten Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "LightChap", "Light Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "GlowFont", "Glow Font"));
	PlaceShrine(Mitten + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "AuroraCrate", "Mitten Crate"));
	PlacePickup(Lights + FVector2D(40.f, -40.f), TEXT("auroraMitten"), NSLOCTEXT("Holypaw", "MittenPick", "aurora mitten"));

	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Lights.X + I * 150.f;
		const float Y = Lights.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 200.f), FVector(0.12f, 1.8f, 0.35f), FLinearColor(0.45f, 0.72f, 0.95f), MakeName(TEXT("AuroraRibbon")));
		PlaceCube(FVector(X, Y, Z + 40.f), FVector(0.5f, 0.35f, 0.45f), FLinearColor(0.85f, 0.55f, 0.72f), MakeName(TEXT("MittenPair")));
	}
}

void AHolypawWorldBuilder::BuildTundraParishDistricts()
{
	const FVector2D Gate = TundraParish + FVector2D(20.f, 30.f);
	const FVector2D Quiet = TundraParish + FVector2D(-640.f, 180.f);
	const FVector2D Choir = TundraParish + FVector2D(50.f, 800.f);
	const FVector2D Stoop = TundraParish + FVector2D(660.f, -150.f);
	const FVector2D Sermon = TundraParish + FVector2D(-50.f, -640.f);

	PlaceSign(Gate, NSLOCTEXT("Holypaw", "TundraGate", "Parish Gate  |  silence, then a round loud"));
	PlaceSign(Quiet, NSLOCTEXT("Holypaw", "TundraQuiet", "Quiet Walk  |  sermons whispered into snow"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "TundraChoir", "Drift Choir  |  hymns that freeze mid-word"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "TundraStoop", "Whisper Stoop  |  wishes that come back honest"));
	PlaceSign(Sermon, NSLOCTEXT("Holypaw", "TundraSermon", "Wolf Notch  |  Drift Wolves. Hymn louder"));

	PlaceShrine(Gate + FVector2D(170.f, 40.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "ParishInn", "Parish Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "DriftChap", "Drift Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "WhisperFont", "Whisper Font"));
	PlaceShrine(Sermon + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "TundraCrate", "Sermon Crate"));
	PlacePickup(Quiet + FVector2D(40.f, -40.f), TEXT("snowSermon"), NSLOCTEXT("Holypaw", "SermonPick", "snow sermon"));

	PlaceCube(FVector(Gate.X - 60.f, Gate.Y, SampleHeight(Gate.X, Gate.Y) + 130.f), FVector(0.28f, 1.8f, 2.6f), FLinearColor(0.78f, 0.86f, 0.95f), MakeName(TEXT("ParishPost")));
	PlaceCube(FVector(Gate.X + 60.f, Gate.Y, SampleHeight(Gate.X, Gate.Y) + 130.f), FVector(0.28f, 1.8f, 2.6f), FLinearColor(0.78f, 0.86f, 0.95f), MakeName(TEXT("ParishPostB")));
	for (int32 I = 0; I < 4; ++I)
	{
		const float X = Quiet.X + I * 120.f;
		const float Y = Quiet.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 18.f), FVector(0.9f, 0.9f, 0.14f), FLinearColor(0.9f, 0.93f, 0.98f), MakeName(TEXT("SnowPage")));
	}
}

void AHolypawWorldBuilder::BuildFeltIceCampDistricts()
{
	const FVector2D Camp = FeltIceCamp + FVector2D(20.f, 20.f);
	const FVector2D Scarf = FeltIceCamp + FVector2D(-560.f, 160.f);
	const FVector2D Choir = FeltIceCamp + FVector2D(40.f, 720.f);
	const FVector2D Stoop = FeltIceCamp + FVector2D(580.f, -140.f);
	const FVector2D Echo = FeltIceCamp + FVector2D(-40.f, -560.f);

	PlaceSign(Camp, NSLOCTEXT("Holypaw", "FeltCamp", "Last Camp  |  end of the sewn world"));
	PlaceSign(Scarf, NSLOCTEXT("Holypaw", "FeltScarf", "Spare Walk  |  last scarf, saved for a mayor"));
	PlaceSign(Choir, NSLOCTEXT("Holypaw", "FeltChoir", "Echo Choir  |  quiet so the coup has acoustics"));
	PlaceSign(Stoop, NSLOCTEXT("Holypaw", "FeltStoop", "Penguin Stoop  |  tiny scarves, tiny policy"));
	PlaceSign(Echo, NSLOCTEXT("Holypaw", "FeltEcho", "Porch Path  |  home is a lantern away"));

	PlaceShrine(Camp + FVector2D(160.f, 40.f), EHolypawShrineKind::Inn, NSLOCTEXT("Holypaw", "IceInn", "Ice Inn"));
	PlaceShrine(Choir + FVector2D(40.f, 20.f), EHolypawShrineKind::Chapel, NSLOCTEXT("Holypaw", "EchoChap", "Echo Chapel"));
	PlaceShrine(Stoop + FVector2D(-30.f, 40.f), EHolypawShrineKind::Wish, NSLOCTEXT("Holypaw", "PenguinFont", "Penguin Font"));
	PlaceShrine(Echo + FVector2D(50.f, 20.f), EHolypawShrineKind::Crate, NSLOCTEXT("Holypaw", "FeltCrate", "Spare Crate"));
	PlacePickup(Scarf + FVector2D(40.f, -40.f), TEXT("spareScarf"), NSLOCTEXT("Holypaw", "ScarfPickF", "spare scarf"));

	PlaceCube(FVector(Camp.X, Camp.Y, SampleHeight(Camp.X, Camp.Y) + 70.f), FVector(2.0f, 1.4f, 1.1f), FLinearColor(0.82f, 0.90f, 0.98f), MakeName(TEXT("IceTent")));
	for (int32 I = 0; I < 3; ++I)
	{
		const float X = Stoop.X + I * 90.f;
		const float Y = Stoop.Y;
		const float Z = SampleHeight(X, Y);
		PlaceCube(FVector(X, Y, Z + 35.f), FVector(0.35f, 0.28f, 0.5f), FLinearColor(0.2f, 0.22f, 0.28f), MakeName(TEXT("Penguin")));
		PlaceCube(FVector(X, Y, Z + 70.f), FVector(0.4f, 0.12f, 0.18f), FLinearColor(0.85f, 0.45f, 0.55f), MakeName(TEXT("TinyScarf")));
	}
}
