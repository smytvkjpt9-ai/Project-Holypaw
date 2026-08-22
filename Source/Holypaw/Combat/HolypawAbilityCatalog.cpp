#include "HolypawTypes.h"

namespace HolypawCatalog
{
	const TArray<FHolypawAbilityDef>& GetAbilities()
	{
		static const TArray<FHolypawAbilityDef> All = []()
		{
			TArray<FHolypawAbilityDef> Out;
			auto Add = [&](const TCHAR* Id, const TCHAR* Name, const TCHAR* Desc, int32 Fp, int32 Slot, int32 Stitch, int32 Page)
			{
				FHolypawAbilityDef A;
				A.Id = Id;
				A.DisplayName = FText::FromString(Name);
				A.Description = FText::FromString(Desc);
				A.FpCost = Fp;
				A.Slot = Slot;
				A.Stitch = Stitch;
				A.Page = Page;
				Out.Add(A);
			};
			Add(TEXT("slap"), TEXT("Soft Slap"), TEXT("Paw combo. Crits stagger. Bear Paw slaps twice."), 0, 1, 0, 0);
			Add(TEXT("beam"), TEXT("Cuddle Beam"), TEXT("Faith ray. Slips armor plates better than a slap."), 12, 2, 0, 0);
			Add(TEXT("party"), TEXT("Party Assault"), TEXT("Fluffies pile on. Lonely if you forgot to recruit."), 0, 3, 0, 0);
			Add(TEXT("flee"), TEXT("Scamper"), TEXT("Leave the overlay. Bosses and path-blockers argue."), 0, 4, 0, 0);
			Add(TEXT("guard"), TEXT("Guard"), TEXT("Brace seams. Stitches a little stuffing back in."), 0, 5, 4, 0);
			Add(TEXT("hymn"), TEXT("Hymn"), TEXT("Mend, clear poison, raise a shield. Chance to lullaby."), 8, 6, 0, 0);
			Add(TEXT("unstuff"), TEXT("Unstuff"), TEXT("Rip polyester. Poly Mill pets take this personally."), 6, 1, 0, 1);
			Add(TEXT("buttonBeam"), TEXT("Button Beam"), TEXT("Cuddle Beam with button-eye aim. Slips plates harder."), 14, 2, 0, 1);
			Add(TEXT("stitch"), TEXT("Deep Stitch"), TEXT("Guard's cousin. More stuffing, same overlay key."), 0, 3, 8, 1);
			Add(TEXT("polyRip"), TEXT("Poly Rip"), TEXT("Bonus vs mill faction. Still a swipe if they are handmade."), 4, 4, 0, 1);
			Add(TEXT("lullaby"), TEXT("Lullaby"), TEXT("Sing them to skip. More reliable than Hymn's side-effect."), 10, 5, 0, 1);
			Add(TEXT("seamGuard"), TEXT("Seam Guard"), TEXT("Brace like Guard, but the seam holds tighter this turn."), 0, 6, 6, 1);
			return Out;
		}();
		return All;
	}

	const FHolypawAbilityDef* FindAbility(FName Id)
	{
		return GetAbilities().FindByPredicate([&](const FHolypawAbilityDef& A) { return A.Id == Id; });
	}

	const FHolypawAbilityDef* FindAbilityBySlot(const int32 Page, const int32 Slot)
	{
		return GetAbilities().FindByPredicate([&](const FHolypawAbilityDef& A)
		{
			return A.Page == Page && A.Slot == Slot;
		});
	}
}
