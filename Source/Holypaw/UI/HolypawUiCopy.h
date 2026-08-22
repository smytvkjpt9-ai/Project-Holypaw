#pragma once

#include "CoreMinimal.h"

/** Loc keys for the product UI surface. Widgets never invent leftover HUD strings. */
namespace HolypawUiCopy
{
	FText GameTitle();
	FText Tagline();
	FText SlotHint();
	FText TitleMuteOn();
	FText TitleMuteOff();
	FText EmptyPorch();

	FText Paused();
	FText PauseBlurb();
	FText Resume();
	FText SaveSlot();
	FText Mute();
	FText BackToTitle();

	FText MapTitle();
	FText MapBlurb();
	FText MapHere();
	FText MapHop();
	FText MapHearts();
	FText MapUnlit();

	FText JournalTitle();
	FText Campaign();
	FText Errands();
	FText Current();
	FText JournalClose();

	FText TalkTitle();
	FText Listen();
	FText AskWay();
	FText TurnIn();
	FText TakeJob();
	FText QuietLine();
	FText ErrandDone();
	FText JobReady();

	FText ShopTitle();
	FText ShopDiscount();
	FText ShopFullPrice();
	FText FaithJar();
	FText FaithJarBlurb();
	FText ShopFooter();

	FText BattleYou();
	FText BattleVs();
	FText PageBasics();
	FText PageOverflow();
	FText PageParty();
	FText PhaseTwo();
	FText PolyMill();

	FText PlayTitle();
	FText Miracle();
	FText Hearts();
	FText CityHearts();
	FText Legend();
}
