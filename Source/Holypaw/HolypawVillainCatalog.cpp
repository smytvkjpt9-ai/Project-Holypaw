#include "HolypawCatalog.h"

namespace HolypawCatalog
{
	static TArray<FVillainDef> BuildVillains()
	{
		TArray<FVillainDef> Out;
		auto Add = [&](EHolypawVillain Id, const TCHAR* Name, EHolypawZone Zone, EVillainRank Rank, EVillainShape Shape,
			EVillainSpecial Special, FLinearColor Color, FLinearColor Accent, int32 HP, int32 Atk, float Scale,
			float Aggro, float Respawn, bool bNoFlee, int32 AP, int32 FP, float Miracle,
			const TCHAR* Intro, const TCHAR* Attack, const TCHAR* Defeat, const TCHAR* Blurb)
		{
			FVillainDef D;
			D.Id = Id;
			D.DisplayName = FText::FromString(Name);
			D.HomeZone = Zone;
			D.Rank = Rank;
			D.Shape = Shape;
			D.Special = Special;
			D.Color = Color;
			D.AccentColor = Accent;
			D.HP = HP;
			D.Attack = Atk;
			D.Scale = Scale;
			D.AggroRange = Aggro;
			D.RespawnSeconds = Respawn;
			D.bBlocksFlee = bNoFlee;
			D.ApReward = AP;
			D.FpReward = FP;
			D.MiracleReward = Miracle;
			D.IntroLine = Intro;
			D.AttackLine = Attack;
			D.DefeatLine = Defeat;
			D.CodexBlurb = Blurb;
			Out.Add(D);
		};

		// Original browser-prototype four, now roaming Stuffed Park and the city roads.
		Add(EHolypawVillain::ScrapDog, TEXT("Scrap Dog"), EHolypawZone::ForestCottage, EVillainRank::Minion, EVillainShape::Cube,
			EVillainSpecial::Rip, FLinearColor(0.47f, 0.44f, 0.42f), FLinearColor(0.72f, 0.35f, 0.32f),
			28, 7, 1.f, 900.f, 25.f, false, 10, 5, 8.f,
			TEXT("Scrap Dog tries to rip the fluff apart!"),
			TEXT("chews seams"),
			TEXT("Scrap Dog unstuffs into a sad pile of buttons."),
			TEXT("Rust-thread mongrel of Stuffed Park. Chews lantern posts after dusk."));

		Add(EHolypawVillain::CorpCat, TEXT("Corp Cat"), EHolypawZone::NurseryHills, EVillainRank::Minion, EVillainShape::Sphere,
			EVillainSpecial::DrainFaith, FLinearColor(0.86f, 0.87f, 0.9f), FLinearColor(0.35f, 0.38f, 0.45f),
			32, 8, 1.f, 950.f, 25.f, false, 12, 6, 8.f,
			TEXT("Corp Cat clocks in. Its badge says DENY."),
			TEXT("stamps a denial"),
			TEXT("Corp Cat's clipboard snaps. Faith leaks back."),
			TEXT("Pale office feline. Drains Faith with paperwork claws."));

		Add(EHolypawVillain::RazorPetbot, TEXT("Razor Petbot"), EHolypawZone::RibbonCity, EVillainRank::Elite, EVillainShape::Cylinder,
			EVillainSpecial::DoubleStrike, FLinearColor(0.62f, 0.68f, 0.74f), FLinearColor(0.95f, 0.75f, 0.4f),
			40, 10, 1.12f, 1100.f, 32.f, false, 16, 8, 10.f,
			TEXT("Razor Petbot unfolds a second pair of shears."),
			TEXT("shears twice"),
			TEXT("Petbot powers down into polite tinsel."),
			TEXT("Clockwork groomer gone feral. Hits twice before you blink."));

		Add(EHolypawVillain::VoidRat, TEXT("Void Rat"), EHolypawZone::RibbonCity, EVillainRank::Minion, EVillainShape::Sphere,
			EVillainSpecial::StealMiracle, FLinearColor(0.22f, 0.2f, 0.21f), FLinearColor(0.55f, 0.35f, 0.7f),
			22, 9, 0.88f, 850.f, 22.f, false, 11, 7, 4.f,
			TEXT("Void Rat nibbles the miracle bar like cheese."),
			TEXT("gnaws the glow"),
			TEXT("Void Rat pops into a puff of lint."),
			TEXT("Shadow snack-thief. Steals Miracle Charge one nibble at a time."));

		Add(EHolypawVillain::NightThread, TEXT("Night Thread"), EHolypawZone::ForestCottage, EVillainRank::Minion, EVillainShape::Cone,
			EVillainSpecial::PoisonThread, FLinearColor(0.18f, 0.16f, 0.28f), FLinearColor(0.72f, 0.45f, 0.85f),
			24, 8, 0.95f, 880.f, 26.f, false, 11, 6, 7.f,
			TEXT("Night Thread unspools from the pines."),
			TEXT("laces venom-stitch"),
			TEXT("The thread snaps. Forest air sweetens."),
			TEXT("Living embroidery that poisons stuffing with night-dye."));

		Add(EHolypawVillain::StitchedWolf, TEXT("Stitched Wolf"), EHolypawZone::ForestCottage, EVillainRank::Elite, EVillainShape::Cube,
			EVillainSpecial::Rage, FLinearColor(0.38f, 0.32f, 0.36f), FLinearColor(0.92f, 0.55f, 0.5f),
			48, 12, 1.28f, 1200.f, 40.f, false, 18, 9, 12.f,
			TEXT("Stitched Wolf howls through a zipper mouth."),
			TEXT("tears a patch"),
			TEXT("The wolf's zipper smiles and goes slack."),
			TEXT("Park alpha. Rages twice as hard once half-unstuffed."));

		Add(EHolypawVillain::ParkProwler, TEXT("Park Prowler"), EHolypawZone::ForestCottage, EVillainRank::Minion, EVillainShape::Cube,
			EVillainSpecial::Rip, FLinearColor(0.42f, 0.48f, 0.4f), FLinearColor(0.55f, 0.35f, 0.28f),
			26, 7, 1.f, 920.f, 24.f, false, 10, 5, 7.f,
			TEXT("Park Prowler slinks between lanterns."),
			TEXT("pounces seams"),
			TEXT("Prowler rolls over, defeated and oddly cute."),
			TEXT("Scrap Dog cousin that learned to hunt the cottage path."));

		Add(EHolypawVillain::Tatterfox, TEXT("Tatterfox"), EHolypawZone::NurseryHills, EVillainRank::Elite, EVillainShape::Cone,
			EVillainSpecial::ThreadCut, FLinearColor(0.85f, 0.48f, 0.32f), FLinearColor(0.95f, 0.85f, 0.55f),
			36, 11, 1.08f, 1000.f, 30.f, false, 15, 8, 10.f,
			TEXT("Tatterfox grins. Party ribbons look tasty."),
			TEXT("snips the bond"),
			TEXT("Tatterfox leaves a polite apology note."),
			TEXT("Cuts Party Assault down to a lonely swipe."));

		Add(EHolypawVillain::AlleyScrapDog, TEXT("Alley Scrap Dog"), EHolypawZone::RibbonCity, EVillainRank::Minion, EVillainShape::Cube,
			EVillainSpecial::Rip, FLinearColor(0.4f, 0.38f, 0.4f), FLinearColor(0.8f, 0.45f, 0.55f),
			26, 7, 1.f, 900.f, 24.f, false, 10, 5, 7.f,
			TEXT("Alley Scrap Dog rattles a trash-ribbon lid."),
			TEXT("chews cobbles"),
			TEXT("The alley goes quiet except for distant hymns."),
			TEXT("City mutt. Same bite as the park, worse manners."));

		Add(EHolypawVillain::RibbonEnforcer, TEXT("Ribbon Enforcer"), EHolypawZone::RibbonCity, EVillainRank::Elite, EVillainShape::Cylinder,
			EVillainSpecial::ArmorPlates, FLinearColor(0.72f, 0.28f, 0.42f), FLinearColor(0.95f, 0.82f, 0.4f),
			52, 11, 1.22f, 1150.f, 40.f, false, 20, 10, 12.f,
			TEXT("Ribbon Enforcer buttons its brass vest."),
			TEXT("baton-hugs"),
			TEXT("The vest pops. Confetti of regulations."),
			TEXT("Armored city elite. Soft Slap barely dents the brass."));

		Add(EHolypawVillain::TinselGolem, TEXT("Tinsel Golem"), EHolypawZone::RibbonCity, EVillainRank::Elite, EVillainShape::Cube,
			EVillainSpecial::ArmorPlates, FLinearColor(0.92f, 0.78f, 0.45f), FLinearColor(0.85f, 0.55f, 0.7f),
			60, 9, 1.45f, 800.f, 45.f, true, 22, 10, 14.f,
			TEXT("Tinsel Golem blocks the lane like a parade float."),
			TEXT("crushes with glitter"),
			TEXT("Golem slumps into a gift pile."),
			TEXT("Slow, shiny, unfleeable. Festival leftover with fists."));

		Add(EHolypawVillain::GoldSnipper, TEXT("Gold Snipper"), EHolypawZone::RibbonCity, EVillainRank::Minion, EVillainShape::Cone,
			EVillainSpecial::DrainFaith, FLinearColor(0.95f, 0.78f, 0.28f), FLinearColor(0.55f, 0.4f, 0.2f),
			30, 9, 0.92f, 980.f, 26.f, false, 13, 8, 6.f,
			TEXT("Gold Snipper invoices you for existing."),
			TEXT("clips a tithe"),
			TEXT("The snips rust. Charity wins."),
			TEXT("Tiny market pest that bills Faith by the inch."));

		Add(EHolypawVillain::PlazaCorpCat, TEXT("Plaza Corp Cat"), EHolypawZone::RibbonCity, EVillainRank::Minion, EVillainShape::Sphere,
			EVillainSpecial::DrainFaith, FLinearColor(0.9f, 0.9f, 0.93f), FLinearColor(0.78f, 0.32f, 0.45f),
			34, 8, 1.05f, 960.f, 26.f, false, 12, 7, 8.f,
			TEXT("Plaza Corp Cat files a complaint in triplicate."),
			TEXT("stamps harder"),
			TEXT("The complaint is denied — by you."),
			TEXT("Downtown cousin of Corp Cat. Loves the mayor's steps."));

		Add(EHolypawVillain::SewerVoidRat, TEXT("Sewer Void Rat"), EHolypawZone::RibbonCity, EVillainRank::Minion, EVillainShape::Sphere,
			EVillainSpecial::StealMiracle, FLinearColor(0.16f, 0.2f, 0.22f), FLinearColor(0.4f, 0.7f, 0.55f),
			20, 10, 0.82f, 820.f, 20.f, false, 12, 8, 3.f,
			TEXT("Sewer Void Rat surfaces smelling of lost miracles."),
			TEXT("drains the halo"),
			TEXT("It slips back into a grate, empty-pawed."),
			TEXT("Under-plaza rat. Hungrier for Miracle Charge than cheese."));

		Add(EHolypawVillain::SilkMagistrate, TEXT("Silk Magistrate"), EHolypawZone::RibbonCity, EVillainRank::Boss, EVillainShape::Cylinder,
			EVillainSpecial::FaithBurn, FLinearColor(0.55f, 0.22f, 0.38f), FLinearColor(0.98f, 0.86f, 0.5f),
			96, 14, 1.7f, 1500.f, 90.f, true, 40, 22, 20.f,
			TEXT("Silk Magistrate raises a gavel of compressed bows."),
			TEXT("sentences the stuffing"),
			TEXT("The court adjourns. Ribbon City exhales."),
			TEXT("Ribbon City boss. Burns Faith as damage. No fleeing the bench."));

		Add(EHolypawVillain::SaltCrab, TEXT("Salt Crab"), EHolypawZone::Tidewell, EVillainRank::Minion, EVillainShape::Cylinder,
			EVillainSpecial::ArmorPlates, FLinearColor(0.85f, 0.55f, 0.48f), FLinearColor(0.95f, 0.9f, 0.82f),
			30, 8, 1.f, 880.f, 26.f, false, 11, 6, 8.f,
			TEXT("Salt Crab clicks like a harbor clock."),
			TEXT("pinches stuffing"),
			TEXT("The shell becomes a cute soup bowl. Empty."),
			TEXT("Tidewell shoreline pest. Shell shrugs off slaps."));

		Add(EHolypawVillain::HarborHook, TEXT("Harbor Hook"), EHolypawZone::Tidewell, EVillainRank::Elite, EVillainShape::Cone,
			EVillainSpecial::ThreadCut, FLinearColor(0.35f, 0.42f, 0.5f), FLinearColor(0.7f, 0.45f, 0.35f),
			44, 12, 1.18f, 1050.f, 34.f, false, 17, 8, 11.f,
			TEXT("Harbor Hook snags a party leash."),
			TEXT("yanks the line"),
			TEXT("The hook rusts into a lucky charm."),
			TEXT("Dock elite. Reels party members out of formation."));

		Add(EHolypawVillain::BrineGull, TEXT("Brine Gull"), EHolypawZone::Tidewell, EVillainRank::Minion, EVillainShape::Cone,
			EVillainSpecial::StealMiracle, FLinearColor(0.88f, 0.9f, 0.92f), FLinearColor(0.95f, 0.72f, 0.35f),
			24, 9, 0.9f, 1100.f, 22.f, false, 12, 7, 5.f,
			TEXT("Brine Gull steals the shine right off your halo."),
			TEXT("snatches glow"),
			TEXT("Gull drops a polite fish-shaped apology."),
			TEXT("Dive-bombs Miracle Charge, then your sandwich."));

		Add(EHolypawVillain::DockRat, TEXT("Dock Rat"), EHolypawZone::Tidewell, EVillainRank::Minion, EVillainShape::Sphere,
			EVillainSpecial::Rip, FLinearColor(0.4f, 0.36f, 0.34f), FLinearColor(0.45f, 0.62f, 0.7f),
			22, 8, 0.85f, 840.f, 22.f, false, 10, 5, 6.f,
			TEXT("Dock Rat scuttles under the planks."),
			TEXT("nibbles rope"),
			TEXT("It tips an invisible cap and faints."),
			TEXT("Wet cousin of Void Rat. Less void, more barnacle."));

		Add(EHolypawVillain::BrineWarden, TEXT("Brine Warden"), EHolypawZone::Tidewell, EVillainRank::Boss, EVillainShape::Cylinder,
			EVillainSpecial::DoubleStrike, FLinearColor(0.22f, 0.38f, 0.52f), FLinearColor(0.55f, 0.85f, 0.8f),
			100, 15, 1.75f, 1550.f, 95.f, true, 42, 24, 22.f,
			TEXT("Brine Warden rises from the tide with a net of hymns."),
			TEXT("lashes twice with kelp"),
			TEXT("The harbor bells ring for you, not it."),
			TEXT("Tidewell boss. Two hits, no flee, salt in every seam."));

		Add(EHolypawVillain::ScarecrowHound, TEXT("Scarecrow Hound"), EHolypawZone::Hearthfold, EVillainRank::Minion, EVillainShape::Cone,
			EVillainSpecial::Rip, FLinearColor(0.62f, 0.52f, 0.32f), FLinearColor(0.35f, 0.45f, 0.28f),
			34, 9, 1.05f, 950.f, 28.f, false, 12, 6, 8.f,
			TEXT("Scarecrow Hound's straw bristles like fur."),
			TEXT("snaps straw-jaws"),
			TEXT("It becomes a perfectly normal scarecrow. Almost."),
			TEXT("Farm-path guardian that forgot which side it was on."));

		Add(EHolypawVillain::HaywireScarecrow, TEXT("Haywire Scarecrow"), EHolypawZone::Hearthfold, EVillainRank::Elite, EVillainShape::Cube,
			EVillainSpecial::Rage, FLinearColor(0.7f, 0.42f, 0.28f), FLinearColor(0.9f, 0.85f, 0.4f),
			50, 12, 1.32f, 1180.f, 40.f, false, 19, 9, 12.f,
			TEXT("Haywire Scarecrow's grin stitches itself wider."),
			TEXT("flails harvest-hooks"),
			TEXT("Straw rains. Crows applaud the wrong team."),
			TEXT("Broken field elite. Rages when the stuffing shows."));

		Add(EHolypawVillain::ThreshCat, TEXT("Thresh Cat"), EHolypawZone::Hearthfold, EVillainRank::Minion, EVillainShape::Sphere,
			EVillainSpecial::DoubleStrike, FLinearColor(0.55f, 0.48f, 0.35f), FLinearColor(0.92f, 0.62f, 0.3f),
			28, 8, 0.98f, 900.f, 26.f, false, 12, 6, 8.f,
			TEXT("Thresh Cat bats twice — work ethic."),
			TEXT("scythe-paws"),
			TEXT("It kneads a wheat sheaf and naps."),
			TEXT("Barn cat with sickle toes. Double swat."));

		Add(EHolypawVillain::GrainMite, TEXT("Grain Mite"), EHolypawZone::Hearthfold, EVillainRank::Minion, EVillainShape::Sphere,
			EVillainSpecial::PoisonThread, FLinearColor(0.78f, 0.72f, 0.42f), FLinearColor(0.45f, 0.35f, 0.2f),
			18, 7, 0.7f, 700.f, 18.f, false, 9, 5, 5.f,
			TEXT("Grain Mite dusts the air with sneeze-gold."),
			TEXT("itches the seams"),
			TEXT("One sneeze later, it is history."),
			TEXT("Tiny mill pest. Poison itch more than bite."));

		Add(EHolypawVillain::HarvestOverseer, TEXT("Harvest Overseer"), EHolypawZone::Hearthfold, EVillainRank::Boss, EVillainShape::Cube,
			EVillainSpecial::ThreadCut, FLinearColor(0.55f, 0.32f, 0.18f), FLinearColor(0.95f, 0.78f, 0.35f),
			108, 15, 1.8f, 1600.f, 100.f, true, 44, 24, 22.f,
			TEXT("Harvest Overseer plants a pitchfork like a throne."),
			TEXT("reaps the party"),
			TEXT("The fields bow. Hearthfold belongs to fluff again."),
			TEXT("Hearthfold boss. Reaps party bonds. Does not permit fleeing."));

		Add(EHolypawVillain::MireLurker, TEXT("Mire Lurker"), EHolypawZone::Emberfen, EVillainRank::Minion, EVillainShape::Cube,
			EVillainSpecial::PoisonThread, FLinearColor(0.32f, 0.28f, 0.22f), FLinearColor(0.72f, 0.28f, 0.25f),
			38, 11, 1.15f, 1000.f, 30.f, false, 14, 7, 9.f,
			TEXT("Mire Lurker peels itself off the mud."),
			TEXT("smears toxin-mud"),
			TEXT("The lurker sinks, politely this time."),
			TEXT("Crimson Hollow ambusher. Poisoned peat in every hug."));

		Add(EHolypawVillain::FenWitchPet, TEXT("Fen Witch-Pet"), EHolypawZone::Emberfen, EVillainRank::Elite, EVillainShape::Cone,
			EVillainSpecial::FaithBurn, FLinearColor(0.42f, 0.22f, 0.38f), FLinearColor(0.95f, 0.45f, 0.35f),
			46, 12, 1.2f, 1120.f, 38.f, false, 18, 11, 12.f,
			TEXT("Fen Witch-Pet recites a hymn backwards."),
			TEXT("burns borrowed faith"),
			TEXT("The hex unravels into fireflies."),
			TEXT("Familiar of the fen. Turns your Faith into a weapon against you."));

		Add(EHolypawVillain::BogLeech, TEXT("Bog Leech"), EHolypawZone::Emberfen, EVillainRank::Minion, EVillainShape::Cylinder,
			EVillainSpecial::DrainFaith, FLinearColor(0.28f, 0.18f, 0.2f), FLinearColor(0.55f, 0.22f, 0.28f),
			26, 9, 0.95f, 860.f, 24.f, false, 12, 8, 6.f,
			TEXT("Bog Leech latches onto a miracle vein."),
			TEXT("sips FP"),
			TEXT("It lets go, embarrassed and full."),
			TEXT("Drinks Faith like pondwater. Bring extra FP."));

		Add(EHolypawVillain::EmberToad, TEXT("Ember Toad"), EHolypawZone::Emberfen, EVillainRank::Minion, EVillainShape::Sphere,
			EVillainSpecial::Rage, FLinearColor(0.72f, 0.32f, 0.22f), FLinearColor(0.95f, 0.55f, 0.28f),
			32, 10, 1.1f, 900.f, 28.f, false, 13, 7, 9.f,
			TEXT("Ember Toad puffs until the stitches glow."),
			TEXT("belches cinder-fluff"),
			TEXT("The toad cools into a charming paperweight."),
			TEXT("Warm-bellied fen beast. Angrier the puffier it gets."));

		Add(EHolypawVillain::BogKing, TEXT("Bog King"), EHolypawZone::Emberfen, EVillainRank::Boss, EVillainShape::Cube,
			EVillainSpecial::PoisonThread, FLinearColor(0.22f, 0.14f, 0.12f), FLinearColor(0.85f, 0.25f, 0.22f),
			112, 16, 1.85f, 1650.f, 105.f, true, 46, 26, 24.f,
			TEXT("Bog King crowns itself with a wreath of drowned ribbons."),
			TEXT("anoints with venom-peat"),
			TEXT("The crown sinks. Emberfen breathes."),
			TEXT("Emberfen boss. Poison throne, no escape, very sticky."));

		Add(EHolypawVillain::FrostMoth, TEXT("Frost Moth"), EHolypawZone::Snowveil, EVillainRank::Minion, EVillainShape::Cone,
			EVillainSpecial::FrostBite, FLinearColor(0.82f, 0.9f, 0.98f), FLinearColor(0.55f, 0.7f, 0.95f),
			26, 10, 1.f, 1000.f, 26.f, false, 13, 7, 8.f,
			TEXT("Frost Moth dusts the air with quiet ice."),
			TEXT("numbs a seam"),
			TEXT("Wings melt into snow-sugar."),
			TEXT("Alpine flutterer. Frostbite makes stuffing slow and sore."));

		Add(EHolypawVillain::IceShardCat, TEXT("Ice Shard Cat"), EHolypawZone::Snowveil, EVillainRank::Elite, EVillainShape::Sphere,
			EVillainSpecial::FrostBite, FLinearColor(0.7f, 0.82f, 0.95f), FLinearColor(0.95f, 0.95f, 1.f),
			42, 12, 1.15f, 1100.f, 36.f, false, 18, 9, 12.f,
			TEXT("Ice Shard Cat's fur rings like glass."),
			TEXT("rakes with icicles"),
			TEXT("It sheds a single perfect snowflake and yields."),
			TEXT("Snowveil elite. Cold claws, colder stare."));

		Add(EHolypawVillain::DriftWolf, TEXT("Drift Wolf"), EHolypawZone::Snow, EVillainRank::Elite, EVillainShape::Cube,
			EVillainSpecial::Rage, FLinearColor(0.78f, 0.84f, 0.92f), FLinearColor(0.45f, 0.55f, 0.7f),
			54, 13, 1.3f, 1250.f, 42.f, false, 20, 10, 13.f,
			TEXT("Drift Wolf steps out of a whiteout."),
			TEXT("hammers with a snow-shoulder"),
			TEXT("The blizzard sits down and behaves."),
			TEXT("Ridge pack hunter. Rages in the last half of the fight."));

		Add(EHolypawVillain::AuroraWisp, TEXT("Aurora Wisp"), EHolypawZone::Snowveil, EVillainRank::Minion, EVillainShape::Sphere,
			EVillainSpecial::StealMiracle, FLinearColor(0.45f, 0.85f, 0.75f), FLinearColor(0.72f, 0.55f, 0.95f),
			20, 8, 0.78f, 1200.f, 20.f, false, 14, 8, 4.f,
			TEXT("Aurora Wisp sips color from your halo."),
			TEXT("drinks the lights"),
			TEXT("The lights return to the sky, embarrassed."),
			TEXT("Pretty, rude, and obsessed with Miracle Charge."));

		Add(EHolypawVillain::AuroraWarden, TEXT("Aurora Warden"), EHolypawZone::Snowveil, EVillainRank::Boss, EVillainShape::Cylinder,
			EVillainSpecial::FrostBite, FLinearColor(0.35f, 0.55f, 0.85f), FLinearColor(0.85f, 0.7f, 1.f),
			104, 15, 1.72f, 1580.f, 100.f, true, 44, 24, 22.f,
			TEXT("Aurora Warden hangs the northern lights like a curtain."),
			TEXT("seals with frost-law"),
			TEXT("The curtain parts. Snowveil is a hamlet again."),
			TEXT("Snowveil boss. Frost law, locked gates, no fleeing the lights."));

		Add(EHolypawVillain::VelvetTyrant, TEXT("Velvet Tyrant"), EHolypawZone::Highlands, EVillainRank::WorldBoss, EVillainShape::Cube,
			EVillainSpecial::Rage, FLinearColor(0.42f, 0.22f, 0.48f), FLinearColor(0.95f, 0.75f, 0.4f),
			88, 16, 1.85f, 1700.f, 140.f, true, 55, 30, 28.f,
			TEXT("Velvet Tyrant unfurls a cape of stolen plush."),
			TEXT("rends with royal claws"),
			TEXT("The peak is sacred again. The cape becomes a blanket."),
			TEXT("World boss of Velvet Peak. Rages, forbids flee, hoards fluff."));

		Add(EHolypawVillain::Unmaker, TEXT("The Unmaker"), EHolypawZone::Highlands, EVillainRank::WorldBoss, EVillainShape::Cone,
			EVillainSpecial::FaithBurn, FLinearColor(0.08f, 0.07f, 0.1f), FLinearColor(0.95f, 0.35f, 0.55f),
			140, 18, 2.1f, 1900.f, 180.f, true, 80, 40, 35.f,
			TEXT("The Unmaker arrives to un-sew the world."),
			TEXT("unmakes a stitch of you"),
			TEXT("The needle stills. Everything stays stuffed."),
			TEXT("Final peak horror. Burns Faith, unmakes seams, never lets go."));

		Add(EHolypawVillain::MiracleEater, TEXT("Miracle Eater"), EHolypawZone::NurseryHills, EVillainRank::Elite, EVillainShape::Sphere,
			EVillainSpecial::StealMiracle, FLinearColor(0.55f, 0.35f, 0.62f), FLinearColor(1.f, 0.85f, 0.45f),
			36, 10, 1.08f, 1300.f, 35.f, false, 20, 6, 2.f,
			TEXT("Miracle Eater opens a polite, endless mouth."),
			TEXT("swallows charge"),
			TEXT("It hiccups a blessing back, by accident."),
			TEXT("Road rare. Lives on Miracle Charge. Leave some for later."));

		Add(EHolypawVillain::ButtonThief, TEXT("Button Thief"), EHolypawZone::NurseryHills, EVillainRank::Minion, EVillainShape::Sphere,
			EVillainSpecial::DrainFaith, FLinearColor(0.35f, 0.45f, 0.7f), FLinearColor(0.95f, 0.8f, 0.45f),
			18, 6, 0.72f, 780.f, 18.f, false, 14, 9, 6.f,
			TEXT("Button Thief jingles like a guilty pocket."),
			TEXT("picks a faith-button"),
			TEXT("Buttons bounce everywhere. You keep the gold ones."),
			TEXT("Pickpocket of FP. Small, fast, fabulous at crime."));

		Add(EHolypawVillain::UnstuffedShade, TEXT("Unstuffed Shade"), EHolypawZone::Highlands, EVillainRank::Elite, EVillainShape::Cone,
			EVillainSpecial::FaithBurn, FLinearColor(0.25f, 0.22f, 0.3f), FLinearColor(0.7f, 0.55f, 0.85f),
			40, 13, 1.15f, 1400.f, 40.f, false, 18, 10, 10.f,
			TEXT("Unstuffed Shade remembers being a toy."),
			TEXT("hollows a memory"),
			TEXT("The shade folds into a spare blanket."),
			TEXT("Peak remnant of failed hugs. Hurts more if you carry Faith."));

		const TArray<EHolypawVillain> Poly = {
			EHolypawVillain::CorpCat, EHolypawVillain::RazorPetbot, EHolypawVillain::PlazaCorpCat,
			EHolypawVillain::RibbonEnforcer, EHolypawVillain::TinselGolem, EHolypawVillain::GoldSnipper,
			EHolypawVillain::SilkMagistrate, EHolypawVillain::MiracleEater, EHolypawVillain::Unmaker
		};
		for (FVillainDef& D : Out)
		{
			if (Poly.Contains(D.Id))
			{
				D.Faction = EHolypawFaction::PolyMill;
			}
		}

		return Out;
	}

	const TArray<FVillainDef>& GetVillains()
	{
		static const TArray<FVillainDef> All = BuildVillains();
		return All;
	}

	FVillainDef GetVillain(EHolypawVillain Id)
	{
		for (const FVillainDef& D : GetVillains())
		{
			if (D.Id == Id)
			{
				return D;
			}
		}
		return GetVillains()[0];
	}

	FString SpecialLabel(EVillainSpecial Special)
	{
		switch (Special)
		{
		case EVillainSpecial::Rip: return TEXT("Rip");
		case EVillainSpecial::DrainFaith: return TEXT("Drain Faith");
		case EVillainSpecial::StealMiracle: return TEXT("Eat Miracle");
		case EVillainSpecial::DoubleStrike: return TEXT("Double Strike");
		case EVillainSpecial::ArmorPlates: return TEXT("Armor Plates");
		case EVillainSpecial::FrostBite: return TEXT("Frostbite");
		case EVillainSpecial::PoisonThread: return TEXT("Poison Thread");
		case EVillainSpecial::Rage: return TEXT("Rage");
		case EVillainSpecial::ThreadCut: return TEXT("Cut Party");
		case EVillainSpecial::FaithBurn: return TEXT("Faith Burn");
		default: return TEXT("Rip");
		}
	}

	FString RankLabel(EVillainRank Rank)
	{
		switch (Rank)
		{
		case EVillainRank::Minion: return TEXT("Minion");
		case EVillainRank::Elite: return TEXT("Elite");
		case EVillainRank::Boss: return TEXT("Boss");
		case EVillainRank::WorldBoss: return TEXT("World Boss");
		default: return TEXT("Minion");
		}
	}
}
