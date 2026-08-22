#include "Audio/HolypawAudioInternal.h"

namespace HolypawCues
{
	const TArray<FHolypawCueDef>& All()
	{
		static const TArray<FHolypawCueDef> Table = []()
		{
			TArray<FHolypawCueDef> Out;
			auto Add = [&](const TCHAR* Id, EHolypawBus Bus, float Vol, float Sec, int32 Cents)
			{
				FHolypawCueDef D;
				D.Id = Id;
				D.Bus = Bus;
				D.Volume = Vol;
				D.Seconds = Sec;
				D.VaryCents = Cents;
				D.bTEMP = true;
				Out.Add(D);
			};

			Add(TEXT("Title"), EHolypawBus::Ui, 0.72f, 1.15f, 8);
			Add(TEXT("Save"), EHolypawBus::Ui, 0.42f, 0.22f, 18);
			Add(TEXT("Shop"), EHolypawBus::Ui, 0.48f, 0.18f, 30);
			Add(TEXT("Talk"), EHolypawBus::Ui, 0.38f, 0.12f, 55);
			Add(TEXT("UiOpen"), EHolypawBus::Ui, 0.40f, 0.16f, 20);
			Add(TEXT("UiClose"), EHolypawBus::Ui, 0.36f, 0.14f, 20);
			Add(TEXT("UiMove"), EHolypawBus::Ui, 0.28f, 0.08f, 40);
			Add(TEXT("UiConfirm"), EHolypawBus::Ui, 0.50f, 0.18f, 12);
			Add(TEXT("UiDeny"), EHolypawBus::Ui, 0.40f, 0.16f, 10);

			Add(TEXT("Hug"), EHolypawBus::Sfx, 0.62f, 0.28f, 35);
			Add(TEXT("Convert"), EHolypawBus::Sfx, 0.74f, 1.05f, 12);
			Add(TEXT("Miracle"), EHolypawBus::Sfx, 0.80f, 1.80f, 6);
			Add(TEXT("Kneel"), EHolypawBus::Sfx, 0.58f, 0.70f, 10);
			Add(TEXT("Shrine"), EHolypawBus::Sfx, 0.60f, 0.85f, 8);
			Add(TEXT("Fountain"), EHolypawBus::Sfx, 0.55f, 0.90f, 15);
			Add(TEXT("Inn"), EHolypawBus::Sfx, 0.48f, 0.55f, 10);
			Add(TEXT("Chapel"), EHolypawBus::Sfx, 0.66f, 1.40f, 6);

			Add(TEXT("Door"), EHolypawBus::Foley, 0.50f, 0.42f, 25);
			Add(TEXT("Travel"), EHolypawBus::Sfx, 0.58f, 0.85f, 10);
			Add(TEXT("Jump"), EHolypawBus::Foley, 0.40f, 0.16f, 40);
			Add(TEXT("Land"), EHolypawBus::Foley, 0.46f, 0.18f, 30);
			Add(TEXT("Pickup"), EHolypawBus::Sfx, 0.52f, 0.28f, 35);
			Add(TEXT("Lantern"), EHolypawBus::Sfx, 0.50f, 0.45f, 18);
			Add(TEXT("FootstepSoft"), EHolypawBus::Foley, 0.28f, 0.12f, 80);
			Add(TEXT("FootstepWood"), EHolypawBus::Foley, 0.32f, 0.11f, 70);
			Add(TEXT("FootstepStone"), EHolypawBus::Foley, 0.34f, 0.10f, 55);
			Add(TEXT("FootstepSnow"), EHolypawBus::Foley, 0.26f, 0.13f, 90);
			Add(TEXT("FootstepSand"), EHolypawBus::Foley, 0.27f, 0.12f, 85);
			Add(TEXT("FootstepWater"), EHolypawBus::Foley, 0.30f, 0.14f, 70);

			Add(TEXT("BattleStart"), EHolypawBus::Combat, 0.78f, 1.10f, 6);
			Add(TEXT("BattleHit"), EHolypawBus::Combat, 0.70f, 0.22f, 45);
			Add(TEXT("BattleCrit"), EHolypawBus::Combat, 0.82f, 0.38f, 18);
			Add(TEXT("BattleStagger"), EHolypawBus::Combat, 0.76f, 0.42f, 12);
			Add(TEXT("BattleWin"), EHolypawBus::Combat, 0.80f, 1.35f, 8);
			Add(TEXT("BattleLose"), EHolypawBus::Combat, 0.72f, 1.20f, 6);
			Add(TEXT("BossPhase"), EHolypawBus::Combat, 0.84f, 1.50f, 4);
			Add(TEXT("Hurt"), EHolypawBus::Combat, 0.68f, 0.28f, 30);
			Add(TEXT("Slap"), EHolypawBus::Combat, 0.66f, 0.20f, 50);
			Add(TEXT("Beam"), EHolypawBus::Combat, 0.72f, 0.55f, 12);
			Add(TEXT("Party"), EHolypawBus::Combat, 0.70f, 0.40f, 25);
			Add(TEXT("Guard"), EHolypawBus::Combat, 0.50f, 0.32f, 20);
			Add(TEXT("Hymn"), EHolypawBus::Combat, 0.68f, 0.90f, 8);
			Add(TEXT("DuskHymn"), EHolypawBus::Music, 0.58f, 1.80f, 4);
			Add(TEXT("BannerDown"), EHolypawBus::Sfx, 0.62f, 0.55f, 10);
			Add(TEXT("ShopOpen"), EHolypawBus::Ui, 0.52f, 0.28f, 12);
			Add(TEXT("Lullaby"), EHolypawBus::Combat, 0.64f, 1.05f, 8);
			Add(TEXT("Unstuff"), EHolypawBus::Combat, 0.74f, 0.45f, 20);
			Add(TEXT("PolyRip"), EHolypawBus::Combat, 0.76f, 0.50f, 16);
			Add(TEXT("Flee"), EHolypawBus::Combat, 0.55f, 0.40f, 25);
			Add(TEXT("MillSting"), EHolypawBus::Combat, 0.70f, 0.80f, 8);

			Add(TEXT("Theme"), EHolypawBus::Music, 0.40f, 0.5f, 0);
			return Out;
		}();
		return Table;
	}

	const FHolypawCueDef* Find(const FName Id)
	{
		return All().FindByPredicate([&](const FHolypawCueDef& D) { return D.Id == Id; });
	}

	EHolypawBus BusFor(const FName Id)
	{
		if (const FHolypawCueDef* D = Find(Id))
		{
			return D->Bus;
		}
		return EHolypawBus::Sfx;
	}

	TArray<FName> RequiredIds()
	{
		TArray<FName> Ids;
		for (const FHolypawCueDef& D : All())
		{
			Ids.Add(D.Id);
		}
		return Ids;
	}
}
