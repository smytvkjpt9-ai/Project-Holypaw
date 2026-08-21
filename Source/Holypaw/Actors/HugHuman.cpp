#include "Actors/HugHuman.h"
#include "Character/HolypawCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHugHuman::AHugHuman()
{
	PrimaryActorTick.bCanEverTick = false;

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

FText AHugHuman::GetPrompt() const
{
	if (bBeliever)
	{
		return FText::Format(NSLOCTEXT("Holypaw", "Believer", "{0} keeps the Bear Faith"), PersonName);
	}
	return FText::Format(NSLOCTEXT("Holypaw", "Hug", "E  Hug {0}  ({1}%)"),
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
	}
}

void AHugHuman::KneelInWorship()
{
	BecomeBeliever();
	AddActorWorldRotation(FRotator(35.f, 0.f, 0.f));
	SetActorScale3D(FVector(1.f, 1.f, 0.72f));
}
