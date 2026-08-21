#include "Actors/HugHuman.h"
#include "Character/HolypawCharacter.h"
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
	BaseScale = GetActorScale3D();
	HomeLocation = GetActorLocation();
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
	BounceT += DeltaSeconds;
	if (HugPulse > 0.f)
	{
		HugPulse = FMath::Max(0.f, HugPulse - DeltaSeconds);
	}
	const float Bounce = bBeliever ? 0.06f * FMath::Sin(BounceT * 6.f) : 0.02f * FMath::Sin(BounceT * 2.4f);
	const float Squeeze = HugPulse > 0.f ? 0.12f * HugPulse : 0.f;
	SetActorScale3D(BaseScale * FVector(1.f + Squeeze, 1.f + Squeeze, 1.f + Bounce - Squeeze * 0.4f));

	if (ArmL && ArmR)
	{
		const float Wrap = (HugPulse > 0.f) ? 28.f : (bBeliever ? 8.f : 0.f);
		ArmL->SetRelativeRotation(FRotator(0.f, 0.f, Wrap));
		ArmR->SetRelativeRotation(FRotator(0.f, 0.f, -Wrap));
	}

	if (bBeliever && !bKnelt)
	{
		const float Orbit = 160.f;
		const float Ang = BounceT * 0.55f;
		const FVector Parade = HomeLocation + FVector(FMath::Cos(Ang) * Orbit, FMath::Sin(Ang) * Orbit, 0.f);
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), Parade, DeltaSeconds, 1.6f));
	}
}

FText AHugHuman::GetPrompt() const
{
	if (bBeliever)
	{
		return FText::Format(NSLOCTEXT("Holypaw", "Believer", "E  {0} is yours forever"), PersonName);
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
	return InstigatorPawn->HugPerson(this);
}

void AHugHuman::ReceiveHug()
{
	HugPulse = 1.f;
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

void AHugHuman::KneelInWorship()
{
	if (bKnelt)
	{
		return;
	}
	bKnelt = true;
	BecomeBeliever();
	AddActorWorldRotation(FRotator(35.f, 0.f, 0.f));
	BaseScale = FVector(1.f, 1.f, 0.72f);
	SetActorScale3D(BaseScale);
}

void AHugHuman::ResetFaith()
{
	bBeliever = false;
	ConvertProgress = 0.f;
	if (bKnelt)
	{
		SetActorRotation(FRotator::ZeroRotator);
	}
	bKnelt = false;
	BaseScale = FVector::OneVector;
	SetActorScale3D(BaseScale);
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
	}
}
