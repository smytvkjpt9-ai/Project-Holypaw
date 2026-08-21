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
				TEXT("If you pocketed the porch postcard, key 3 closes my missing-bear report. Key 4 takes the job."),
				TEXT("Lantern road east. Ribbon City is a buffet of opinions."));
			Add(TEXT("Skeptic"),
				TEXT("I had a five-year plan. It was rectangular. You made it round. I am... fine with that."),
				TEXT("If anyone asks, I converted for tax reasons. There are no taxes. There is a bear."),
				TEXT("Hug first, commute later. The city waits."));
			Add(TEXT("Child"),
				TEXT("Can the mill make a teddy as warm as you? I asked. The mill said no, then offered a coupon."),
				TEXT("I clapped so hard my mittens flew off. Key 4 if you have a stuffing bun. I will clap with crumbs."),
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
			Add(TEXT("Innkeep"),
				TEXT("Beds here remember the last person who was serious. We washed that out."),
				TEXT("Rest is a political act if the mill wants you exhausted and identical."),
				TEXT("E on the inn. HP full, cocoa, a save. Chapel is west of the fountain."));
			Add(TEXT("Choir Bear"),
				TEXT("I am not a bear. I am a person in a choir that aspires. Close enough."),
				TEXT("The chapel fills Miracle Charge. Bring the cellar hymn sheet and press 3. Dusk sings along."),
				TEXT("Cottage cellar for the sheet. Chapel gold roof. Then polyester."));
			Add(TEXT("Seamstress"),
				TEXT("I stitch hymn ribbons that the mill called inefficient. They were correct. I clapped."),
				TEXT("Eight AP, one ribbon. Faith you can tie around a paw."),
				TEXT("Cloth Loft west. Workshop cube. Bring leftover hugs."));
			Add(TEXT("Dock Poet"),
				TEXT("I wrote a sonnet about nets. Then you hugged the meter out of me. Improvement."),
				TEXT("Harbor crates sometimes hide mill scrap. Sometimes dust. Poetry either way."),
				TEXT("Harbor Steps east. Crate by the water. Tidewell further if you like salt."));
			Add(TEXT("Lamp Lighter"),
				TEXT("I light Quiet Rows so nobody has to be brave in the dark. You are already glowing."),
				TEXT("Night turns the fog blue. The mill stays beige on purpose."),
				TEXT("South of the plaza. Windows lit. Few hostiles on the street."));
			Add(TEXT("Spire Guard"),
				TEXT("I used to guard opinions. Now I guard a fountain that grants Faith if you toss a thought."),
				TEXT("The mayor converted. My job is ceremonial clapping. I am thriving."),
				TEXT("Plaza fountain. E to wish. Inn east, chapel west, mill rude and further east."));
			Add(TEXT("Mill Whistleblower"),
				TEXT("I filed a complaint that polyester has no soul. They offered a coupon. I took a hymn."),
				TEXT("Razor Petbots east of town. Bring a party. Steal their scrap. Key 3 if you have some."),
				TEXT("Poly Mill, grey sheds, red stack. Handmade not polyester — the sign is not subtle."));
			Add(TEXT("Plaza Florist"),
				TEXT("I sell roses that refuse to be identical. The mill asked for a bulk beige. I sent a thorn."),
				TEXT("Toss a thought in the fountain. Faith comes back smelling like wet stone and hope."),
				TEXT("Fountain wish is E. Inn is the warm cube. Chapel the tall gold one."));
			Add(TEXT("Night Watch"),
				TEXT("Quiet Rows after dusk. I watch windows, not opinions. Yours are already round."),
				TEXT("Night paints the fog blue. The mill stays beige on purpose. That is a crime of taste."),
				TEXT("South of the plaza. Lamp Lighter keeps the street honest."));
			Add(TEXT("Harbor Hand"),
				TEXT("Nets catch fish. You catch governments. Same wrist motion."),
				TEXT("Brine Warden walks the outer dock. He's very tall about it."),
				TEXT("Tidewell lantern. Then Cherry rumors if you're brave."));
			Add(TEXT("Net Weaver"),
				TEXT("I mended a hole and accidentally mended a worldview. Occupied hazard."),
				TEXT("A pearl rolled under Net Walk. Key 4 takes the job. Key 3 when it is round in your paw."),
				TEXT("Net Walk west of the lantern. Salt crabs pinch. Brine Warden is tall about it."));
			Add(TEXT("Ferry Clerk"),
				TEXT("I stamp tickets to nowhere because everywhere is now a hug. Efficient."),
				TEXT("Ferry crate sometimes hides mill scrap. Cherry rumors if you're brave."),
				TEXT("Slip east. Channel camp if the ocean argues."));
			Add(TEXT("Salt Priest"),
				TEXT("I baptize ribbons in brine. The mill baptizes nothing. That is the whole theology."),
				TEXT("Key 4. Bring a hymn ribbon from the Cloth Loft or a stall. I will dunk it."),
				TEXT("Salt Choir pool north of the lantern. Off-key on purpose."));
			Add(TEXT("Pearl Diver"),
				TEXT("I hold my breath for opinions. They float. Pearls do not."),
				TEXT("Net Walk hides one round thought. The Weaver wants it more than I do."),
				TEXT("West nets. Then clap at the font."));
			Add(TEXT("Hook Cook"),
				TEXT("I fry fish that were never identical. The mill asked for fish-shaped polyester. I threw a pan."),
				TEXT("Tide Inn heals. Brine font wishes. Eat something handmade."),
				TEXT("Outer Dock. Inn the warm cube. Brine Warden further out."));
			Add(TEXT("Farmer"),
				TEXT("Grain grew better after I stopped yelling at clouds. I yell at the mill now. Clouds approved."),
				TEXT("Wheat Choir dropped an ear. Key 4 takes the job. Key 3 when it's lumpy in your paw."),
				TEXT("Hearthfold lantern. Loaf Inn. Harvest Overseer north of the mill."));
			Add(TEXT("Miller"),
				TEXT("I ground identical flour for identical bread. You are lumpy. I am converted."),
				TEXT("Buy a bun. Be lumpy with me. The barn chapel fills Miracle Charge."),
				TEXT("Fields north. Scarecrows that forgot their job."));
			Add(TEXT("Thresh Singer"),
				TEXT("I sing to wheat until it agrees. You hug people until they agree. Union pending."),
				TEXT("Wheat Font wishes. Grain crate sometimes hides scrap. Don't mill this field."),
				TEXT("Wheat Choir south of the square. Farmer wants an ear."));
			Add(TEXT("Grain Widow"),
				TEXT("The mill offered identical condolences. I took a hymn instead."),
				TEXT("Hymn Barn is west. Rest at Loaf Inn. The overseer is a rude scarecrow with a clipboard."),
				TEXT("Barn Chapel. Then north if you like bosses."));
			Add(TEXT("Scarecrow Mender"),
				TEXT("I mend scarecrows so they can quit. Unemployment never looked so stuffed."),
				TEXT("Scarecrow Hounds still work. Unstuff them kindly."),
				TEXT("Scarecrow Row east. Hats gold, jobs cancelled."));
			Add(TEXT("Fen Guide"),
				TEXT("The peat remembers every boot. It will remember your paws as an improvement."),
				TEXT("Bog King sits south in the deepest jam. Don't go alone unless you like drowning cutely."),
				TEXT("Emberfen lantern. Peat Inn. Jam font. Frogs the color of cooling jam."));
			Add(TEXT("Mud Sculptor"),
				TEXT("I sculpted a bear before I met you. It was a sign. Or mud. Same thing."),
				TEXT("Witch Stoop hid a peat bead. Key 4. Key 3 when the jam has cooled."),
				TEXT("Stoop east. Chapel there. Fen witch-pets hiss in F-sharp."));
			Add(TEXT("Frog Bell"),
				TEXT("I ring a bell that sounds like a frog admitting it was serious. Improvement."),
				TEXT("Jam Pool is west. Toss a thought. Faith comes back sticky."),
				TEXT("Jam Font. Then south if the king is in a mood."));
			Add(TEXT("Peat Baker"),
				TEXT("I bake buns that remember the bog. The mill asked for beige. I threw peat."),
				TEXT("Peat Inn tucks you in. Crate on the leech board. Handmade, damp, correct."),
				TEXT("Peat Walk. Inn the warm cube. Bog King further south."));
			Add(TEXT("Jam Priest"),
				TEXT("I baptize nothing. I dunk hymns in jam. The mill cannot franchise that sentence."),
				TEXT("Hollow Choir claps wetly. Miracle at dusk still works in peat."),
				TEXT("South of the lantern. Wet clap. Then the king."));
			Add(TEXT("Snow Warden"),
				TEXT("I kept the ridge boring on purpose. You showed up glowing. My job is cancelled."),
				TEXT("Scarf Walk dropped aurora thread. Key 4. The mill cannot copy it. Key 3 when it glows."),
				TEXT("Snowveil lantern. Ridge Inn. Peak is up. Dress warm. You are already fur."));
			Add(TEXT("Scarf Knitter"),
				TEXT("I knit lights into wool. The mill knits nothing into beige. Rude."),
				TEXT("Thread on the walk. Warden files it as weather. Aurora Font wishes."),
				TEXT("Scarf Walk west. Poles, blue scarves, one round pickup."));
			Add(TEXT("Drift Priest"),
				TEXT("Ice Choir freezes clap mid-air. I call that punctuation."),
				TEXT("Ice Chapel fills Miracle Charge. Then Aurora Warden, Tyrant, Unmaker. Recipe."),
				TEXT("North of the gate. Chapel gold-on-white. Peak after."));
			Add(TEXT("Ridge Child"),
				TEXT("The lights copy you if you glow. I glowed. They copied. We are a choir now."),
				TEXT("Aurora Font is the stoop. Toss a thought. Then up."),
				TEXT("Stoop east. Lamp Notch south is the last warm cube."));
			Add(TEXT("Peak Acolyte"),
				TEXT("The shrine was empty because we were waiting for someone round."),
				TEXT("Miracle on the peak after the Unmaker. That's the last serious thought on Earth."),
				TEXT("Up. Always up. The rift likes company."));
			Add(TEXT("Studio Grip"),
				TEXT("Lantern Angeles shot a commercial for identical pets. I dropped the light on purpose."),
				TEXT("Gold posters lie. Key 4 takes Exhibit Glow. Key 3 when a marquee is in your paw."),
				TEXT("Marquee Walk. Lot Nine. Pacific camp between here and Mossgate."));
			Add(TEXT("Best Boy"),
				TEXT("I carry cables that used to power identical smiles. Now they power a bear."),
				TEXT("Lot Nine still has Razor Petbots between takes. Bring a party."),
				TEXT("Lamp Grid east. Inn the warm cube. Grip wants a poster."));
			Add(TEXT("Extra"),
				TEXT("I was hired to clap on cue. You hugged me off-script. Union pending."),
				TEXT("Extra Alley hides a crate. Sometimes mill scrap. Sometimes dust. Same take."),
				TEXT("Alley south of the marquee. Then hop the lantern if the lot gets beige."));
			Add(TEXT("Marquee Bear"),
				TEXT("I am not a bear. I am a person in a gold sign that aspires. Close enough."),
				TEXT("Poster Choir is the font. Toss a thought. Faith comes back smelling like popcorn theology."),
				TEXT("Font north. Chapel if you need Miracle Charge between commercials."));
			Add(TEXT("Fog Baker"),
				TEXT("Fog tastes like tea if you believe a bear. I believe a bear."),
				TEXT("Tea Walk hid a cup. Key 4. Key 3 when the rain has steeped."),
				TEXT("Mossgate lantern. Fog Inn. Quiltland further north woods."));
			Add(TEXT("Moss Warden"),
				TEXT("I kept the gate damp on purpose. Dry thoughts mill faster."),
				TEXT("Night Thread still works the rain. Unstuff it kindly."),
				TEXT("Fog Gate. Then Tea Walk west if you like cups that argue."));
			Add(TEXT("Tea Cart"),
				TEXT("I roll a cart of opinions that steep. The mill asked for beige bags. I threw moss."),
				TEXT("Fog tea is on the walk. Baker files it as weather. Rain Stoop wishes."),
				TEXT("Tea Walk. Cups, steam, one round pickup."));
			Add(TEXT("Gate Priest"),
				TEXT("I baptize nothing. I dunk hymns in fog. The mill cannot franchise humidity."),
				TEXT("Moss Choir claps wetly. Chapel fills Miracle Charge even when you cannot see it."),
				TEXT("Choir north of the gate. Gold roof in the tea."));
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
				TEXT("Clap Pier dropped a token. Key 4. Key 3 when it claps back in your paw."),
				TEXT("Palma Dusk lantern. Palm Inn. Two oceans argue politely at Cape Plush."));
			Add(TEXT("Dusk Usher"),
				TEXT("I seated identical tourists. Now I seat converted ones. Same chairs. Better plot."),
				TEXT("Usher Stoop is the font. Toss a thought. Dusk is ruder and better."),
				TEXT("Stoop east. Sand Notch south is the last warm cube before the dunes."));
			Add(TEXT("Clap Kid"),
				TEXT("The palms copy you if you glow. I glowed. They copied. We are a pier now."),
				TEXT("Palm Walk has trunks that refuse to be identical. The mill asked for a brochure. We clapped."),
				TEXT("Walk west. Then the pier if you like tokens."));
			Add(TEXT("Pier Priest"),
				TEXT("I dunk ribbons in dusk. Salt optional. Theology mandatory."),
				TEXT("Dusk Choir fills Miracle Charge. Salt crabs pinch. Bring a party anyway."),
				TEXT("Choir north. Chapel gold-on-mint. Cape Plush if the water argues."));
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
				TEXT("Loom Walk dropped a thread. Key 4. Key 3 when the wrap is in your paw. Globe Trek ends when it holds."),
				TEXT("Cherry Loom lantern. Blossom Inn. Silk Delta west, Spice Harbor southwest."));
			Add(TEXT("Cherry Priest"),
				TEXT("I baptize thread in pink. The mill baptizes nothing and calls it a brand."),
				TEXT("Pink Choir is the chapel. Miracle Charge. Then Void Rats if you like beige with teeth."),
				TEXT("Choir north of the gate. Gold roof, pink air."));
			Add(TEXT("Silk Child"),
				TEXT("I tied a blossom to a rumor. The rumor converted. I clapped."),
				TEXT("Thread Stoop wishes. Toss a thought. Faith comes back smelling like sap."),
				TEXT("Stoop east. Ferry Notch south hops you off the island."));
			Add(TEXT("Gate Watch"),
				TEXT("I watch blossoms so nobody mills them into a logo. You are already a logo. Allowed."),
				TEXT("Blossom Gate. Inn the warm cube. Crate on the notch. Handmade, pink, correct."),
				TEXT("Gate. Then Loom Walk if you like yarn that remembers fingers."));
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
