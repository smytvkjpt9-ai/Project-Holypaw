#include "HolypawTypes.h"

namespace HolypawCatalog
{
	const TArray<FHolypawTalkDef>& GetTalks()
	{
		static const TArray<FHolypawTalkDef> All = []()
		{
			TArray<FHolypawTalkDef> Out;
			auto Add = [&](const TCHAR* Who, const TCHAR* A, const TCHAR* B, const TCHAR* Hint)
			{
				FHolypawTalkDef T;
				T.Speaker = Who;
				T.Line = A;
				T.LineB = B;
				T.Hint = Hint;
				Out.Add(T);
			};
			Add(TEXT("Park Ranger"),
				TEXT("I used to write tickets. Now I write hymns in the margin. East, the lanterns. Don't overthink it."),
				TEXT("The Skeptic by the porch is almost ready. One more hug and he'll clap."),
				TEXT("Lantern road east. Ribbon City is a buffet of opinions."));
			Add(TEXT("Skeptic"),
				TEXT("I had a five-year plan. It was rectangular. You made it round. I am... fine with that."),
				TEXT("If anyone asks, I converted for tax reasons. There are no taxes. There is a bear."),
				TEXT("Hug first, commute later. The city waits."));
			Add(TEXT("Child"),
				TEXT("Can the mill make a teddy as warm as you? I asked. The mill said no, then offered a coupon."),
				TEXT("I clapped so hard my mittens flew off. Keep going."),
				TEXT("Nursery hills, then the gold lanterns."));
			Add(TEXT("Baker"),
				TEXT("I used to sell identical rolls. Now every bun has a secret. The mill hated that sentence."),
				TEXT("Stuffing Buns are on the stall. Buy one. Be spherical."),
				TEXT("Market east of the plaza. Poly Mill further east, grey and rude."));
			Add(TEXT("Acolyte"),
				TEXT("I practiced kneeling before it was fashionable. You're late and also perfect."),
				TEXT("Miracle at dusk. The sky agrees harder then."),
				TEXT("North of the plaza the Silk Magistrate holds cheap court."));
			Add(TEXT("Mayor"),
				TEXT("Ribbon City hereby recognizes the Bear Faith as extremely reasonable. I did not take a bribe. I took a hug."),
				TEXT("Hearts open shops. Convert my people and the lanterns get friendlier."),
				TEXT("Poly Mill sits east of town. Polyester, no soul, excellent to unstuff."));
			Add(TEXT("Market Hawker"),
				TEXT("Faith jars, ribbons, buns — not factory smiles. Touch anything handmade. Leave the polyester."),
				TEXT("If you have mill scrap I'll gasp on purpose."),
				TEXT("Cloth Quarter is west. Harbor Steps splash east."));
			Add(TEXT("Cloth Dyer"),
				TEXT("I dye banners the color of leftover sunset. The mill dyes everything beige and calls it efficiency."),
				TEXT("Handmade thread remembers fingers. Cheap thread remembers meetings."),
				TEXT("Quiet Rows south of the plaza. People there convert softly."));
			Add(TEXT("Harbor Kid"),
				TEXT("I can skip a stone three times. I can skip a serious thought forever now."),
				TEXT("Boats go to Tidewell. Hugs go everywhere."),
				TEXT("Follow the docks. Tidewell smells like salt and opinions."));
			Add(TEXT("Quiet Neighbor"),
				TEXT("I will not make a speech. I will clap once, correctly."),
				TEXT("Thank you for not asking me to network."),
				TEXT("Lanterns by the spire hop you home."));
			Add(TEXT("Harbor Hand"),
				TEXT("Nets catch fish. You catch governments. Same wrist motion."),
				TEXT("Brine Warden walks the outer dock. He's very tall about it."),
				TEXT("Tidewell lantern. Then Cherry rumors if you're brave."));
			Add(TEXT("Net Weaver"),
				TEXT("I mended a hole and accidentally mended a worldview. Occupied hazard."),
				TEXT("Salt crabs pinch. You pinch back with hymns."),
				TEXT("East docks, then the ferry camp."));
			Add(TEXT("Farmer"),
				TEXT("Grain grew better after I stopped yelling at clouds. I yell at the mill now. Clouds approved."),
				TEXT("Harvest Overseer north of the mill. Bring a party."),
				TEXT("Hearthfold lantern. Hymns in the wheat."));
			Add(TEXT("Miller"),
				TEXT("I ground identical flour for identical bread. You are lumpy. I am converted."),
				TEXT("Buy a bun. Be lumpy with me."),
				TEXT("Fields north. Scarecrows that forgot their job."));
			Add(TEXT("Fen Guide"),
				TEXT("The peat remembers every boot. It will remember your paws as an improvement."),
				TEXT("Bog King sits south in the deepest jam. Don't go alone unless you like drowning cutely."),
				TEXT("Emberfen lantern. Frogs the color of cooling jam."));
			Add(TEXT("Mud Sculptor"),
				TEXT("I sculpted a bear before I met you. It was a sign. Or mud. Same thing."),
				TEXT("Fen witch-pets hiss in F-sharp. Hymn in G."),
				TEXT("South into the peat. Bring Faith."));
			Add(TEXT("Snow Warden"),
				TEXT("I kept the ridge boring on purpose. You showed up glowing. My job is cancelled."),
				TEXT("Aurora Warden, then Velvet Tyrant, then The Unmaker. In that order, like a recipe."),
				TEXT("Snowveil lantern. Peak is up. Dress warm. You are already fur."));
			Add(TEXT("Peak Acolyte"),
				TEXT("The shrine was empty because we were waiting for someone round."),
				TEXT("Miracle on the peak after the Unmaker. That's the last serious thought on Earth."),
				TEXT("Up. Always up. The rift likes company."));
			Add(TEXT("Studio Grip"),
				TEXT("Lantern Angeles shot a commercial for identical pets. I dropped the light on purpose."),
				TEXT("Gold posters lie. Your fur does not."),
				TEXT("Pacific camp between here and Mossgate."));
			Add(TEXT("Fog Baker"),
				TEXT("Fog tastes like tea if you believe a bear. I believe a bear."),
				TEXT("Mossgate rain forgives polyester slower than hugs do."),
				TEXT("Quiltland is further north woods."));
			Add(TEXT("Quilt Ranger"),
				TEXT("I patch pines. You patch people. Union pending."),
				TEXT("Bring a ribbon. The trees like accessories."),
				TEXT("Dust Mesa is a long south-west of beige."));
			Add(TEXT("Mesa Guide"),
				TEXT("Canyons full of lost beads. I used to think that was sad. Now it's inventory."),
				TEXT("Don't trust identical cacti. They're mill interns."),
				TEXT("Palma Dusk if you want palms that clap."));
			Add(TEXT("Palm Singer"),
				TEXT("We clap at hymns. You clap at humans. Collaboration."),
				TEXT("Two oceans argue politely at Cape Plush."),
				TEXT("Sand Hymn dunes hum at noon. Dusk is ruder and better."));
			Add(TEXT("Ivory Clerk"),
				TEXT("Old stone, new stuffing. I stamped your coup. It is legal because I said so."),
				TEXT("North-shore spire. The mill wanted a franchise. I lost the form."),
				TEXT("Lantern here hops you off the beige."));
			Add(TEXT("Sand Priest"),
				TEXT("Dunes that hum at noon. I hummed back. They converted first."),
				TEXT("Bring a cocoa button. Deserts respect chocolate theology."),
				TEXT("Cape Plush south if you like argumentative water."));
			Add(TEXT("Cape Lookout"),
				TEXT("Two oceans, one bear. The math checks out."),
				TEXT("I used to scan for ships. Now I scan for remaining opinions."),
				TEXT("Coral Choir if you can stand being serenaded by reef."));
			Add(TEXT("Loom Weaver"),
				TEXT("Cherry thread can wrap a moon. We wrapped a mayor instead. More useful."),
				TEXT("Globe Trek ends when you've seen our blossoms and still wanted hugs."),
				TEXT("Silk Delta west, Spice Harbor southwest."));
			Add(TEXT("Aurora Child"),
				TEXT("The lights are shy until you glow. Then they copy you. Rude. Cute."),
				TEXT("Tundra Parish keeps the cold honest."),
				TEXT("Felt Ice Camp is the last scarf on Earth."));
			Add(TEXT("Tundra Keeper"),
				TEXT("I kept a parish for people who like silence. You are loud in a round way. Allowed."),
				TEXT("Drift wolves. Hymn louder."),
				TEXT("Aurora Borough if you miss colors."));
			Add(TEXT("Confetti Baker"),
				TEXT("Carnival Bahia puts drums in the stuffing. I put stuffing in the drums. Same parade."),
				TEXT("Andes Loom is up. Wool dyed in weather."),
				TEXT("South Sewn lantern. Confetti is a valid government."));
			Add(TEXT("Wool Climber"),
				TEXT("The spine of the world wears a sweater now. I knitted the first inch."),
				TEXT("Don't mill this mountain. It would become a brochure."),
				TEXT("Carnival Bahia if you need noise."));
			Add(TEXT("Bell Warden"),
				TEXT("Clockhaven teatime bells. I rang one for your coup. The fog applauded on delay."),
				TEXT("Velvet Seine bakes blessings. Marble Forum wears tiny hats."),
				TEXT("Old World hops: Clockhaven, Seine, Forum."));
			Add(TEXT("Ribbon Baker"),
				TEXT("Strangers get pastry. You got a city. Fair trade."),
				TEXT("The mill asked for a logo. I gave them a bun with no face."),
				TEXT("Marble Forum columns. Sit. Convert. Repeat."));
			Add(TEXT("Column Sitter"),
				TEXT("I have sat on this column for nine years. I will sit converted. Growth."),
				TEXT("Hats for columns. Hats for bears. Hats for mill executives as they leave."),
				TEXT("Lantern home whenever the stone gets smug."));
			Add(TEXT("Grass Bell"),
				TEXT("Bells hung on acacia. Wind does hymns without a permit."),
				TEXT("Savannah does not need polyester. It has gold already."),
				TEXT("Sand Belt lanterns: Palma, Ivory, Hymn, Cape, here."));
			Add(TEXT("Delta Weaver"),
				TEXT("Thread enough to wrap a moon. We wrapped a factory rumor instead."),
				TEXT("Spice Harbor pepper docks if you like sneezing politically."),
				TEXT("Cherry Loom is the island that claps in pink."));
			Add(TEXT("Pepper Clerk"),
				TEXT("Saffron sails. I stamped 'handmade' on a crate of identical pepper. They fired me. I clapped."),
				TEXT("Mill scrap belongs in a museum of bad ideas."),
				TEXT("Silk Delta north. Hug the clerks."));
			Add(TEXT("Reef Choir"),
				TEXT("We already sang. You gave us lyrics that are just the word bear, repeatedly. Chart-topping."),
				TEXT("Cape Plush argues. We harmonize. Visit both."),
				TEXT("Lantern off the reef before you get soggy thoughts."));
			Add(TEXT("Ice Scarf"),
				TEXT("Last scarf on the last ice. I was saving it for a mayor. You can have the spare."),
				TEXT("Felt South is quiet so the coup echoes. Enjoy the acoustics."),
				TEXT("Home is a lantern away. The porch remembers you."));
			Add(TEXT("Default"),
				TEXT("I was being so serious. I'm not, now. Thank you, bear."),
				TEXT("Clap first. Policy later."),
				TEXT("Find a gold lantern. Tab, then E. The globe is small when you hop."));
			return Out;
		}();
		return All;
	}

	const FHolypawTalkDef* FindTalk(const FString& Speaker)
	{
		const FHolypawTalkDef* Found = GetTalks().FindByPredicate([&](const FHolypawTalkDef& T)
		{
			return T.Speaker.Equals(Speaker, ESearchCase::IgnoreCase);
		});
		if (Found)
		{
			return Found;
		}
		return GetTalks().FindByPredicate([](const FHolypawTalkDef& T) { return T.Speaker == TEXT("Default"); });
	}
}
