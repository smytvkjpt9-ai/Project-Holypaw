#include "Audio/HolypawAudioRig.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"

AHolypawAudioRig::AHolypawAudioRig()
{
	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MusicA = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicA"));
	MusicA->SetupAttachment(Root);
	MusicA->bAutoActivate = false;
	MusicA->bIsUISound = true;

	MusicB = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicB"));
	MusicB->SetupAttachment(Root);
	MusicB->bAutoActivate = false;
	MusicB->bIsUISound = true;

	CombatComp = CreateDefaultSubobject<UAudioComponent>(TEXT("CombatStem"));
	CombatComp->SetupAttachment(Root);
	CombatComp->bAutoActivate = false;
	CombatComp->bIsUISound = true;

	Ambient = CreateDefaultSubobject<UAudioComponent>(TEXT("Ambient"));
	Ambient->SetupAttachment(Root);
	Ambient->bAutoActivate = false;
	Ambient->bIsUISound = true;

	Mill = CreateDefaultSubobject<UAudioComponent>(TEXT("MillDrone"));
	Mill->SetupAttachment(Root);
	Mill->bAutoActivate = false;
	Mill->bIsUISound = true;
}
