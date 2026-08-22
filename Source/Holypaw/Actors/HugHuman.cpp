#include "Actors/HugHuman.h"
#include "Character/HolypawCharacter.h"
#include "HolypawGameInstance.h"
#include "AI/HolypawSchedule.h"
#include "Faith/HolypawFaithSim.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

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

	Sash = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sash"));
	Sash->SetupAttachment(Root);
	Sash->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sash->SetRelativeLocation(FVector(4.f, 0.f, 22.f));
	Sash->SetRelativeScale3D(FVector(0.42f, 0.08f, 0.55f));
	Sash->SetRelativeRotation(FRotator(0.f, 0.f, 28.f));
	Sash->SetHiddenInGame(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereFinder.Succeeded())
	{
		HeadMesh->SetStaticMesh(SphereFinder.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		Sash->SetStaticMesh(CubeFinder.Object);
		ArmL->SetStaticMesh(CubeFinder.Object);
		ArmR->SetStaticMesh(CubeFinder.Object);
	}
}

void AHugHuman::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetWorldScale3D(FVector(0.45f, 0.35f, 1.05f));
	BaseScale = GetActorScale3D();
	HomeLocation = GetActorLocation();
	HomeZone = HolypawFaith::ZoneAt(this, HomeLocation);
	bHomeZoneReady = true;
	bTendsStall = PersonName.ToString().Contains(TEXT("Shopkeep")) || PersonName.ToString().Contains(TEXT("Hawker"));
	ParadeSalt = FMath::Fmod(static_cast<float>(GetTypeHash(PersonName.ToString())) * 0.017f + FMath::Abs(HomeLocation.X) * 0.00013f, 4.f);
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
	if (ClapBurst > 0.f)
	{
		ClapBurst = FMath::Max(0.f, ClapBurst - DeltaSeconds);
	}
	const float Bounce = bBeliever ? 0.06f * FMath::Sin(BounceT * 6.f) : 0.02f * FMath::Sin(BounceT * 2.4f);
	const float Squeeze = HugPulse > 0.f ? 0.12f * HugPulse : 0.f;
	SetActorScale3D(BaseScale * FVector(1.f + Squeeze, 1.f + Squeeze, 1.f + Bounce - Squeeze * 0.4f));

	float Hour = 12.f;
	bool bDusk = false;
	if (const UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		Hour = GI->GetWorldHour();
		bDusk = GI->IsDusk();
	}
	const int32 Hearts = HolypawFaith::HeartsAt(this, HomeZone);
	const bool bClap = IsClapping();
	if (ArmL && ArmR)
	{
		const float Wrap = (HugPulse > 0.f) ? 28.f : (bBeliever ? 8.f : 0.f);
		const float Rate = HolypawFaith::ClapRate(Hearts, bDusk);
		const float Clap = bClap ? 42.f * FMath::Abs(FMath::Sin(BounceT * Rate)) : 0.f;
		ArmL->SetRelativeRotation(FRotator(Clap, 0.f, Wrap));
		ArmR->SetRelativeRotation(FRotator(Clap, 0.f, -Wrap));
		ArmL->SetRelativeLocation(FVector(8.f + Clap * 0.15f, 22.f, 18.f + Clap * 0.35f));
		ArmR->SetRelativeLocation(FVector(8.f + Clap * 0.15f, -22.f, 18.f + Clap * 0.35f));
	}

	HolypawSchedule::TickHuman(*this, DeltaSeconds, Hour);
	// Day parade / dusk chapel / night inn: bBeliever && !bKnelt is the old beat; TickHuman owns it now.
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
	if (IsClapping())
	{
		return FString::Printf(TEXT("%s: \"The bear is correct. I was being so serious.\" *clap*"), *PersonName.ToString());
	}
	return FString::Printf(TEXT("%s: \"The bear is correct. I was being so serious.\""), *PersonName.ToString());
}

bool AHugHuman::IsClapping() const
{
	if (!bBeliever || bKnelt)
	{
		return false;
	}
	if (ClapBurst > 0.f)
	{
		return true;
	}
	if (ParadeKick > 0.f)
	{
		return true;
	}
	const int32 Hearts = HolypawFaith::HeartsAt(this, HomeZone);
	if (!HolypawFaith::BelieversParade(Hearts))
	{
		return false;
	}
	bool bDusk = false;
	if (const UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		bDusk = GI->IsDusk();
	}
	if (bDusk && HolypawFaith::ChoirOwnsDusk(Hearts))
	{
		return true;
	}
	return FMath::Fmod(BounceT, 3.2f) < 1.1f;
}

void AHugHuman::BecomeBeliever(const bool bCelebrate)
{
	bBeliever = true;
	ConvertProgress = 100.f;
	if (bCelebrate)
	{
		ClapBurst = 2.6f;
		ParadeKick = 7.5f;
	}
	if (Sash)
	{
		Sash->SetHiddenInGame(false);
		if (ShapeMat)
		{
			if (UMaterialInstanceDynamic* Mid = Sash->CreateDynamicMaterialInstance(0, ShapeMat))
			{
				Mid->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.95f, 0.42f, 0.58f));
			}
		}
	}
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
	ClapBurst = 0.f;
	ParadeKick = 0.f;
	BaseScale = FVector::OneVector;
	SetActorScale3D(BaseScale);
	SetActorLocation(HomeLocation);
	SetSolidColor(ShirtColor);
	if (Sash)
	{
		Sash->SetHiddenInGame(true);
	}
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
		BecomeBeliever(false);
	}
	if (bNowKnelt)
	{
		KneelInWorship();
	}
}
