#include "HolypawTypes.h"

namespace HolypawCatalog
{
	const TArray<FHolypawQuestDef>& GetQuests()
	{
		static const TArray<FHolypawQuestDef> All = []()
		{
			TArray<FHolypawQuestDef> Out;
			auto Add = [&](const TCHAR* Id, const TCHAR* Title, const TCHAR* Brief, const TCHAR* Giver, const TCHAR* TurnIn,
				const TCHAR* Need, int32 Count, int32 AP, int32 FP, float Miracle, const TCHAR* Offer, const TCHAR* Done)
			{
				FHolypawQuestDef Q;
				Q.Id = Id;
				Q.Title = FText::FromString(Title);
				Q.Brief = FText::FromString(Brief);
				Q.Giver = Giver;
				Q.TurnIn = TurnIn;
				Q.NeedItem = Need;
				Q.NeedCount = Count;
				Q.RewardAP = AP;
				Q.RewardFP = FP;
				Q.RewardMiracle = Miracle;
				Q.OfferLine = Offer;
				Q.DoneLine = Done;
				Out.Add(Q);
			};
			Add(TEXT("cellarHymn"), TEXT("Cellar Hymn"),
				TEXT("Pocket the cottage cellar sheet and give it to Choir Bear."),
				TEXT("Choir Bear"), TEXT("Choir Bear"), TEXT("hymnSheet"), 1, 10, 6, 40.f,
				TEXT("Bring the cellar lyrics. I will pretend I can read music."),
				TEXT("Choir Bear sight-reads your cellar hymn. The plaza goes rounder."));
			Add(TEXT("scrapExhibit"), TEXT("Exhibit Bear"),
				TEXT("Unstuff a Poly Mill pet, then hand mill scrap to the whistleblower."),
				TEXT("Mill Whistleblower"), TEXT("Mill Whistleblower"), TEXT("millScrap"), 1, 18, 8, 0.f,
				TEXT("Steal their beige. I will file it as Exhibit Bear."),
				TEXT("Whistleblower stamps the scrap. The mill is now evidence."));
			Add(TEXT("tidePearl"), TEXT("Net Pearl"),
				TEXT("Find a salt pearl on Tidewell's Net Walk and give it to the Net Weaver."),
				TEXT("Net Weaver"), TEXT("Net Weaver"), TEXT("saltPearl"), 1, 16, 10, 8.f,
				TEXT("A pearl rolled under the nets. Fetch it. I will clap in brine."),
				TEXT("Net Weaver knots the pearl into a hymn. Tidewell agrees."));
			Add(TEXT("bunForChild"), TEXT("Warm bun"),
				TEXT("The Child on the lantern road wants a stuffing bun. You start with one."),
				TEXT("Child"), TEXT("Child"), TEXT("stuffingBun"), 1, 8, 4, 6.f,
				TEXT("If the mill cannot bake warmth, you can. One bun. I will clap with crumbs."),
				TEXT("The Child is spherical with joy. Crumbs become policy."));
			Add(TEXT("postcardHome"), TEXT("Porch mail"),
				TEXT("Pocket the porch postcard and show the Park Ranger you woke up."),
				TEXT("Park Ranger"), TEXT("Park Ranger"), TEXT("stuffedPostcard"), 1, 8, 4, 0.f,
				TEXT("If you found the porch postcard, I can close the missing-bear report."),
				TEXT("Ranger files 'bear present.' The park is legally yours."));
			Add(TEXT("brineHymn"), TEXT("Salt choir"),
				TEXT("Carry a hymn ribbon to Tidewell's Salt Priest."),
				TEXT("Salt Priest"), TEXT("Salt Priest"), TEXT("hymnRibbon"), 1, 14, 8, 12.f,
				TEXT("Ribbon from the Cloth Loft or a stall. I will baptize it in brine."),
				TEXT("Salt Priest dunks the ribbon. The harbor sings off-key on purpose."));
			Add(TEXT("wheatHymn"), TEXT("Wheat ear"),
				TEXT("Pocket a wheat ear in Hearthfold's Wheat Choir and give it to the Farmer."),
				TEXT("Farmer"), TEXT("Farmer"), TEXT("wheatEar"), 1, 14, 8, 6.f,
				TEXT("The choir dropped an ear. Fetch it. Grain should be lumpy."),
				TEXT("Farmer plants the ear. The field hums a round hymn."));
			Add(TEXT("peatKeepsake"), TEXT("Peat bead"),
				TEXT("Take the peat bead from Emberfen's Witch Stoop to the Mud Sculptor."),
				TEXT("Mud Sculptor"), TEXT("Mud Sculptor"), TEXT("peatBead"), 1, 14, 8, 8.f,
				TEXT("A bead of cooled jam. I will sculpt a bear that stays wet on purpose."),
				TEXT("Mud Sculptor presses the bead into a snout. The fen claps."));
			Add(TEXT("ridgeThread"), TEXT("Aurora thread"),
				TEXT("Pick aurora thread on Snowveil's Scarf Walk and give it to the Snow Warden."),
				TEXT("Snow Warden"), TEXT("Snow Warden"), TEXT("auroraThread"), 1, 16, 8, 10.f,
				TEXT("The scarves dropped a thread the mill cannot copy. Bring it."),
				TEXT("Warden knits the ridge shut against beige. Peak is next."));
			Add(TEXT("goldReel"), TEXT("Exhibit Glow"),
				TEXT("Pocket a gold poster on Lantern Angeles' Marquee Walk and give it to Studio Grip."),
				TEXT("Studio Grip"), TEXT("Studio Grip"), TEXT("goldPoster"), 1, 16, 8, 6.f,
				TEXT("Steal a gold poster. I dropped the light. You drop the lie. Key 3 when it shines."),
				TEXT("Grip stamps the poster Exhibit Glow. The lot converts between takes."));
			Add(TEXT("teaFog"), TEXT("Fog cup"),
				TEXT("Find fog tea on Mossgate's Tea Walk and give it to the Fog Baker."),
				TEXT("Fog Baker"), TEXT("Fog Baker"), TEXT("fogTea"), 1, 14, 8, 8.f,
				TEXT("Tea that tastes like rain if you believe a bear. Key 4. Key 3 when the cup is warm."),
				TEXT("Fog Baker pours. Mossgate forgives polyester a little slower, on purpose."));
			Add(TEXT("duskClap"), TEXT("Palm duet"),
				TEXT("Pick a palm clap on Palma Dusk's Clap Pier and give it to the Palm Singer."),
				TEXT("Palm Singer"), TEXT("Palm Singer"), TEXT("palmClap"), 1, 16, 8, 10.f,
				TEXT("Palms clap at hymns. Bring the token. We will duet until dusk agrees."),
				TEXT("Palm Singer and the pier clap together. Two oceans take notes."));
			Add(TEXT("loomWrap"), TEXT("Cherry wrap"),
				TEXT("Take cherry thread from Cherry Loom's Loom Walk to the Loom Weaver."),
				TEXT("Loom Weaver"), TEXT("Loom Weaver"), TEXT("cherryThread"), 1, 18, 10, 8.f,
				TEXT("Thread enough to wrap a mayor. Fetch it. Globe Trek ends when the wrap holds."),
				TEXT("Weaver wraps the rumor shut. Cherry Isle claps in pink."));
			return Out;
		}();
		return All;
	}

	const FHolypawQuestDef* FindQuest(FName Id)
	{
		return GetQuests().FindByPredicate([&](const FHolypawQuestDef& Q) { return Q.Id == Id; });
	}

	const FHolypawQuestDef* FindQuestByGiver(const FString& Speaker)
	{
		return GetQuests().FindByPredicate([&](const FHolypawQuestDef& Q)
		{
			return Q.Giver.Equals(Speaker, ESearchCase::IgnoreCase);
		});
	}
}
