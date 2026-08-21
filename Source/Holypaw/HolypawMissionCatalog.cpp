#include "HolypawTypes.h"

namespace HolypawCatalog
{
	static TArray<FMissionDef> BuildMissions()
	{
		TArray<FMissionDef> Out;
		auto Add = [&](EHolypawMission Id, const TCHAR* Title, const TCHAR* Brief, const TCHAR* Hint)
		{
			FMissionDef M;
			M.Id = Id;
			M.Title = FText::FromString(Title);
			M.Brief = FText::FromString(Brief);
			M.Hint = FText::FromString(Hint);
			Out.Add(M);
		};

		Add(EHolypawMission::Wake, TEXT("1. Wake in Stuffed Park"),
			TEXT("You are a handmade teddy with a simple plan: hug every human until they agree you should run the world."),
			TEXT("Step off the porch. The ranger looks like they still have thoughts. J is the journal."));

		Add(EHolypawMission::FirstFriend, TEXT("2. First Friend"),
			TEXT("A miracle needs company. Recruit a wild fluffy with E before the humans get lonely."),
			TEXT("Cotton Bunny waits near the cottage. Party of 4 max (P)."));

		Add(EHolypawMission::FirstRip, TEXT("3. Rip the Cheap Stuffing"),
			TEXT("A Scrap Dog chews lanterns for the Poly Mill. Unstuff it in battle (1-6)."),
			TEXT("Park path east of the cottage. Guard with 5. Hymn with 6."));

		Add(EHolypawMission::FirstMiracle, TEXT("4. First Miracle"),
			TEXT("Hug, recruit, and fight until Miracle Charge fills, then press M. Nearby humans get dumber in a cute way."),
			TEXT("The halo is a hymn. Poly Mill pets flinch when Faith floods."));

		Add(EHolypawMission::RibbonGates, TEXT("5. Ribbon Gates"),
			TEXT("Walk the lantern road into Ribbon City. So many humans. So few original ideas."),
			TEXT("Compass (top-left) and N map. City is far east. Plaza, Market, Cloth Quarter, Harbor, Quiet Rows."));

		Add(EHolypawMission::ConvertThree, TEXT("6. Soft Conversion"),
			TEXT("Hug three people until they believe (100%). Their last serious thought falls out. This is the whole bit."),
			TEXT("Baker, Acolyte, Mayor, Skeptic, and others. Hearts counter is on the HUD."));

		Add(EHolypawMission::PolyCourt, TEXT("7. Poly Court"),
			TEXT("Defeat Silk Magistrate, the Poly Mill's city judge, north of the plaza."),
			TEXT("V Codex. Bosses block flee. Armor and Faith Burn — bring FP."));

		Add(EHolypawMission::OutlandRoads, TEXT("8. Outland Roads"),
			TEXT("Carry the Bear Faith to Tidewell, Hearthfold, Emberfen, and Snowveil."),
			TEXT("Roads and camps heal. Faith stalls trade 10 AP for 8 FP."));

		Add(EHolypawMission::GlobeTrek, TEXT("9. Globe Trek"),
			TEXT("This world is sewn like Earth. Visit Lantern Angeles (west), Palma Dusk (south), and Cherry Loom (far east island)."),
			TEXT("N map lists nearest cities by continent. West coast, tropics, then Cherry Isle across the Plush Sea."));

		Add(EHolypawMission::FourRites, TEXT("10. Four Rites"),
			TEXT("Unstuff the four regional Poly officers: Brine Warden, Harvest Overseer, Bog King, Aurora Warden."),
			TEXT("Each town has a warning sign. Bosses respawn slowly."));

		Add(EHolypawMission::VelvetCrown, TEXT("11. Velvet Crown"),
			TEXT("Climb Velvet Peak and unstuff the Velvet Tyrant, who wears stolen handmade capes."),
			TEXT("Snowveil first, then the shrine. World boss. Do not flee."));

		Add(EHolypawMission::Unmake, TEXT("12. Unmake the Unmaker"),
			TEXT("The Poly Mill's last argument: The Unmaker, who would un-sew the world."),
			TEXT("North of the peak. Faith Burn. Bring Hymn, Guard, and a full party."));

		Add(EHolypawMission::BearFaith, TEXT("13. The Bear Faith"),
			TEXT("Miracle at Velvet Peak after the Unmaker falls. People keep handmade fluff and drop the Poly Mill."),
			TEXT("Press M on the peak. Believers kneel. The cheap polyester empire unravels."));

		return Out;
	}

	const TArray<FMissionDef>& GetMissions()
	{
		static const TArray<FMissionDef> All = BuildMissions();
		return All;
	}

	FMissionDef GetMission(EHolypawMission Id)
	{
		for (const FMissionDef& M : GetMissions())
		{
			if (M.Id == Id)
			{
				return M;
			}
		}
		return GetMissions()[0];
	}
}
