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
	FText SlotN(int32 N);

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
	FText HomePin();
	FText PeakPin();
	FText Lanterns();

	FText JournalTitle();
	FText Campaign();
	FText Errands();
	FText Current();
	FText JournalClose();
	FText RecruitsN(int32 N);
	FText MiraclesN(int32 N);
	FText CitiesN(int32 Have, int32 Total);
	FText JournalEmptyErrands();

	FText TalkTitle();
	FText Listen();
	FText AskWay();
	FText TurnIn();
	FText TakeJob();
	FText QuietLine();
	FText QuietAt(int32 Faith);
	FText ErrandDone();
	FText JobReady();
	FText Believer();

	FText ShopTitle();
	FText ShopDiscount();
	FText ShopFullPrice();
	FText FaithJar();
	FText FaithJarBlurb();
	FText ShopFooter();
	FText ApN(int32 N);
	FText PriceAp(int32 N);
	FText PriceDrop(int32 From, int32 To);
	FText NeedAp(int32 N);

	FText Continue();
	FText NewCoup();
	FText LoadSlot();
	FText AudioOn();
	FText HomeSewn();
	FText HeatCold();
	FText HeatWarm();
	FText HeatGlow();
	FText CodexTitle();
	FText CodexUnseen();
	FText CodexUnknown();
	FText CodexHunting();
	FText CodexFell();
	FText SeenN(int32 N);
	FText FellN(int32 Have, int32 Total);
	FText TabHint();
	FText Owned();
	FText Buy();

	FText BattleYou();
	FText BattleVs();
	FText PageBasics();
	FText PageOverflow();
	FText PageParty();
	FText PhaseTwo();
	FText PolyMill();
	FText PolyMillTag();
	FText Hostile();
	FText FpCost(int32 N);
	FText StitchPlus(int32 N);
	FText PartyTitle();
	FText PartyEmpty();
	FText PocketsTitle();
	FText PocketsEmpty();
	FText AtkN(int32 N);
	FText CountX(int32 N);
	FText HpFrac(int32 Have, int32 Max);

	FText PlayTitle();
	FText Miracle();
	FText Hearts();
	FText HeartsN(int32 N);
	FText CityHearts();
	FText Legend();
}
