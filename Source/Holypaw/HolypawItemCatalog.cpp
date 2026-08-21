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
			Add(TEXT("stuffedPostcard"), TEXT("Stuffed Postcard"), TEXT("The porch wrote home. Decorative. Proof you woke up."), 0, 0, 0);
			return Out;
		}();
		return All;
	}

	const FHolypawItemDef* FindItem(FName Id)
	{
		return GetItems().FindByPredicate([&](const FHolypawItemDef& D) { return D.Id == Id; });
	}
}
