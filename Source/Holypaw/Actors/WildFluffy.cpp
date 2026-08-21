#include "Actors/WildFluffy.h"
#include "Character/HolypawCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWildFluffy::AWildFluffy()
{
	PrimaryActorTick.bCanEverTick = true;

	EarL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EarL"));
	EarL->SetupAttachment(Mesh);
	EarL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EarL->SetRelativeLocation(FVector(0.f, 18.f, 28.f));
	EarL->SetRelativeScale3D(FVector(0.22f, 0.14f, 0.4f));

	EarR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EarR"));
	EarR->SetupAttachment(Mesh);
	EarR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EarR->SetRelativeLocation(FVector(0.f, -18.f, 28.f));
	EarR->SetRelativeScale3D(FVector(0.22f, 0.14f, 0.4f));

	Tail = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tail"));
	Tail->SetupAttachment(Mesh);
	Tail->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Tail->SetRelativeLocation(FVector(-28.f, 0.f, 4.f));
	Tail->SetRelativeScale3D(FVector(0.28f, 0.2f, 0.2f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeFinder(TEXT("/Engine/BasicShapes/Cone.Cone"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (ConeFinder.Succeeded())
	{
		EarL->SetStaticMesh(ConeFinder.Object);
		EarR->SetStaticMesh(ConeFinder.Object);
	}
	if (SphereFinder.Succeeded())
	{
		Tail->SetStaticMesh(SphereFinder.Object);
	}
}

void AWildFluffy::BeginPlay()
{
	Super::BeginPlay();
	Home = GetActorLocation();
	Mesh->SetWorldScale3D(FVector(0.55f, 0.4f, 0.4f));
	SetSolidColor(Type.Color);
	if (ShapeMat)
	{
		const FLinearColor Dark = Type.Color * 0.75f;
		if (UMaterialInstanceDynamic* Mid = EarL->CreateDynamicMaterialInstance(0, ShapeMat))
		{
			Mid->SetVectorParameterValue(TEXT("Color"), Dark);
		}
		if (UMaterialInstanceDynamic* Mid = EarR->CreateDynamicMaterialInstance(0, ShapeMat))
		{
			Mid->SetVectorParameterValue(TEXT("Color"), Dark);
		}
		if (UMaterialInstanceDynamic* Mid = Tail->CreateDynamicMaterialInstance(0, ShapeMat))
		{
			Mid->SetVectorParameterValue(TEXT("Color"), Type.Color);
		}
	}
	WanderT = FMath::FRandRange(0.5f, 2.f);
}

FText AWildFluffy::GetPrompt() const
{
	if (bRecruited)
	{
		return FText::GetEmpty();
	}
	return FText::Format(NSLOCTEXT("Holypaw", "Recruit", "E  Recruit {0} (tiny hench-fluff)"), Type.DisplayName);
}

bool AWildFluffy::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn || bRecruited)
	{
		return false;
	}
	return InstigatorPawn->RecruitFluffy(this);
}

void AWildFluffy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bRecruited)
	{
		return;
	}
	WanderT -= DeltaSeconds;
	if (WanderT <= 0.f)
	{
		WanderT = FMath::FRandRange(1.5f, 3.5f);
		Vel = FVector(FMath::FRandRange(-80.f, 80.f), FMath::FRandRange(-80.f, 80.f), 0.f);
	}
	FVector Next = GetActorLocation() + Vel * DeltaSeconds;
	if (FVector::Dist2D(Next, Home) > 350.f)
	{
		Vel *= -1.f;
	}
	else
	{
		SetActorLocation(Next);
	}
}
