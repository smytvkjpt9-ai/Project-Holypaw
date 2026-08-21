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
			TEXT("You are a handmade teddy. Cheap polyester from the Poly Mill wants the world smooth, identical, and unloved."),
			TEXT("Step off the porch. Follow the lanterns. J opens the journal."));

		Add(EHolypawMission::FirstFriend, TEXT("2. First Friend"),
			TEXT("A miracle needs company. Recruit a wild fluffy with E."),
			TEXT("Cotton Bunny waits near the cottage. Party of 4 max (P)."));

		Add(EHolypawMission::FirstRip, TEXT("3. Rip the Cheap Stuffing"),
			TEXT("A Scrap Dog chews lanterns for the Poly Mill. Unstuff it in battle (1-6)."),
			TEXT("Park path east of the cottage. Guard with 5. Hymn with 6."));

		Add(EHolypawMission::FirstMiracle, TEXT("4. First Miracle"),
			TEXT("Hug, recruit, and fight until Miracle Charge fills, then press M."),
			TEXT("The halo is a hymn. Poly Mill pets flinch when Faith floods."));

		Add(EHolypawMission::RibbonGates, TEXT("5. Ribbon Gates"),
			TEXT("Walk the lantern road into Ribbon City. The Poly Mill holds court there."),
			TEXT("Compass (top-left) and N map. City is far east."));

		Add(EHolypawMission::ConvertThree, TEXT("6. Soft Conversion"),
			TEXT("Hug three people until they believe (100%). Faith is handmade, not factory."),
			TEXT("Baker, Acolyte, Mayor, and others live in the settlements."));

		Add(EHolypawMission::PolyCourt, TEXT("7. Poly Court"),
			TEXT("Defeat Silk Magistrate, the Poly Mill's city judge, north of the plaza."),
			TEXT("V Codex. Bosses block flee. Armor and Faith Burn — bring FP."));

		Add(EHolypawMission::OutlandRoads, TEXT("8. Outland Roads"),
			TEXT("Carry the Bear Faith to Tidewell, Hearthfold, Emberfen, and Snowveil."),
			TEXT("Roads and camps heal. Faith stalls trade 10 AP for 8 FP."));

		Add(EHolypawMission::FourRites, TEXT("9. Four Rites"),
			TEXT("Unstuff the four regional Poly officers: Brine Warden, Harvest Overseer, Bog King, Aurora Warden."),
			TEXT("Each town has a warning sign. Bosses respawn slowly."));

		Add(EHolypawMission::VelvetCrown, TEXT("10. Velvet Crown"),
			TEXT("Climb Velvet Peak and unstuff the Velvet Tyrant, who wears stolen handmade capes."),
			TEXT("Snowveil first, then the shrine. World boss. Do not flee."));

		Add(EHolypawMission::Unmake, TEXT("11. Unmake the Unmaker"),
			TEXT("The Poly Mill's last argument: The Unmaker, who would un-sew the world."),
			TEXT("North of the peak. Faith Burn. Bring Hymn, Guard, and a full party."));

		Add(EHolypawMission::BearFaith, TEXT("12. The Bear Faith"),
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
