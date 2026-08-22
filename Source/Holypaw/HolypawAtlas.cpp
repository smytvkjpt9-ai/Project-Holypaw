#include "HolypawCatalog.h"

namespace HolypawCatalog
{
	const TArray<FHolypawCity>& GetCities()
	{
		static const TArray<FHolypawCity> All = []()
		{
			TArray<FHolypawCity> Out;
			auto Add = [&](EHolypawZone Zone, const TCHAR* Name, FVector2D Pos, FLinearColor Accent,
				int32 Cols, int32 Rows, bool bSpire, float Radius, float FlattenZ,
				const TCHAR* Continent, const TCHAR* Flavor)
			{
				FHolypawCity C;
				C.Zone = Zone;
				C.DisplayName = FText::FromString(Name);
				C.Pos = Pos;
				C.Accent = Accent;
				C.Cols = Cols;
				C.Rows = Rows;
				C.bTallSpire = bSpire;
				C.Radius = Radius;
				C.FlattenZ = FlattenZ;
				C.Continent = FText::FromString(Continent);
				C.Flavor = FText::FromString(Flavor);
				Out.Add(C);
			};

			// Home Sewn (North America analog). Opening still cottage -> Ribbon City.
			Add(EHolypawZone::RibbonCity, TEXT("Ribbon City"), FVector2D(26000.f, 400.f), FLinearColor(0.82f, 0.42f, 0.55f), 5, 4, true, 6500.f, 55.f,
				TEXT("Home Sewn"), TEXT("east-coast metropolis  |  Poly Mill court sits here"));
			Add(EHolypawZone::Tidewell, TEXT("Tidewell"), FVector2D(40000.f, 7000.f), FLinearColor(0.40f, 0.62f, 0.78f), 3, 3, false, 4200.f, 28.f,
				TEXT("Home Sewn"), TEXT("atlantic harbor  |  ferry rumors toward Cherry Loom"));
			Add(EHolypawZone::Hearthfold, TEXT("Hearthfold"), FVector2D(2000.f, 24000.f), FLinearColor(0.78f, 0.62f, 0.28f), 3, 2, false, 3800.f, 70.f,
				TEXT("Home Sewn"), TEXT("breadbasket plains  |  grain and hymns"));
			Add(EHolypawZone::Emberfen, TEXT("Emberfen"), FVector2D(6000.f, -23000.f), FLinearColor(0.62f, 0.32f, 0.28f), 3, 2, false, 3600.f, 22.f,
				TEXT("Home Sewn"), TEXT("gulf peat  |  frogs the color of cooling jam"));
			Add(EHolypawZone::Snowveil, TEXT("Snowveil"), FVector2D(-2000.f, 20000.f), FLinearColor(0.75f, 0.82f, 0.95f), 2, 2, false, 3200.f, 140.f,
				TEXT("Home Sewn"), TEXT("ridge town  |  gate to Velvet Peak"));
			Add(EHolypawZone::LanternAngeles, TEXT("Lantern Angeles"), FVector2D(-82000.f, -22000.f), FLinearColor(0.95f, 0.72f, 0.28f), 4, 3, true, 5200.f, 40.f,
				TEXT("Home Sewn"), TEXT("pacific studios  |  gold light, cheap posters"));
			Add(EHolypawZone::Mossgate, TEXT("Mossgate"), FVector2D(-86000.f, 18000.f), FLinearColor(0.45f, 0.72f, 0.62f), 3, 3, false, 4000.f, 80.f,
				TEXT("Home Sewn"), TEXT("rainy northwest  |  fog that tastes like tea"));
			Add(EHolypawZone::Quiltland, TEXT("Quiltland"), FVector2D(-62000.f, 38000.f), FLinearColor(0.38f, 0.55f, 0.42f), 3, 2, false, 3800.f, 90.f,
				TEXT("Home Sewn"), TEXT("north woods  |  patchwork pines"));
			Add(EHolypawZone::DustMesa, TEXT("Dust Mesa"), FVector2D(-48000.f, -52000.f), FLinearColor(0.86f, 0.62f, 0.38f), 2, 2, false, 3600.f, 48.f,
				TEXT("Home Sewn"), TEXT("button desert  |  canyons full of lost beads"));

			// South Sewn (South America analog).
			Add(EHolypawZone::CarnivalBahia, TEXT("Carnival Bahia"), FVector2D(-8000.f, -86000.f), FLinearColor(0.95f, 0.45f, 0.38f), 3, 3, true, 4200.f, 22.f,
				TEXT("South Sewn"), TEXT("carnival harbor  |  drums stuffed with confetti"));
			Add(EHolypawZone::AndesLoom, TEXT("Andes Loom"), FVector2D(-28000.f, -70000.f), FLinearColor(0.72f, 0.48f, 0.32f), 2, 2, false, 3400.f, 180.f,
				TEXT("South Sewn"), TEXT("high spine  |  wool dyed in mountain weather"));

			// Old World (Europe analog).
			Add(EHolypawZone::Clockhaven, TEXT("Clockhaven"), FVector2D(50000.f, 34000.f), FLinearColor(0.62f, 0.68f, 0.78f), 4, 3, true, 4400.f, 55.f,
				TEXT("Old World"), TEXT("fog capital  |  teatime bells, wet cobbles"));
			Add(EHolypawZone::VelvetSeine, TEXT("Velvet Seine"), FVector2D(56000.f, 22000.f), FLinearColor(0.78f, 0.52f, 0.72f), 3, 3, true, 4000.f, 48.f,
				TEXT("Old World"), TEXT("ribbon river  |  bakeries that bless strangers"));
			Add(EHolypawZone::MarbleForum, TEXT("Marble Forum"), FVector2D(62000.f, 8000.f), FLinearColor(0.88f, 0.82f, 0.70f), 3, 3, true, 3800.f, 60.f,
				TEXT("Old World"), TEXT("stone plaza  |  columns wearing tiny hats"));

			// Sand Belt / Cape (Africa analog).
			Add(EHolypawZone::PalmaDusk, TEXT("Palma Dusk"), FVector2D(12000.f, -72000.f), FLinearColor(0.28f, 0.72f, 0.55f), 3, 3, false, 4200.f, 18.f,
				TEXT("Sand Belt"), TEXT("tropic dusk  |  palms that clap at hymns"));
			Add(EHolypawZone::IvorySpire, TEXT("Ivory Spire"), FVector2D(58000.f, -40000.f), FLinearColor(0.92f, 0.88f, 0.78f), 3, 3, true, 4000.f, 60.f,
				TEXT("Sand Belt"), TEXT("north-shore spire  |  old stone, new stuffing"));
			Add(EHolypawZone::SandHymn, TEXT("Sand Hymn"), FVector2D(32000.f, -58000.f), FLinearColor(0.90f, 0.74f, 0.42f), 3, 2, false, 3800.f, 32.f,
				TEXT("Sand Belt"), TEXT("desert choir  |  dunes that hum at noon"));
			Add(EHolypawZone::CapePlush, TEXT("Cape Plush"), FVector2D(18000.f, -98000.f), FLinearColor(0.55f, 0.42f, 0.62f), 2, 2, false, 3400.f, 24.f,
				TEXT("Sand Belt"), TEXT("southern cape  |  two oceans arguing politely"));
			Add(EHolypawZone::SavannahBell, TEXT("Savannah Bell"), FVector2D(44000.f, -80000.f), FLinearColor(0.86f, 0.70f, 0.32f), 3, 2, false, 3600.f, 40.f,
				TEXT("Sand Belt"), TEXT("grass hymns  |  bells hung on acacia"));

			// Silk East (Asia analog) and Cherry island.
			Add(EHolypawZone::SilkDelta, TEXT("Silk Delta"), FVector2D(80000.f, 22000.f), FLinearColor(0.86f, 0.32f, 0.38f), 4, 3, true, 4600.f, 42.f,
				TEXT("Silk East"), TEXT("great river  |  thread enough to wrap a moon"));
			Add(EHolypawZone::SpiceHarbor, TEXT("Spice Harbor"), FVector2D(74000.f, -14000.f), FLinearColor(0.90f, 0.55f, 0.22f), 3, 3, false, 4000.f, 30.f,
				TEXT("Silk East"), TEXT("pepper docks  |  saffron sails"));
			Add(EHolypawZone::CherryLoom, TEXT("Cherry Loom"), FVector2D(104000.f, 10000.f), FLinearColor(0.92f, 0.45f, 0.58f), 4, 3, true, 4800.f, 50.f,
				TEXT("Cherry Isle"), TEXT("island of looms  |  petals sewn to rain"));

			// Coral South (Oceania analog).
			Add(EHolypawZone::CoralChoir, TEXT("Coral Choir"), FVector2D(110000.f, -58000.f), FLinearColor(0.28f, 0.72f, 0.78f), 3, 2, false, 3800.f, 20.f,
				TEXT("Coral South"), TEXT("reef city  |  the lagoon sings off-key on purpose"));

			// Ice caps.
			Add(EHolypawZone::AuroraBorough, TEXT("Aurora Borough"), FVector2D(8000.f, 82000.f), FLinearColor(0.45f, 0.72f, 0.95f), 3, 2, false, 3600.f, 70.f,
				TEXT("Felt North"), TEXT("northern lights  |  mittens as civic policy"));
			Add(EHolypawZone::TundraParish, TEXT("Tundra Parish"), FVector2D(-18000.f, 98000.f), FLinearColor(0.78f, 0.86f, 0.95f), 2, 2, false, 3000.f, 40.f,
				TEXT("Felt North"), TEXT("far parish  |  sermons whispered into snow"));
			Add(EHolypawZone::FeltIceCamp, TEXT("Felt Ice Camp"), FVector2D(2000.f, -124000.f), FLinearColor(0.82f, 0.90f, 0.98f), 2, 1, false, 2800.f, 16.f,
				TEXT("Felt South"), TEXT("end of the sewn world  |  penguins in tiny scarves"));
			return Out;
		}();
		return All;
	}

	FHolypawCity GetCity(EHolypawZone Zone)
	{
		for (const FHolypawCity& C : GetCities())
		{
			if (C.Zone == Zone)
			{
				return C;
			}
		}
		return GetCities()[0];
	}

	bool IsCityZone(EHolypawZone Zone)
	{
		for (const FHolypawCity& C : GetCities())
		{
			if (C.Zone == Zone)
			{
				return true;
			}
		}
		return false;
	}

	const TArray<FHolypawRoadLink>& GetRoads()
	{
		static const TArray<FHolypawRoadLink> All = []()
		{
			TArray<FHolypawRoadLink> Out;
			auto Link = [&](EHolypawZone A, EHolypawZone B, int32 Steps, int32 Salt)
			{
				FHolypawRoadLink R;
				R.A = A;
				R.B = B;
				R.Steps = Steps;
				R.Salt = Salt;
				Out.Add(R);
			};

			Link(EHolypawZone::RibbonCity, EHolypawZone::Tidewell, 18, 1);
			Link(EHolypawZone::RibbonCity, EHolypawZone::Hearthfold, 22, 2);
			Link(EHolypawZone::RibbonCity, EHolypawZone::Emberfen, 22, 3);
			Link(EHolypawZone::Hearthfold, EHolypawZone::Snowveil, 12, 4);
			Link(EHolypawZone::LanternAngeles, EHolypawZone::Mossgate, 16, 6);
			Link(EHolypawZone::Mossgate, EHolypawZone::Quiltland, 14, 7);
			Link(EHolypawZone::LanternAngeles, EHolypawZone::DustMesa, 14, 9);
			Link(EHolypawZone::DustMesa, EHolypawZone::AndesLoom, 16, 16);
			Link(EHolypawZone::AndesLoom, EHolypawZone::CarnivalBahia, 12, 17);
			Link(EHolypawZone::Emberfen, EHolypawZone::PalmaDusk, 16, 10);
			Link(EHolypawZone::PalmaDusk, EHolypawZone::SandHymn, 12, 11);
			Link(EHolypawZone::SandHymn, EHolypawZone::IvorySpire, 12, 12);
			Link(EHolypawZone::SandHymn, EHolypawZone::CapePlush, 14, 13);
			Link(EHolypawZone::CapePlush, EHolypawZone::SavannahBell, 12, 18);
			Link(EHolypawZone::CarnivalBahia, EHolypawZone::CapePlush, 14, 19);
			Link(EHolypawZone::Hearthfold, EHolypawZone::AuroraBorough, 16, 14);
			Link(EHolypawZone::AuroraBorough, EHolypawZone::TundraParish, 10, 15);
			Link(EHolypawZone::Clockhaven, EHolypawZone::VelvetSeine, 10, 20);
			Link(EHolypawZone::VelvetSeine, EHolypawZone::MarbleForum, 12, 21);
			Link(EHolypawZone::MarbleForum, EHolypawZone::IvorySpire, 14, 22);
			Link(EHolypawZone::IvorySpire, EHolypawZone::SpiceHarbor, 14, 23);
			Link(EHolypawZone::SpiceHarbor, EHolypawZone::SilkDelta, 14, 24);
			Link(EHolypawZone::SilkDelta, EHolypawZone::CherryLoom, 12, 25);
			return Out;
		}();
		return All;
	}

	const TArray<FHolypawLandmass>& GetLandmasses()
	{
		static const TArray<FHolypawLandmass> All = []()
		{
			TArray<FHolypawLandmass> Out;
			auto Add = [&](const TCHAR* Name, FVector2D Center, float RadiusX, float RadiusY, float HeightBias)
			{
				FHolypawLandmass L;
				L.Name = Name;
				L.Center = Center;
				L.RadiusX = RadiusX;
				L.RadiusY = RadiusY;
				L.HeightBias = HeightBias;
				Out.Add(L);
			};
			Add(TEXT("Home Sewn"), FVector2D(-22000.f, 2000.f), 68000.f, 42000.f, 88.f);
			Add(TEXT("Canyon West"), FVector2D(-50000.f, -48000.f), 20000.f, 18000.f, 55.f);
			Add(TEXT("South Sewn"), FVector2D(-16000.f, -80000.f), 30000.f, 24000.f, 74.f);
			Add(TEXT("Old World"), FVector2D(58000.f, 24000.f), 15000.f, 21000.f, 92.f);
			Add(TEXT("Sand Belt"), FVector2D(36000.f, -56000.f), 30000.f, 26000.f, 48.f);
			Add(TEXT("Cape Land"), FVector2D(22000.f, -96000.f), 20000.f, 16000.f, 52.f);
			Add(TEXT("Silk East"), FVector2D(80000.f, 6000.f), 16000.f, 28000.f, 86.f);
			Add(TEXT("Cherry Isle"), FVector2D(104000.f, 10000.f), 9000.f, 8000.f, 70.f);
			Add(TEXT("Coral South"), FVector2D(110000.f, -58000.f), 14000.f, 12000.f, 58.f);
			Add(TEXT("Felt North"), FVector2D(-2000.f, 90000.f), 42000.f, 24000.f, 36.f);
			Add(TEXT("Felt South"), FVector2D(2000.f, -126000.f), 38000.f, 14000.f, 18.f);
			return Out;
		}();
		return All;
	}

	float LandHeightBias(const FVector2D& P)
	{
		float Best = -140.f;
		for (const FHolypawLandmass& L : GetLandmasses())
		{
			const float NX = (P.X - L.Center.X) / FMath::Max(1.f, L.RadiusX);
			const float NY = (P.Y - L.Center.Y) / FMath::Max(1.f, L.RadiusY);
			const float E = NX * NX + NY * NY;
			if (E < 1.35f)
			{
				const float T = FMath::Clamp(1.f - E * 0.72f, 0.f, 1.f);
				Best = FMath::Max(Best, L.HeightBias * T);
			}
		}
		return Best;
	}

	EHolypawZone ResolveWilderness(const FVector2D& P)
	{
		if (P.Y > 70000.f || P.Y < -115000.f)
		{
			return EHolypawZone::Ice;
		}
		if (LandHeightBias(P) < 8.f)
		{
			return EHolypawZone::Ocean;
		}
		if (FVector2D::Distance(P, GetCity(EHolypawZone::DustMesa).Pos) < 22000.f)
		{
			return EHolypawZone::Desert;
		}
		if (FVector2D::Distance(P, GetCity(EHolypawZone::SandHymn).Pos) < 20000.f)
		{
			return EHolypawZone::Desert;
		}
		if (FVector2D::Distance(P, GetCity(EHolypawZone::PalmaDusk).Pos) < 18000.f
			|| FVector2D::Distance(P, GetCity(EHolypawZone::CarnivalBahia).Pos) < 14000.f
			|| FVector2D::Distance(P, GetCity(EHolypawZone::SavannahBell).Pos) < 12000.f)
		{
			return EHolypawZone::Jungle;
		}
		if (P.X > 34000.f && P.X < 46000.f && P.Y > -12000.f && P.Y < 16000.f)
		{
			return EHolypawZone::Coast;
		}
		if (P.Y < -14000.f && P.Y > -40000.f && P.X > -8000.f && P.X < 20000.f)
		{
			return EHolypawZone::Mire;
		}
		if (P.Y > 10000.f && P.X > -8000.f && P.X < 16000.f && P.Y < 32000.f)
		{
			return EHolypawZone::Homestead;
		}
		if (P.X < -20000.f && P.X > -75000.f && P.Y > -28000.f && P.Y < 28000.f)
		{
			return EHolypawZone::ForestCottage;
		}
		return EHolypawZone::NurseryHills;
	}

	FLinearColor ZoneTerrainColor(EHolypawZone Zone)
	{
		switch (Zone)
		{
		case EHolypawZone::ForestCottage: return FLinearColor(0.32f, 0.52f, 0.34f);
		case EHolypawZone::NurseryHills: return FLinearColor(0.55f, 0.72f, 0.40f);
		case EHolypawZone::RibbonCity: return FLinearColor(0.62f, 0.58f, 0.52f);
		case EHolypawZone::Tidewell: return FLinearColor(0.55f, 0.62f, 0.68f);
		case EHolypawZone::Hearthfold: return FLinearColor(0.62f, 0.70f, 0.38f);
		case EHolypawZone::Emberfen: return FLinearColor(0.52f, 0.34f, 0.30f);
		case EHolypawZone::Snowveil: return FLinearColor(0.82f, 0.86f, 0.92f);
		case EHolypawZone::Homestead: return FLinearColor(0.58f, 0.68f, 0.36f);
		case EHolypawZone::Coast: return FLinearColor(0.78f, 0.72f, 0.52f);
		case EHolypawZone::Mire: return FLinearColor(0.48f, 0.32f, 0.28f);
		case EHolypawZone::Highlands: return FLinearColor(0.55f, 0.52f, 0.50f);
		case EHolypawZone::Snow: return FLinearColor(0.90f, 0.93f, 0.97f);
		case EHolypawZone::LanternAngeles: return FLinearColor(0.78f, 0.62f, 0.38f);
		case EHolypawZone::Mossgate: return FLinearColor(0.42f, 0.58f, 0.52f);
		case EHolypawZone::Quiltland: return FLinearColor(0.30f, 0.48f, 0.34f);
		case EHolypawZone::DustMesa: return FLinearColor(0.82f, 0.64f, 0.40f);
		case EHolypawZone::PalmaDusk: return FLinearColor(0.28f, 0.62f, 0.42f);
		case EHolypawZone::IvorySpire: return FLinearColor(0.78f, 0.74f, 0.62f);
		case EHolypawZone::SandHymn: return FLinearColor(0.86f, 0.70f, 0.42f);
		case EHolypawZone::CapePlush: return FLinearColor(0.42f, 0.38f, 0.48f);
		case EHolypawZone::CherryLoom: return FLinearColor(0.82f, 0.52f, 0.58f);
		case EHolypawZone::AuroraBorough: return FLinearColor(0.55f, 0.72f, 0.88f);
		case EHolypawZone::TundraParish: return FLinearColor(0.80f, 0.86f, 0.92f);
		case EHolypawZone::Desert: return FLinearColor(0.84f, 0.68f, 0.42f);
		case EHolypawZone::Jungle: return FLinearColor(0.22f, 0.48f, 0.28f);
		case EHolypawZone::Ocean: return FLinearColor(0.28f, 0.48f, 0.68f);
		case EHolypawZone::Ice: return FLinearColor(0.88f, 0.92f, 0.97f);
		case EHolypawZone::CarnivalBahia: return FLinearColor(0.78f, 0.48f, 0.38f);
		case EHolypawZone::AndesLoom: return FLinearColor(0.58f, 0.48f, 0.36f);
		case EHolypawZone::Clockhaven: return FLinearColor(0.52f, 0.58f, 0.62f);
		case EHolypawZone::VelvetSeine: return FLinearColor(0.68f, 0.52f, 0.62f);
		case EHolypawZone::MarbleForum: return FLinearColor(0.74f, 0.70f, 0.58f);
		case EHolypawZone::SavannahBell: return FLinearColor(0.72f, 0.64f, 0.32f);
		case EHolypawZone::SilkDelta: return FLinearColor(0.70f, 0.38f, 0.36f);
		case EHolypawZone::SpiceHarbor: return FLinearColor(0.78f, 0.52f, 0.28f);
		case EHolypawZone::CoralChoir: return FLinearColor(0.32f, 0.62f, 0.64f);
		case EHolypawZone::FeltIceCamp: return FLinearColor(0.86f, 0.90f, 0.95f);
		default: return FLinearColor(0.45f, 0.62f, 0.38f);
		}
	}
}
