#include "HolypawTypes.h"

namespace HolypawCatalog
{
	const TArray<FHolypawItemDef>& GetItems()
	{
		static const TArray<FHolypawItemDef> All = []()
		{
			TArray<FHolypawItemDef> Out;
			auto Add = [&](const TCHAR* Id, const TCHAR* Name, const TCHAR* Desc, int32 Cost, int32 Heal, int32 Faith)
			{
				FHolypawItemDef D;
				D.Id = Id;
				D.DisplayName = FText::FromString(Name);
				D.Description = FText::FromString(Desc);
				D.ShopCostAP = Cost;
				D.Heal = Heal;
				D.Faith = Faith;
				Out.Add(D);
			};
			Add(TEXT("stuffingBun"), TEXT("Stuffing Bun"), TEXT("A warm bun full of leftover hugs. Mends 20 HP."), 12, 20, 0);
			Add(TEXT("hymnRibbon"), TEXT("Hymn Ribbon"), TEXT("Tied around a paw. Restores 10 FP."), 15, 0, 10);
			Add(TEXT("cocoaButton"), TEXT("Cocoa Button"), TEXT("Edible only in the Bear Faith. +12 HP and +4 FP."), 18, 12, 4);
			Add(TEXT("millScrap"), TEXT("Mill Scrap"), TEXT("Cheap polyester. Proof the factory bleeds. Hand to the whistleblower (talk 3)."), 0, 0, 0);
			Add(TEXT("hymnSheet"), TEXT("Hymn Sheet"), TEXT("Cellar lyrics. Choir Bear will sight-read this (talk 3). Restores 16 FP if you keep it."), 0, 0, 16);
			Add(TEXT("stuffedPostcard"), TEXT("Stuffed Postcard"), TEXT("The porch wrote home. Decorative. Proof you woke up. Ranger wants it (talk 3)."), 0, 0, 0);
			Add(TEXT("saltPearl"), TEXT("Salt Pearl"), TEXT("Tidewell net-walk treasure. Net Weaver will knot it into a hymn (talk 3)."), 0, 0, 0);
			Add(TEXT("brineTaffy"), TEXT("Brine Taffy"), TEXT("Chewy salt. Mends 8 HP and tastes like an argument."), 9, 8, 0);
			Add(TEXT("wheatEar"), TEXT("Wheat Ear"), TEXT("Lumpy grain from Hearthfold's choir. The Farmer wants it (talk 3)."), 0, 0, 0);
			Add(TEXT("peatBead"), TEXT("Peat Bead"), TEXT("Cooled jam from Emberfen. Mud Sculptor will make a wet bear (talk 3)."), 0, 0, 0);
			Add(TEXT("auroraThread"), TEXT("Aurora Thread"), TEXT("Snowveil scarf-walk yarn. The mill cannot copy it. Warden wants it (talk 3)."), 0, 0, 0);
			Add(TEXT("goldPoster"), TEXT("Gold Poster"), TEXT("A Lantern Angeles marquee lie. Studio Grip files it as Exhibit Glow (talk 3)."), 0, 0, 0);
			Add(TEXT("fogTea"), TEXT("Fog Tea"), TEXT("Mossgate rain steeped in a cup. Fog Baker believes a bear (talk 3)."), 0, 0, 4);
			Add(TEXT("palmClap"), TEXT("Palm Clap"), TEXT("A Palma Dusk token that claps when you glow. Palm Singer wants the duet (talk 3)."), 0, 0, 0);
			Add(TEXT("cherryThread"), TEXT("Cherry Thread"), TEXT("Pink loom yarn that can wrap a rumor. Loom Weaver will finish the wrap (talk 3)."), 0, 0, 0);
			return Out;
		}();
		return All;
	}

	const FHolypawItemDef* FindItem(FName Id)
	{
		return GetItems().FindByPredicate([&](const FHolypawItemDef& D) { return D.Id == Id; });
	}
}
