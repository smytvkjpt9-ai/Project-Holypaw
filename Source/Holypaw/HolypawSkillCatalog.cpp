#include "HolypawTypes.h"

namespace HolypawCatalog
{
	TArray<FSkillDef> MakeSkills()
	{
		TArray<FSkillDef> Out;
		auto Add = [&](EHolypawSkillTree Tree, const TCHAR* Id, const TCHAR* Name, int32 Cost, const TCHAR* Desc, const TCHAR* Req)
		{
			FSkillDef S;
			S.Tree = Tree;
			S.Id = Id;
			S.DisplayName = FText::FromString(Name);
			S.Description = FText::FromString(Desc);
			S.Cost = Cost;
			S.Prerequisite = Req;
			Out.Add(S);
		};

		Add(EHolypawSkillTree::Hug, TEXT("softFur"), TEXT("Soft Fur"), 15, TEXT("+3 Attack. Hugs feel irresistible."), TEXT(""));
		Add(EHolypawSkillTree::Hug, TEXT("buttonEyes"), TEXT("Button Eyes"), 25, TEXT("+20% Affection from hugs."), TEXT("softFur"));
		Add(EHolypawSkillTree::Hug, TEXT("deepHug"), TEXT("Deep Hug"), 35, TEXT("Converts gain +15 extra progress."), TEXT("buttonEyes"));
		Add(EHolypawSkillTree::Hug, TEXT("stitchHeart"), TEXT("Stitch Heart"), 40, TEXT("Hugs mend 8 HP."), TEXT("deepHug"));
		Add(EHolypawSkillTree::Hug, TEXT("irresistible"), TEXT("Irresistible"), 55, TEXT("Hugs grant +50% AP."), TEXT("stitchHeart"));
		Add(EHolypawSkillTree::Hug, TEXT("bearPaw"), TEXT("Bear Paw"), 60, TEXT("Soft Slap hits a second time."), TEXT("irresistible"));

		Add(EHolypawSkillTree::Miracle, TEXT("haloStep"), TEXT("Halo Step"), 20, TEXT("+120 move speed and a visible halo."), TEXT(""));
		Add(EHolypawSkillTree::Miracle, TEXT("miracleEcho"), TEXT("Miracle Echo"), 40, TEXT("Miracles grant +50% Faith."), TEXT("haloStep"));
		Add(EHolypawSkillTree::Miracle, TEXT("hymnWard"), TEXT("Hymn Ward"), 45, TEXT("Battle Hymn heals more."), TEXT("miracleEcho"));
		Add(EHolypawSkillTree::Miracle, TEXT("faithArmor"), TEXT("Faith Armor"), 50, TEXT("Enemy hits deal 2 less."), TEXT("hymnWard"));
		Add(EHolypawSkillTree::Miracle, TEXT("peakLiturgy"), TEXT("Peak Liturgy"), 65, TEXT("Miracles mend 30 HP."), TEXT("faithArmor"));
		Add(EHolypawSkillTree::Miracle, TEXT("bearCreed"), TEXT("Bear Creed"), 80, TEXT("Miracles convert nearby people +40%."), TEXT("peakLiturgy"));

		Add(EHolypawSkillTree::Party, TEXT("partyBond"), TEXT("Party Bond"), 25, TEXT("Party Assault deals +40%."), TEXT(""));
		Add(EHolypawSkillTree::Party, TEXT("fluffShield"), TEXT("Fluff Shield"), 40, TEXT("+20 Max HP. Start battles braced."), TEXT("partyBond"));
		Add(EHolypawSkillTree::Party, TEXT("rallyCry"), TEXT("Rally Cry"), 45, TEXT("Party members deal +2 Attack."), TEXT("fluffShield"));
		Add(EHolypawSkillTree::Party, TEXT("seamGuard"), TEXT("Seam Guard"), 50, TEXT("Guard blocks most of the next hit."), TEXT("rallyCry"));
		Add(EHolypawSkillTree::Party, TEXT("polyRip"), TEXT("Poly Rip"), 60, TEXT("+6 damage vs Poly Mill stuffing."), TEXT("seamGuard"));
		Add(EHolypawSkillTree::Party, TEXT("choirAssault"), TEXT("Choir Assault"), 75, TEXT("Party Assault hits twice."), TEXT("polyRip"));
		return Out;
	}

	TArray<FSkillDef> SkillsForTree(EHolypawSkillTree Tree)
	{
		TArray<FSkillDef> Out;
		for (const FSkillDef& S : MakeSkills())
		{
			if (S.Tree == Tree)
			{
				Out.Add(S);
			}
		}
		return Out;
	}

	const TCHAR* SkillTreeName(EHolypawSkillTree Tree)
	{
		switch (Tree)
		{
		case EHolypawSkillTree::Hug: return TEXT("Hug Tree");
		case EHolypawSkillTree::Miracle: return TEXT("Miracle Tree");
		case EHolypawSkillTree::Party: return TEXT("Party Tree");
		default: return TEXT("Skills");
		}
	}
}
