#include "HolypawTypes.h"

namespace HolypawCatalog
{
	const TArray<FHolypawAbilityDef>& GetAbilities()
	{
		static const TArray<FHolypawAbilityDef> All = []()
		{
			TArray<FHolypawAbilityDef> Out;
			auto Add = [&](const TCHAR* Id, const TCHAR* Name, const TCHAR* Desc, int32 Fp, int32 Slot, int32 Stitch)
			{
				FHolypawAbilityDef A;
				A.Id = Id;
				A.DisplayName = FText::FromString(Name);
				A.Description = FText::FromString(Desc);
				A.FpCost = Fp;
				A.Slot = Slot;
				A.Stitch = Stitch;
				Out.Add(A);
			};
			Add(TEXT("slap"), TEXT("Soft Slap"), TEXT("Paw combo. Crits stagger. Bear Paw slaps twice."), 0, 1, 0);
			Add(TEXT("beam"), TEXT("Cuddle Beam"), TEXT("Faith ray. Slips armor plates better than a slap."), 12, 2, 0);
			Add(TEXT("party"), TEXT("Party Assault"), TEXT("Fluffies pile on. Lonely if you forgot to recruit."), 0, 3, 0);
			Add(TEXT("flee"), TEXT("Scamper"), TEXT("Leave the overlay. Bosses and path-blockers argue."), 0, 4, 0);
			Add(TEXT("guard"), TEXT("Guard"), TEXT("Brace seams. Stitches a little stuffing back in."), 0, 5, 4);
			Add(TEXT("hymn"), TEXT("Hymn"), TEXT("Mend, clear poison, raise a shield. Chance to lullaby."), 8, 6, 0);
			Add(TEXT("unstuff"), TEXT("Unstuff"), TEXT("Rip polyester. Poly Mill pets take this personally. Overflow."), 6, 0, 0);
			Add(TEXT("buttonBeam"), TEXT("Button Beam"), TEXT("Cuddle Beam with button-eye aim. Overflow until command list grows."), 14, 0, 0);
			Add(TEXT("stitch"), TEXT("Deep Stitch"), TEXT("Guard's cousin. More stuffing, same overlay slot later."), 0, 0, 8);
			Add(TEXT("polyRip"), TEXT("Poly Rip"), TEXT("Skill-flagged bonus vs mill faction. Catalogued so directors can find it."), 0, 0, 0);
			Add(TEXT("lullaby"), TEXT("Lullaby"), TEXT("Hymn side-effect. 28% skip their turn."), 0, 0, 0);
			Add(TEXT("seamGuard"), TEXT("Seam Guard"), TEXT("Skill-flagged brace. Catalogued for the director."), 0, 0, 0);
			return Out;
		}();
		return All;
	}

	const FHolypawAbilityDef* FindAbility(FName Id)
	{
		return GetAbilities().FindByPredicate([&](const FHolypawAbilityDef& A) { return A.Id == Id; });
	}
}
