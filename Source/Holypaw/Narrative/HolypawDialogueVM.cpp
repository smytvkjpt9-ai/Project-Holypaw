#include "Narrative/HolypawDialogueVM.h"

namespace HolypawDialogue
{
	ETalkVerb VerbFromSlot(const int32 Index)
	{
		switch (Index)
		{
		case 0: return ETalkVerb::Listen;
		case 1: return ETalkVerb::Hint;
		case 2: return ETalkVerb::TurnIn;
		case 3: return ETalkVerb::Accept;
		default: return ETalkVerb::None;
		}
	}

	FString ListGivers()
	{
		TArray<FString> Names;
		for (const FHolypawQuestDef& Q : HolypawCatalog::GetQuests())
		{
			Names.AddUnique(Q.Giver);
		}
		FString Out;
		for (int32 I = 0; I < Names.Num(); ++I)
		{
			if (I > 0)
			{
				Out += TEXT(", ");
			}
			Out += Names[I];
		}
		return Out;
	}

	static FTalkOutcome Listen(const FTalkState& State)
	{
		FTalkOutcome O;
		O.bHandled = true;
		O.Cue = TEXT("Talk");
		if (!State.bTalkSecond)
		{
			if (const FHolypawTalkDef* Talk = HolypawCatalog::FindTalk(State.Speaker))
			{
				O.TalkBody = Talk->LineB;
			}
			else
			{
				O.TalkBody = State.TalkBody;
			}
			O.bTalkSecond = true;
			return O;
		}
		O.bCloseTalk = true;
		O.Cue = NAME_None;
		return O;
	}

	static FTalkOutcome Hint(const FTalkState& State)
	{
		FTalkOutcome O;
		O.bHandled = true;
		O.Cue = TEXT("Talk");
		O.Toast = State.TalkHint.IsEmpty()
			? TEXT("Lanterns. Tab. E. The globe shrinks.")
			: State.TalkHint;
		O.bTalkSecond = State.bTalkSecond;
		O.TalkBody = State.TalkBody;
		return O;
	}

	static FTalkOutcome TurnIn(const FTalkState& State, const TFunction<int32(FName)>& GetCount)
	{
		FTalkOutcome O;
		O.bHandled = true;
		O.bTalkSecond = State.bTalkSecond;
		O.TalkBody = State.TalkBody;
		bool bAny = false;
		for (const FHolypawQuestDef& Q : HolypawCatalog::GetQuests())
		{
			if (!Q.TurnIn.Equals(State.Speaker, ESearchCase::IgnoreCase))
			{
				continue;
			}
			bAny = true;
			if (State.QuestDone.Contains(Q.Id))
			{
				O.Toast = TEXT("That errand already clapped. Find another round person.");
				return O;
			}
			const int32 Have = GetCount ? GetCount(Q.NeedItem) : 0;
			if (Have < Q.NeedCount)
			{
				const FHolypawItemDef* Item = HolypawCatalog::FindItem(Q.NeedItem);
				O.Toast = FString::Printf(TEXT("Need %s. %s"),
					Item ? *Item->DisplayName.ToString() : *Q.NeedItem.ToString(),
					*Q.Brief.ToString());
				return O;
			}
			O.bConsumeItem = true;
			O.NeedItem = Q.NeedItem;
			O.NeedCount = Q.NeedCount;
			O.bMarkQuestDone = true;
			O.QuestId = Q.Id;
			O.RewardAP = Q.RewardAP;
			O.RewardFP = Q.RewardFP;
			O.RewardMiracle = Q.RewardMiracle;
			O.Cue = TEXT("Convert");
			O.Toast = Q.DoneLine.IsEmpty()
				? FString::Printf(TEXT("Errand done: %s  +%d AP"), *Q.Title.ToString(), Q.RewardAP)
				: Q.DoneLine;
			return O;
		}
		O.Toast = bAny
			? TEXT("Nothing to hand over.")
			: TEXT("They do not have an errand. Hug, clap, or ask the way.");
		return O;
	}

	static FTalkOutcome Accept(const FTalkState& State)
	{
		FTalkOutcome O;
		O.bHandled = true;
		O.bTalkSecond = State.bTalkSecond;
		O.TalkBody = State.TalkBody;
		const FHolypawQuestDef* Q = HolypawCatalog::FindQuestByGiver(State.Speaker);
		if (!Q)
		{
			O.Toast = FString::Printf(TEXT("No job on this person. Try %s."), *ListGivers());
			return O;
		}
		if (State.QuestDone.Contains(Q->Id))
		{
			O.Toast = TEXT("Already finished. They are still clapping about it.");
			return O;
		}
		if (State.QuestActive.Contains(Q->Id))
		{
			O.Toast = Q->Brief.ToString();
			return O;
		}
		O.bMarkQuestActive = true;
		O.QuestId = Q->Id;
		O.Cue = TEXT("Talk");
		O.Toast = Q->OfferLine.IsEmpty() ? Q->Brief.ToString() : Q->OfferLine;
		return O;
	}

	FTalkOutcome Run(const ETalkVerb Verb, const FTalkState& State, TFunction<int32(FName)> GetCount)
	{
		FTalkOutcome O;
		if (!State.bTalkOpen || Verb == ETalkVerb::None)
		{
			return O;
		}
		switch (Verb)
		{
		case ETalkVerb::Listen: return Listen(State);
		case ETalkVerb::Hint: return Hint(State);
		case ETalkVerb::TurnIn: return TurnIn(State, GetCount);
		case ETalkVerb::Accept: return Accept(State);
		default: return O;
		}
	}
}
