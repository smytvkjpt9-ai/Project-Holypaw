#include "HolypawCatalog.h"

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
			Add(TEXT("pineSquare"), TEXT("Pine patch"),
				TEXT("Pocket a pine patch on Quiltland's Patch Walk and give it to the Quilt Ranger."),
				TEXT("Quilt Ranger"), TEXT("Quilt Ranger"), TEXT("pinePatch"), 1, 14, 8, 6.f,
				TEXT("A square that remembers fingers. Key 4. Key 3 when the pines agree."),
				TEXT("Ranger stitches the woods shut against beige. Mossgate can stay damp."));
			Add(TEXT("canyonBead"), TEXT("Canyon bead"),
				TEXT("Find a lost bead on Dust Mesa's Canyon Walk and give it to the Mesa Guide."),
				TEXT("Mesa Guide"), TEXT("Mesa Guide"), TEXT("lostBead"), 1, 14, 8, 0.f,
				TEXT("Canyons full of lost beads. Fetch one. I used to think that was sad."),
				TEXT("Guide pockets the bead. Inventory, not tragedy. The mill intern cacti sulk."));
			Add(TEXT("teaCog"), TEXT("Teatime cog"),
				TEXT("Pick a clock cog on Clockhaven's Cog Walk and give it to the Bell Warden."),
				TEXT("Bell Warden"), TEXT("Bell Warden"), TEXT("clockCog"), 1, 16, 8, 8.f,
				TEXT("A gear that refuses to be identical. Key 4. I will ring it until the fog claps."),
				TEXT("Warden rings the cog. Teatime applauds on delay. Velvet Seine can smell it."));
			Add(TEXT("blessingBun"), TEXT("Faceless bun"),
				TEXT("Take the faceless bun from Velvet Seine's Blessing Walk to the Ribbon Baker."),
				TEXT("Ribbon Baker"), TEXT("Ribbon Baker"), TEXT("facelessBun"), 1, 14, 8, 6.f,
				TEXT("The mill asked for a logo. Bring the bun with no face. Fair trade."),
				TEXT("Baker blesses a stranger with pastry. You already got a city."));
			Add(TEXT("sitHat"), TEXT("Column hat"),
				TEXT("Pocket a column hat on Marble Forum's Column Walk and give it to the Column Sitter."),
				TEXT("Column Sitter"), TEXT("Column Sitter"), TEXT("columnHat"), 1, 14, 8, 4.f,
				TEXT("Nine years sitting. Bring a hat. I will sit converted and millinery."),
				TEXT("Sitter wears the hat without standing. Growth."));
			Add(TEXT("lostFranchise"), TEXT("Lost form"),
				TEXT("Find the lost form on Ivory Spire's Lost Form Walk and give it to the Ivory Clerk."),
				TEXT("Ivory Clerk"), TEXT("Ivory Clerk"), TEXT("ivoryForm"), 1, 16, 8, 0.f,
				TEXT("The mill wanted a franchise. I lost the form. Fetch it so I can lose it again."),
				TEXT("Clerk stamps the form VOID in gold. The coup is legal because they said so."));
			Add(TEXT("noonHum"), TEXT("Dune note"),
				TEXT("Pick a dune note on Sand Hymn's Hum Walk and give it to the Sand Priest."),
				TEXT("Sand Priest"), TEXT("Sand Priest"), TEXT("duneNote"), 1, 14, 8, 10.f,
				TEXT("The dunes hummed first. Bring the note. Cocoa optional, recommended."),
				TEXT("Priest hums it back. The desert converts at noon on purpose."));
			Add(TEXT("twoOceans"), TEXT("Cape shell"),
				TEXT("Take a cape shell from Argument Cliff to the Cape Lookout."),
				TEXT("Cape Lookout"), TEXT("Cape Lookout"), TEXT("capeShell"), 1, 16, 8, 6.f,
				TEXT("Two oceans argued inside this. Key 4. I scan for remaining opinions."),
				TEXT("Lookout files the shell as weather. The math still checks out: two oceans, one bear."));
			Add(TEXT("windPermit"), TEXT("Acacia bell"),
				TEXT("Pick an acacia bell on Savannah Bell's Acacia Walk and give it to Grass Bell."),
				TEXT("Grass Bell"), TEXT("Grass Bell"), TEXT("acaciaBell"), 1, 14, 8, 8.f,
				TEXT("Hung without a permit. Fetch it. Wind does hymns anyway."),
				TEXT("Grass Bell lets the weather keep the permit. Gold already lives here."));
			Add(TEXT("drumGov"), TEXT("Confetti drum"),
				TEXT("Pocket a confetti drum on Carnival Bahia's Drum Walk and give it to the Confetti Baker."),
				TEXT("Confetti Baker"), TEXT("Confetti Baker"), TEXT("confettiDrum"), 1, 16, 8, 6.f,
				TEXT("Stuffing in the drums. Key 4. Confetti is a valid government."),
				TEXT("Baker stamps the drum as policy. The parade converts between beats."));
			Add(TEXT("firstInch"), TEXT("Mountain wool"),
				TEXT("Take mountain wool from Andes Loom's Wool Walk to the Wool Climber."),
				TEXT("Wool Climber"), TEXT("Wool Climber"), TEXT("mountainWool"), 1, 16, 8, 8.f,
				TEXT("I knitted the first inch. Bring the next. Don't mill this mountain."),
				TEXT("Climber knits the spine shut against brochures. Carnival can stay loud."));
			Add(TEXT("wrapMoon"), TEXT("Moon thread"),
				TEXT("Find moon thread on Silk Delta's Thread Walk and give it to the Delta Weaver."),
				TEXT("Delta Weaver"), TEXT("Delta Weaver"), TEXT("moonThread"), 1, 18, 10, 8.f,
				TEXT("Thread enough to wrap a moon. Wrap a factory rumor instead. Key 3 when it holds."),
				TEXT("Weaver wraps the rumor. Cherry Isle can clap in pink now."));
			Add(TEXT("handmadePepper"), TEXT("Saffron sail"),
				TEXT("Pick a saffron sail on Spice Harbor's Sail Walk and give it to the Pepper Clerk."),
				TEXT("Pepper Clerk"), TEXT("Pepper Clerk"), TEXT("saffronSail"), 1, 16, 8, 4.f,
				TEXT("I stamped handmade on identical pepper. They fired me. Bring a sail. I will clap."),
				TEXT("Clerk stamps the sail VOID-BEIGE. Mill scrap still belongs in a museum."));
			Add(TEXT("bearLyric"), TEXT("Reef lyric"),
				TEXT("Take a reef lyric from Coral Choir's Reef Walk to the Reef Choir."),
				TEXT("Reef Choir"), TEXT("Reef Choir"), TEXT("reefLyric"), 1, 14, 8, 10.f,
				TEXT("We already sang. Bring lyrics that are just the word bear. Chart-topping."),
				TEXT("Choir files the lyric. The lagoon stays off-key on purpose."));
			Add(TEXT("mittenLaw"), TEXT("Aurora mitten"),
				TEXT("Pocket an aurora mitten on Copy Walk and give it to the Aurora Child."),
				TEXT("Aurora Child"), TEXT("Aurora Child"), TEXT("auroraMitten"), 1, 14, 8, 8.f,
				TEXT("The lights copy you if you glow. Bring a mitten. Civic policy you can wear."),
				TEXT("Child wears the pair. The lights copy. Rude. Cute."));
			Add(TEXT("loudAllowed"), TEXT("Snow sermon"),
				TEXT("Find a snow sermon on Tundra Parish's Quiet Walk and give it to the Tundra Keeper."),
				TEXT("Tundra Keeper"), TEXT("Tundra Keeper"), TEXT("snowSermon"), 1, 14, 8, 6.f,
				TEXT("Sermons whispered into snow. Bring one. You are loud in a round way. Allowed."),
				TEXT("Keeper files the sermon. Silence keeps the cold honest. You can stay loud."));
			Add(TEXT("mayorSpare"), TEXT("Spare scarf"),
				TEXT("Pick the spare scarf on Felt Ice Camp's Spare Walk and give it to Ice Scarf."),
				TEXT("Ice Scarf"), TEXT("Ice Scarf"), TEXT("spareScarf"), 1, 16, 8, 8.f,
				TEXT("I was saving it for a mayor. You can have the spare. Key 3 when it warms."),
				TEXT("Ice Scarf ties the last knot. The coup echoes. The porch still remembers you."));
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
