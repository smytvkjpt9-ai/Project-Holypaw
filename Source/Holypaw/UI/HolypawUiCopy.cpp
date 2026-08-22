#include "UI/HolypawUiCopy.h"

namespace HolypawUiCopy
{
	FText GameTitle() { return NSLOCTEXT("HolypawUI", "GameTitle", "The Fluffy Ascendancy"); }
	FText Tagline() { return NSLOCTEXT("HolypawUI", "Tagline", "A handmade teddy. A polyester empire. A hug coup."); }
	FText SlotHint() { return NSLOCTEXT("HolypawUI", "SlotHint", "1-3 pick a porch   Enter continue   N new coup   L load"); }
	FText TitleMuteOn() { return NSLOCTEXT("HolypawUI", "MuteOn", "Mute on"); }
	FText TitleMuteOff() { return NSLOCTEXT("HolypawUI", "MuteOff", "Mute off"); }
	FText EmptyPorch() { return NSLOCTEXT("HolypawUI", "EmptyPorch", "empty porch — start a coup"); }
	FText SlotN(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "SlotN", "Slot {0}"), FText::AsNumber(N)); }

	FText Paused() { return NSLOCTEXT("HolypawUI", "Paused", "Paused"); }
	FText PauseBlurb() { return NSLOCTEXT("HolypawUI", "PauseBlurb", "The humans freeze. They were mid-opinion."); }
	FText Resume() { return NSLOCTEXT("HolypawUI", "Resume", "Resume"); }
	FText SaveSlot() { return NSLOCTEXT("HolypawUI", "SaveSlot", "Save this slot"); }
	FText Mute() { return NSLOCTEXT("HolypawUI", "Mute", "Mute"); }
	FText BackToTitle() { return NSLOCTEXT("HolypawUI", "BackToTitle", "Title (progress stays saved)"); }

	FText MapTitle() { return NSLOCTEXT("HolypawUI", "MapTitle", "Plush Earth"); }
	FText MapBlurb() { return NSLOCTEXT("HolypawUI", "MapBlurb", "Hearts warm a city. Lanterns hop. N closes."); }
	FText MapHere() { return NSLOCTEXT("HolypawUI", "MapHere", "You are here"); }
	FText MapHop() { return NSLOCTEXT("HolypawUI", "MapHop", "Tab cycle lantern   E hop"); }
	FText MapHearts() { return NSLOCTEXT("HolypawUI", "MapHearts", "City Hearts"); }
	FText MapUnlit() { return NSLOCTEXT("HolypawUI", "MapUnlit", "Unlit — walk there first"); }
	FText HomePin() { return NSLOCTEXT("HolypawUI", "HomePin", "Home"); }
	FText PeakPin() { return NSLOCTEXT("HolypawUI", "PeakPin", "Peak"); }
	FText Lanterns() { return NSLOCTEXT("HolypawUI", "Lanterns", "Lanterns"); }

	FText JournalTitle() { return NSLOCTEXT("HolypawUI", "JournalTitle", "Bear Faith Journal"); }
	FText Campaign() { return NSLOCTEXT("HolypawUI", "Campaign", "Campaign"); }
	FText Errands() { return NSLOCTEXT("HolypawUI", "Errands", "Errands"); }
	FText Current() { return NSLOCTEXT("HolypawUI", "Current", "Now"); }
	FText JournalClose() { return NSLOCTEXT("HolypawUI", "JournalClose", "J or Esc closes the book"); }
	FText RecruitsN(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "RecruitsN", "Recruits {0}"), FText::AsNumber(N)); }
	FText MiraclesN(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "MiraclesN", "Miracles {0}"), FText::AsNumber(N)); }
	FText CitiesN(int32 Have, int32 Total)
	{
		return FText::Format(NSLOCTEXT("HolypawUI", "CitiesN", "Cities {0}/{1}"), FText::AsNumber(Have), FText::AsNumber(Total));
	}
	FText JournalEmptyErrands() { return NSLOCTEXT("HolypawUI", "JournalEmptyErrands", "Talk 4 takes a job. Talk 3 turns it in."); }

	FText TalkTitle() { return NSLOCTEXT("HolypawUI", "TalkTitle", "Testimony"); }
	FText Listen() { return NSLOCTEXT("HolypawUI", "Listen", "Listen"); }
	FText AskWay() { return NSLOCTEXT("HolypawUI", "AskWay", "Ask the way"); }
	FText TurnIn() { return NSLOCTEXT("HolypawUI", "TurnIn", "Turn in"); }
	FText TakeJob() { return NSLOCTEXT("HolypawUI", "TakeJob", "Take a job"); }
	FText QuietLine() { return NSLOCTEXT("HolypawUI", "QuietLine", "A quieter thought waits if Faith is high."); }
	FText QuietAt(int32 Faith)
	{
		return FText::Format(NSLOCTEXT("HolypawUI", "QuietAt", "Quiet line at {0} FP."), FText::AsNumber(Faith));
	}
	FText ErrandDone() { return NSLOCTEXT("HolypawUI", "ErrandDone", "Errand clapped."); }
	FText JobReady() { return NSLOCTEXT("HolypawUI", "JobReady", "Job"); }
	FText Believer() { return NSLOCTEXT("HolypawUI", "Believer", "Believer"); }

	FText ShopTitle() { return NSLOCTEXT("HolypawUI", "ShopTitle", "Faith stall"); }
	FText ShopDiscount() { return NSLOCTEXT("HolypawUI", "ShopDiscount", "Hearts −2 AP"); }
	FText ShopFullPrice() { return NSLOCTEXT("HolypawUI", "ShopFullPrice", "No Hearts yet"); }
	FText Continue() { return NSLOCTEXT("HolypawUI", "Continue", "Continue"); }
	FText NewCoup() { return NSLOCTEXT("HolypawUI", "NewCoup", "New coup"); }
	FText LoadSlot() { return NSLOCTEXT("HolypawUI", "LoadSlot", "Load"); }
	FText AudioOn() { return NSLOCTEXT("HolypawUI", "AudioOn", "Audio on"); }
	FText HomeSewn() { return NSLOCTEXT("HolypawUI", "HomeSewn", "Cottage porch"); }
	FText HeatCold() { return NSLOCTEXT("HolypawUI", "HeatCold", "cold"); }
	FText HeatWarm() { return NSLOCTEXT("HolypawUI", "HeatWarm", "warm"); }
	FText HeatGlow() { return NSLOCTEXT("HolypawUI", "HeatGlow", "glowing"); }
	FText CodexTitle() { return NSLOCTEXT("HolypawUI", "CodexTitle", "Villain Codex"); }
	FText CodexUnseen() { return NSLOCTEXT("HolypawUI", "CodexUnseen", "Not yet met"); }
	FText CodexUnknown() { return NSLOCTEXT("HolypawUI", "CodexUnknown", "Unknown"); }
	FText CodexHunting() { return NSLOCTEXT("HolypawUI", "CodexHunting", "Hunting"); }
	FText CodexFell() { return NSLOCTEXT("HolypawUI", "CodexFell", "Fell"); }
	FText SeenN(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "SeenN", "Seen {0}"), FText::AsNumber(N)); }
	FText FellN(int32 Have, int32 Total)
	{
		return FText::Format(NSLOCTEXT("HolypawUI", "FellN", "Fell {0} / {1}"), FText::AsNumber(Have), FText::AsNumber(Total));
	}
	FText TabHint() { return NSLOCTEXT("HolypawUI", "TabHint", "Tab"); }
	FText Owned() { return NSLOCTEXT("HolypawUI", "Owned", "owned"); }
	FText Buy() { return NSLOCTEXT("HolypawUI", "Buy", "buy"); }
	FText FaithJar() { return NSLOCTEXT("HolypawUI", "FaithJar", "Faith jar"); }
	FText FaithJarBlurb() { return NSLOCTEXT("HolypawUI", "FaithJarBlurb", "Leftover hugs, bottled. +8 FP."); }
	FText ShopFooter() { return NSLOCTEXT("HolypawUI", "ShopFooter", "1-4 buy   Esc close   I pockets"); }
	FText ApN(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "ApN", "AP {0}"), FText::AsNumber(N)); }
	FText PriceAp(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "PriceAp", "{0} AP"), FText::AsNumber(N)); }
	FText PriceDrop(int32 From, int32 To)
	{
		return FText::Format(NSLOCTEXT("HolypawUI", "PriceDrop", "{0} → {1} AP"), FText::AsNumber(From), FText::AsNumber(To));
	}
	FText NeedAp(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "NeedAp", "need {0} more"), FText::AsNumber(N)); }

	FText BattleYou() { return NSLOCTEXT("HolypawUI", "BattleYou", "You"); }
	FText BattleVs() { return NSLOCTEXT("HolypawUI", "BattleVs", "VS"); }
	FText PageBasics() { return NSLOCTEXT("HolypawUI", "PageBasics", "Basics"); }
	FText PageOverflow() { return NSLOCTEXT("HolypawUI", "PageOverflow", "Overflow"); }
	FText PageParty() { return NSLOCTEXT("HolypawUI", "PageParty", "Party tricks"); }
	FText PhaseTwo() { return NSLOCTEXT("HolypawUI", "PhaseTwo", "Phase two"); }
	FText PolyMill() { return NSLOCTEXT("HolypawUI", "PolyMill", "Poly Mill — cheap polyester"); }
	FText PolyMillTag() { return NSLOCTEXT("HolypawUI", "PolyMillTag", "Poly Mill"); }
	FText Hostile() { return NSLOCTEXT("HolypawUI", "Hostile", "Hostile"); }
	FText FpCost(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "FpCost", "{0} FP"), FText::AsNumber(N)); }
	FText StitchPlus(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "StitchPlus", "+{0} stitch"), FText::AsNumber(N)); }
	FText PartyTitle() { return NSLOCTEXT("HolypawUI", "PartyTitle", "Fluffy Party"); }
	FText PartyEmpty() { return NSLOCTEXT("HolypawUI", "PartyEmpty", "Empty. Recruit wild fluffies with E."); }
	FText PocketsTitle() { return NSLOCTEXT("HolypawUI", "PocketsTitle", "Pockets"); }
	FText PocketsEmpty() { return NSLOCTEXT("HolypawUI", "PocketsEmpty", "Empty. Stalls sell buns. Mills drop scrap."); }
	FText AtkN(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "AtkN", "ATK {0}"), FText::AsNumber(N)); }
	FText CountX(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "CountX", "x{0}"), FText::AsNumber(N)); }
	FText HpFrac(int32 Have, int32 Max)
	{
		return FText::Format(NSLOCTEXT("HolypawUI", "HpFrac", "{0}/{1}"), FText::AsNumber(Have), FText::AsNumber(Max));
	}

	FText PlayTitle() { return NSLOCTEXT("HolypawUI", "PlayTitle", "Fluffy Ascendancy"); }
	FText Miracle() { return NSLOCTEXT("HolypawUI", "Miracle", "Miracle"); }
	FText Hearts() { return NSLOCTEXT("HolypawUI", "Hearts", "Hearts"); }
	FText HeartsN(int32 N) { return FText::Format(NSLOCTEXT("HolypawUI", "HeartsN", "Hearts {0}"), FText::AsNumber(N)); }
	FText CityHearts() { return NSLOCTEXT("HolypawUI", "CityHearts", "City"); }
	FText Legend() { return NSLOCTEXT("HolypawUI", "Legend", "E  I pockets  K trees  J journal  P party  M miracle  N map  V  F5  Esc"); }
}
