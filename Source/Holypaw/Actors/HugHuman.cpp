#include "Actors/HugHuman.h"
#include "Character/HolypawCharacter.h"
#include "Look/HolypawLook.h"
#include "HolypawGameInstance.h"
#include "AI/HolypawSchedule.h"
#include "UObject/ConstructorHelpers.h"

AHugHuman::AHugHuman()
{
	PrimaryActorTick.bCanEverTick = true;

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	HeadMesh->SetupAttachment(Root);
	HeadMesh->SetRelativeLocation(FVector(0.f, 0.f, 58.f));
	HeadMesh->SetRelativeScale3D(FVector(0.40f, 0.38f, 0.40f));

	Hair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(HeadMesh);
	Hair->SetRelativeLocation(FVector(-4.f, 0.f, 18.f));
	Hair->SetRelativeScale3D(FVector(0.95f, 0.95f, 0.55f));

	Bangs = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bangs"));
	Bangs->SetupAttachment(HeadMesh);
	Bangs->SetRelativeLocation(FVector(10.f, 0.f, 12.f));
	Bangs->SetRelativeScale3D(FVector(0.55f, 0.82f, 0.28f));

	Neck = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Neck"));
	Neck->SetupAttachment(Root);
	Neck->SetRelativeLocation(FVector(0.f, 0.f, 38.f));
	Neck->SetRelativeScale3D(FVector(0.16f, 0.16f, 0.18f));

	EyeL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeL"));
	EyeL->SetupAttachment(HeadMesh);
	EyeL->SetRelativeLocation(FVector(16.f, 8.f, 4.f));
	EyeL->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.12f));

	EyeR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeR"));
	EyeR->SetupAttachment(HeadMesh);
	EyeR->SetRelativeLocation(FVector(16.f, -8.f, 4.f));
	EyeR->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.12f));

	HighlightL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightL"));
	HighlightL->SetupAttachment(EyeL);
	HighlightL->SetRelativeLocation(FVector(6.f, 3.f, 4.f));
	HighlightL->SetRelativeScale3D(FVector(0.28f, 0.28f, 0.28f));

	HighlightR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightR"));
	HighlightR->SetupAttachment(EyeR);
	HighlightR->SetRelativeLocation(FVector(6.f, 3.f, 4.f));
	HighlightR->SetRelativeScale3D(FVector(0.28f, 0.28f, 0.28f));

	Hat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hat"));
	Hat->SetupAttachment(HeadMesh);
	Hat->SetRelativeLocation(FVector(0.f, 0.f, 28.f));
	Hat->SetRelativeScale3D(FVector(0.55f, 0.55f, 0.12f));
	Hat->SetHiddenInGame(true);

	ArmL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL"));
	ArmL->SetupAttachment(Root);
	ArmL->SetRelativeLocation(FVector(0.f, 24.f, 18.f));
	ArmL->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.48f));

	ArmR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR"));
	ArmR->SetupAttachment(Root);
	ArmR->SetRelativeLocation(FVector(0.f, -24.f, 18.f));
	ArmR->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.48f));

	HandL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandL"));
	HandL->SetupAttachment(ArmL);
	HandL->SetRelativeLocation(FVector(0.f, 0.f, -22.f));
	HandL->SetRelativeScale3D(FVector(0.85f, 0.70f, 0.28f));

	HandR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandR"));
	HandR->SetupAttachment(ArmR);
	HandR->SetRelativeLocation(FVector(0.f, 0.f, -22.f));
	HandR->SetRelativeScale3D(FVector(0.85f, 0.70f, 0.28f));

	LegL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegL"));
	LegL->SetupAttachment(Root);
	LegL->SetRelativeLocation(FVector(0.f, 10.f, -28.f));
	LegL->SetRelativeScale3D(FVector(0.13f, 0.13f, 0.42f));

	LegR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegR"));
	LegR->SetupAttachment(Root);
	LegR->SetRelativeLocation(FVector(0.f, -10.f, -28.f));
	LegR->SetRelativeScale3D(FVector(0.13f, 0.13f, 0.42f));

	ShoeL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShoeL"));
	ShoeL->SetupAttachment(LegL);
	ShoeL->SetRelativeLocation(FVector(6.f, 0.f, -18.f));
	ShoeL->SetRelativeScale3D(FVector(0.85f, 0.70f, 0.28f));

	ShoeR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShoeR"));
	ShoeR->SetupAttachment(LegR);
	ShoeR->SetRelativeLocation(FVector(6.f, 0.f, -18.f));
	ShoeR->SetRelativeScale3D(FVector(0.85f, 0.70f, 0.28f));
}

void AHugHuman::BeginPlay()
{
	Super::BeginPlay();
	if (CylMesh)
	{
		Mesh->SetStaticMesh(CylMesh);
	}
	HolypawLook::PrepPart(HeadMesh, SphereMesh);
	HolypawLook::PrepPart(Hair, SphereMesh);
	HolypawLook::PrepPart(Bangs, SphereMesh);
	HolypawLook::PrepPart(Neck, CylMesh ? CylMesh : SphereMesh);
	HolypawLook::PrepPart(EyeL, SphereMesh);
	HolypawLook::PrepPart(EyeR, SphereMesh);
	HolypawLook::PrepPart(HighlightL, SphereMesh);
	HolypawLook::PrepPart(HighlightR, SphereMesh);
	HolypawLook::PrepPart(Hat, SphereMesh);
	HolypawLook::PrepPart(ArmL, CylMesh ? CylMesh : CubeMesh);
	HolypawLook::PrepPart(ArmR, CylMesh ? CylMesh : CubeMesh);
	HolypawLook::PrepPart(HandL, SphereMesh);
	HolypawLook::PrepPart(HandR, SphereMesh);
	HolypawLook::PrepPart(LegL, CylMesh ? CylMesh : CubeMesh);
	HolypawLook::PrepPart(LegR, CylMesh ? CylMesh : CubeMesh);
	HolypawLook::PrepPart(ShoeL, SphereMesh);
	HolypawLook::PrepPart(ShoeR, SphereMesh);
	if (ShapeMat)
	{
		for (UStaticMeshComponent* P : { HeadMesh, Hair, Bangs, Neck, EyeL, EyeR, HighlightL, HighlightR, Hat, ArmL, ArmR, HandL, HandR, LegL, LegR, ShoeL, ShoeR })
		{
			if (P) { P->SetMaterial(0, ShapeMat); }
		}
	}
	Mesh->SetRelativeScale3D(FVector(0.42f, 0.32f, 0.95f));
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, 8.f));
	HolypawLook::Paint(HeadMesh, HolypawLook::Skin);
	HolypawLook::Paint(Hair, ShirtColor * 0.55f + FLinearColor(0.12f, 0.08f, 0.06f));
	HolypawLook::Paint(Bangs, ShirtColor * 0.55f + FLinearColor(0.12f, 0.08f, 0.06f));
	HolypawLook::Paint(Neck, HolypawLook::Skin);
	HolypawLook::Paint(EyeL, HolypawLook::Button);
	HolypawLook::Paint(EyeR, HolypawLook::Button);
	HolypawLook::Paint(HighlightL, HolypawLook::Catchlight);
	HolypawLook::Paint(HighlightR, HolypawLook::Catchlight);
	HolypawLook::Paint(Hat, HolypawLook::GoldWarm);
	HolypawLook::Paint(ArmL, ShirtColor);
	HolypawLook::Paint(ArmR, ShirtColor);
	HolypawLook::Paint(HandL, HolypawLook::Skin);
	HolypawLook::Paint(HandR, HolypawLook::Skin);
	HolypawLook::Paint(LegL, ShirtColor * 0.72f);
	HolypawLook::Paint(LegR, ShirtColor * 0.72f);
	HolypawLook::Paint(ShoeL, HolypawLook::Wood);
	HolypawLook::Paint(ShoeR, HolypawLook::Wood);
	HolypawLook::Paint(Mesh, ShirtColor);
	HomeLocation = GetActorLocation();
	HumanRest.Scale = GetActorScale3D();
	HumanRest.ActorRot = GetActorRotation();
	if (HeadMesh)
	{
		HumanRest.HeadLoc = HeadMesh->GetRelativeLocation();
		HumanRest.HeadRot = HeadMesh->GetRelativeRotation();
	}
	if (ArmL)
	{
		HumanRest.ArmLLoc = ArmL->GetRelativeLocation();
	}
	if (ArmR)
	{
		HumanRest.ArmRLoc = ArmR->GetRelativeLocation();
	}
}

void AHugHuman::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	HumanAnim.bBeliever = bBeliever;
	HolypawAnim::TickHuman(HumanAnim, DeltaSeconds);
	const HolypawAnim::FHumanPose Pose = HolypawAnim::EvaluateHuman(HumanAnim, HumanRest);
	SetActorScale3D(Pose.Scale);
	SetActorRotation(Pose.ActorRot);
	if (HeadMesh)
	{
		HeadMesh->SetRelativeLocation(Pose.HeadLoc);
		HeadMesh->SetRelativeRotation(Pose.HeadRot);
	}
	if (ArmL)
	{
		ArmL->SetRelativeRotation(Pose.ArmL);
		ArmL->SetRelativeLocation(Pose.ArmLLoc);
	}
	if (ArmR)
	{
		ArmR->SetRelativeRotation(Pose.ArmR);
		ArmR->SetRelativeLocation(Pose.ArmRLoc);
	}

	BounceT += DeltaSeconds;
	if (!Pose.bHoldFeet)
	{
		float Hour = 12.f;
		if (const UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
		{
			Hour = GI->GetWorldHour();
		}
		HolypawSchedule::TickHuman(*this, DeltaSeconds, Hour);
		// Day parade / dusk chapel / night inn: bBeliever && !bKnelt is the old beat; TickHuman owns it now.
	}
	FVector Feet = GetActorLocation();
	Feet.Z = HomeLocation.Z + Pose.DropZ;
	SetActorLocation(Feet);
}

FText AHugHuman::GetPrompt() const
{
	if (bBeliever)
	{
		return FText::Format(NSLOCTEXT("Holypaw", "BelieverTalk", "E  Hear {0}'s testimony"), PersonName);
	}
	return FText::Format(NSLOCTEXT("Holypaw", "Hug", "E  Hug {0} until they agree  ({1}%)"),
		PersonName,
		FText::AsNumber(FMath::FloorToInt(ConvertProgress)));
}

bool AHugHuman::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn)
	{
		return false;
	}
	if (InstigatorPawn && bBeliever)
	{
		return InstigatorPawn->StartTalk(this);
	}
	return InstigatorPawn->HugPerson(this);
}

void AHugHuman::ReceiveHug()
{
	HolypawAnim::PlayHumanHug(HumanAnim);
}

void AHugHuman::ReceiveHug(const FVector& FromWorld)
{
	HolypawAnim::PlayHumanHug(HumanAnim, FromWorld - GetActorLocation());
}

FString AHugHuman::GetSkepticLine(int32 Pct) const
{
	if (Pct < 25)
	{
		return TEXT("\"Bears cannot be government,\" they mumble.");
	}
	if (Pct < 50)
	{
		return TEXT("\"Why do I want to clap,\" they whisper.");
	}
	if (Pct < 75)
	{
		return TEXT("\"Polyester was a personality. Embarrassing.\"");
	}
	return TEXT("\"I think my opinions are getting round.\"");
}

FString AHugHuman::GetBelieverLine() const
{
	return FString::Printf(TEXT("%s: \"The bear is correct. I was being so serious.\""), *PersonName.ToString());
}

void AHugHuman::BecomeBeliever()
{
	bBeliever = true;
	ConvertProgress = 100.f;
	HumanAnim.bBeliever = true;
	HolypawLook::Paint(HeadMesh, HolypawLook::BelieverGold);
	HolypawLook::Paint(Mesh, HolypawLook::Rose);
	HolypawLook::Paint(Hair, HolypawLook::Gold);
	HolypawLook::Paint(Bangs, HolypawLook::Gold);
	HolypawLook::Paint(ArmL, HolypawLook::Rose);
	HolypawLook::Paint(ArmR, HolypawLook::Rose);
	if (Hat)
	{
		Hat->SetHiddenInGame(false);
	}
}

void AHugHuman::PlayConvertBow()
{
	BecomeBeliever();
	HolypawAnim::PlayConvertBow(HumanAnim);
}

void AHugHuman::KneelInWorship()
{
	if (bKnelt)
	{
		return;
	}
	bKnelt = true;
	BecomeBeliever();
	HolypawAnim::PlayWorshipKneel(HumanAnim);
}

void AHugHuman::ResetFaith()
{
	bBeliever = false;
	ConvertProgress = 0.f;
	bKnelt = false;
	HolypawAnim::ResetHumanMotion(HumanAnim);
	SetActorRotation(HumanRest.ActorRot);
	SetActorScale3D(HumanRest.Scale);
	SetActorLocation(HomeLocation);
	SetSolidColor(ShirtColor);
	HolypawLook::Paint(HeadMesh, HolypawLook::Skin);
	HolypawLook::Paint(Hair, ShirtColor * 0.55f + FLinearColor(0.12f, 0.08f, 0.06f));
	HolypawLook::Paint(Bangs, ShirtColor * 0.55f + FLinearColor(0.12f, 0.08f, 0.06f));
	HolypawLook::Paint(ArmL, ShirtColor);
	HolypawLook::Paint(ArmR, ShirtColor);
	HolypawLook::Paint(HandL, HolypawLook::Skin);
	HolypawLook::Paint(HandR, HolypawLook::Skin);
	if (Hat)
	{
		Hat->SetHiddenInGame(true);
	}
}

void AHugHuman::RestoreFaith(float Progress, bool bNowBeliever, bool bNowKnelt)
{
	ResetFaith();
	ConvertProgress = FMath::Clamp(Progress, 0.f, 100.f);
	if (bNowBeliever || ConvertProgress >= 100.f)
	{
		BecomeBeliever();
	}
	if (bNowKnelt)
	{
		KneelInWorship();
		HumanAnim.KneelT = HolypawAnim::KneelSeconds;
	}
}
