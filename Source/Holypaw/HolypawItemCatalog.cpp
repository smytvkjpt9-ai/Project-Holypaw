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
			Add(TEXT("millScrap"), TEXT("Mill Scrap"), TEXT("Cheap polyester. Proof the factory bleeds."), 0, 0, 0);
			return Out;
		}();
		return All;
	}

	const FHolypawItemDef* FindItem(FName Id)
	{
		return GetItems().FindByPredicate([&](const FHolypawItemDef& D) { return D.Id == Id; });
	}
}
