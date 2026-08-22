#pragma once

#include "CoreMinimal.h"
#include "HolypawTypes.h"

/**
 * Thin talk VM. Keys 1–4 map to verbs. Catalogs stay the source of lines and errands.
 * Character applies inventory, AP/FP, and panel close. Skill-check nodes come later.
 */
namespace HolypawDialogue
{
	enum class ETalkVerb : uint8
	{
		Listen = 0,
		Hint = 1,
		TurnIn = 2,
		Accept = 3,
		None = 255
	};

	struct FTalkState
	{
		FString Speaker;
		bool bTalkOpen = false;
		bool bTalkSecond = false;
		FString TalkBody;
		FString TalkHint;
		TArray<FName> QuestActive;
		TArray<FName> QuestDone;
		bool bTalkThird = false;
		int32 PlayerFP = 0;
	};

	struct FTalkOutcome
	{
		bool bHandled = false;
		bool bCloseTalk = false;
		bool bTalkSecond = false;
		bool bTalkThird = false;
		FString TalkBody;
		FString Toast;
		FName Cue;
		FName NeedItem;
		int32 NeedCount = 0;
		bool bConsumeItem = false;
		FName QuestId;
		bool bMarkQuestDone = false;
		bool bMarkQuestActive = false;
		int32 RewardAP = 0;
		int32 RewardFP = 0;
		float RewardMiracle = 0.f;
	};

	ETalkVerb VerbFromSlot(int32 Index);
	FString ListGivers();
	FTalkOutcome Run(ETalkVerb Verb, const FTalkState& State, TFunction<int32(FName)> GetCount);
}
