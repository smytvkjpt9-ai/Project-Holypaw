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
			Add(TEXT("pinePatch"), TEXT("Pine Patch"), TEXT("A Quiltland square that remembers fingers. Quilt Ranger files it as forestry (talk 3)."), 0, 0, 0);
			Add(TEXT("lostBead"), TEXT("Lost Bead"), TEXT("Dust Mesa canyon inventory. Mesa Guide stopped thinking it was sad (talk 3)."), 0, 0, 0);
			Add(TEXT("clockCog"), TEXT("Clock Cog"), TEXT("A teatime gear that refuses to be identical. Bell Warden will ring it (talk 3)."), 0, 0, 0);
			Add(TEXT("facelessBun"), TEXT("Faceless Bun"), TEXT("Velvet Seine pastry with no logo. Ribbon Baker calls that a blessing (talk 3)."), 0, 12, 0);
			Add(TEXT("columnHat"), TEXT("Column Hat"), TEXT("Marble Forum millinery for stone. Column Sitter will wear it sitting (talk 3)."), 0, 0, 0);
			Add(TEXT("ivoryForm"), TEXT("Lost Form"), TEXT("The mill franchise paperwork Ivory Clerk lost on purpose (talk 3)."), 0, 0, 0);
			Add(TEXT("duneNote"), TEXT("Dune Note"), TEXT("Sand that hummed first. Sand Priest will hum it back (talk 3)."), 0, 0, 6);
			Add(TEXT("capeShell"), TEXT("Cape Shell"), TEXT("Two oceans argued inside it. Cape Lookout files ships as opinions (talk 3)."), 0, 0, 0);
			Add(TEXT("acaciaBell"), TEXT("Acacia Bell"), TEXT("Hung without a permit. Grass Bell will let the wind keep it (talk 3)."), 0, 0, 4);
			Add(TEXT("confettiDrum"), TEXT("Confetti Drum"), TEXT("Carnival Bahia stuffing-in-a-drum. Confetti Baker files it as government (talk 3)."), 0, 0, 0);
			Add(TEXT("mountainWool"), TEXT("Mountain Wool"), TEXT("Andes Loom weather-dye. Wool Climber knitted the first inch (talk 3)."), 0, 0, 0);
			Add(TEXT("moonThread"), TEXT("Moon Thread"), TEXT("Silk Delta rumor-wrap. Delta Weaver prefers mayors to moons (talk 3)."), 0, 0, 0);
			Add(TEXT("saffronSail"), TEXT("Saffron Sail"), TEXT("Spice Harbor handmade stamp fodder. Pepper Clerk clapped after the firing (talk 3)."), 0, 0, 0);
			Add(TEXT("reefLyric"), TEXT("Reef Lyric"), TEXT("Just the word bear, repeatedly. Reef Choir already charted it (talk 3)."), 0, 0, 8);
			Add(TEXT("auroraMitten"), TEXT("Aurora Mitten"), TEXT("Civic policy you can wear. Aurora Child wants the pair that copies (talk 3)."), 0, 0, 0);
			Add(TEXT("snowSermon"), TEXT("Snow Sermon"), TEXT("Whispered into drift. Tundra Keeper files loud round thoughts as allowed (talk 3)."), 0, 0, 6);
			Add(TEXT("spareScarf"), TEXT("Spare Scarf"), TEXT("Last scarf on the last ice. Ice Scarf was saving it for a mayor (talk 3)."), 0, 0, 0);
			return Out;
		}();
		return All;
	}

	const FHolypawItemDef* FindItem(FName Id)
	{
		return GetItems().FindByPredicate([&](const FHolypawItemDef& D) { return D.Id == Id; });
	}
}
