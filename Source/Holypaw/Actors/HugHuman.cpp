#include "Actors/HugHuman.h"
#include "Character/HolypawCharacter.h"
#include "HolypawGameInstance.h"
#include "AI/HolypawSchedule.h"
#include "UObject/ConstructorHelpers.h"

AHugHuman::AHugHuman()
{
	PrimaryActorTick.bCanEverTick = true;

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	HeadMesh->SetupAttachment(Root);
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadMesh->SetRelativeLocation(FVector(0.f, 0.f, 55.f));
	HeadMesh->SetRelativeScale3D(FVector(0.38f, 0.38f, 0.38f));

	ArmL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL"));
	ArmL->SetupAttachment(Root);
	ArmL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArmL->SetRelativeLocation(FVector(0.f, 22.f, 18.f));
	ArmL->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.45f));

	ArmR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR"));
	ArmR->SetupAttachment(Root);
	ArmR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArmR->SetRelativeLocation(FVector(0.f, -22.f, 18.f));
	ArmR->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.45f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereFinder.Succeeded())
	{
		HeadMesh->SetStaticMesh(SphereFinder.Object);
	}
}

void AHugHuman::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetWorldScale3D(FVector(0.45f, 0.35f, 1.05f));
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
	if (ShapeMat)
	{
		if (UMaterialInstanceDynamic* Mid = HeadMesh->CreateDynamicMaterialInstance(0, ShapeMat))
		{
			Mid->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.96f, 0.8f, 0.74f));
		}
		if (UMaterialInstanceDynamic* Mid = ArmL->CreateDynamicMaterialInstance(0, ShapeMat))
		{
			Mid->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.96f, 0.8f, 0.74f));
		}
		if (UMaterialInstanceDynamic* Mid = ArmR->CreateDynamicMaterialInstance(0, ShapeMat))
		{
			Mid->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.96f, 0.8f, 0.74f));
		}
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
	if (ShapeMat)
	{
		if (UMaterialInstanceDynamic* Mid = HeadMesh->CreateDynamicMaterialInstance(0, ShapeMat))
		{
			Mid->SetVectorParameterValue(TEXT("Color"), FLinearColor(1.f, 0.88f, 0.55f));
		}
		if (UMaterialInstanceDynamic* Mid = Mesh->CreateDynamicMaterialInstance(0, ShapeMat))
		{
			Mid->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.95f, 0.72f, 0.82f));
		}
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
	if (ShapeMat && HeadMesh)
	{
		if (UMaterialInstanceDynamic* Mid = HeadMesh->CreateDynamicMaterialInstance(0, ShapeMat))
		{
			Mid->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.96f, 0.8f, 0.74f));
		}
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
