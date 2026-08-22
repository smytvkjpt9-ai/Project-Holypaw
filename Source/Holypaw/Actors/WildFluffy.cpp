#include "Actors/WildFluffy.h"
#include "Character/HolypawCharacter.h"
#include "Look/HolypawLook.h"
#include "Components/StaticMeshComponent.h"

AWildFluffy::AWildFluffy()
{
	PrimaryActorTick.bCanEverTick = true;

	EarL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EarL"));
	EarL->SetupAttachment(Mesh);
	EarL->SetRelativeLocation(FVector(0.f, 20.f, 30.f));
	EarL->SetRelativeScale3D(FVector(0.20f, 0.14f, 0.42f));

	EarR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EarR"));
	EarR->SetupAttachment(Mesh);
	EarR->SetRelativeLocation(FVector(0.f, -20.f, 30.f));
	EarR->SetRelativeScale3D(FVector(0.20f, 0.14f, 0.42f));

	Tail = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tail"));
	Tail->SetupAttachment(Mesh);
	Tail->SetRelativeLocation(FVector(-30.f, 0.f, 4.f));
	Tail->SetRelativeScale3D(FVector(0.30f, 0.22f, 0.22f));

	EyeL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeL"));
	EyeL->SetupAttachment(Mesh);
	EyeL->SetRelativeLocation(FVector(18.f, 8.f, 10.f));
	EyeL->SetRelativeScale3D(FVector(0.10f, 0.10f, 0.10f));

	EyeR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeR"));
	EyeR->SetupAttachment(Mesh);
	EyeR->SetRelativeLocation(FVector(18.f, -8.f, 10.f));
	EyeR->SetRelativeScale3D(FVector(0.10f, 0.10f, 0.10f));

	Snout = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Snout"));
	Snout->SetupAttachment(Mesh);
	Snout->SetRelativeLocation(FVector(22.f, 0.f, 2.f));
	Snout->SetRelativeScale3D(FVector(0.22f, 0.18f, 0.16f));

	PawL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawL"));
	PawL->SetupAttachment(Mesh);
	PawL->SetRelativeLocation(FVector(10.f, 16.f, -16.f));
	PawL->SetRelativeScale3D(FVector(0.22f, 0.18f, 0.14f));

	PawR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawR"));
	PawR->SetupAttachment(Mesh);
	PawR->SetRelativeLocation(FVector(10.f, -16.f, -16.f));
	PawR->SetRelativeScale3D(FVector(0.22f, 0.18f, 0.14f));

	HindL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HindL"));
	HindL->SetupAttachment(Mesh);
	HindL->SetRelativeLocation(FVector(-10.f, 14.f, -16.f));
	HindL->SetRelativeScale3D(FVector(0.20f, 0.16f, 0.14f));

	HindR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HindR"));
	HindR->SetupAttachment(Mesh);
	HindR->SetRelativeLocation(FVector(-10.f, -14.f, -16.f));
	HindR->SetRelativeScale3D(FVector(0.20f, 0.16f, 0.14f));

	WingL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WingL"));
	WingL->SetupAttachment(Mesh);
	WingL->SetRelativeLocation(FVector(-4.f, 22.f, 6.f));
	WingL->SetRelativeRotation(FRotator(0.f, 0.f, 55.f));
	WingL->SetRelativeScale3D(FVector(0.12f, 0.42f, 0.08f));
	WingL->SetHiddenInGame(true);

	WingR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WingR"));
	WingR->SetupAttachment(Mesh);
	WingR->SetRelativeLocation(FVector(-4.f, -22.f, 6.f));
	WingR->SetRelativeRotation(FRotator(0.f, 0.f, -55.f));
	WingR->SetRelativeScale3D(FVector(0.12f, 0.42f, 0.08f));
	WingR->SetHiddenInGame(true);
}

void AWildFluffy::BeginPlay()
{
	Super::BeginPlay();
	Home = GetActorLocation();
	if (SphereMesh)
	{
		Mesh->SetStaticMesh(SphereMesh);
	}
	HolypawLook::PrepPart(EarL, ConeMesh ? ConeMesh : SphereMesh);
	HolypawLook::PrepPart(EarR, ConeMesh ? ConeMesh : SphereMesh);
	HolypawLook::PrepPart(Tail, SphereMesh);
	HolypawLook::PrepPart(EyeL, SphereMesh);
	HolypawLook::PrepPart(EyeR, SphereMesh);
	HolypawLook::PrepPart(Snout, SphereMesh);
	HolypawLook::PrepPart(PawL, SphereMesh);
	HolypawLook::PrepPart(PawR, SphereMesh);
	HolypawLook::PrepPart(HindL, SphereMesh);
	HolypawLook::PrepPart(HindR, SphereMesh);
	HolypawLook::PrepPart(WingL, ConeMesh ? ConeMesh : SphereMesh);
	HolypawLook::PrepPart(WingR, ConeMesh ? ConeMesh : SphereMesh);
	if (ShapeMat)
	{
		for (UStaticMeshComponent* P : { EarL, EarR, Tail, EyeL, EyeR, Snout, PawL, PawR, HindL, HindR, WingL, WingR })
		{
			if (P) { P->SetMaterial(0, ShapeMat); }
		}
	}
	Mesh->SetWorldScale3D(FVector(0.58f, 0.42f, 0.42f));
	SetSolidColor(Type.Color);
	const FLinearColor Dark = Type.Color * 0.72f;
	HolypawLook::Paint(EarL, Dark);
	HolypawLook::Paint(EarR, Dark);
	HolypawLook::Paint(Tail, Type.Color);
	HolypawLook::Paint(EyeL, HolypawLook::Button);
	HolypawLook::Paint(EyeR, HolypawLook::Button);
	HolypawLook::Paint(Snout, HolypawLook::Cream);
	HolypawLook::Paint(PawL, Type.Color * 0.82f);
	HolypawLook::Paint(PawR, Type.Color * 0.82f);
	HolypawLook::Paint(HindL, Type.Color * 0.78f);
	HolypawLook::Paint(HindR, Type.Color * 0.78f);
	HolypawLook::Paint(WingL, Type.Color * 0.88f);
	HolypawLook::Paint(WingR, Type.Color * 0.88f);
	switch (Type.Id)
	{
	case EFluffyId::Bunny:
		if (EarL) { EarL->SetRelativeScale3D(FVector(0.16f, 0.12f, 0.62f)); }
		if (EarR) { EarR->SetRelativeScale3D(FVector(0.16f, 0.12f, 0.62f)); }
		if (Tail) { Tail->SetRelativeScale3D(FVector(0.22f, 0.22f, 0.22f)); }
		break;
	case EFluffyId::Pup:
		if (Tail) { Tail->SetRelativeScale3D(FVector(0.42f, 0.16f, 0.16f)); }
		if (EarL) { EarL->SetRelativeRotation(FRotator(28.f, 8.f, -24.f)); EarL->SetRelativeScale3D(FVector(0.22f, 0.16f, 0.34f)); }
		if (EarR) { EarR->SetRelativeRotation(FRotator(28.f, -8.f, 24.f)); EarR->SetRelativeScale3D(FVector(0.22f, 0.16f, 0.34f)); }
		break;
	case EFluffyId::Duck:
		if (Snout) { Snout->SetRelativeScale3D(FVector(0.38f, 0.22f, 0.12f)); }
		if (EarL) { EarL->SetHiddenInGame(true); }
		if (EarR) { EarR->SetHiddenInGame(true); }
		if (WingL) { WingL->SetHiddenInGame(false); }
		if (WingR) { WingR->SetHiddenInGame(false); }
		HolypawLook::Paint(Snout, HolypawLook::Gold);
		break;
	case EFluffyId::Panda:
		HolypawLook::Paint(EarL, HolypawLook::Button);
		HolypawLook::Paint(EarR, HolypawLook::Button);
		HolypawLook::Paint(EyeL, FLinearColor(0.08f, 0.08f, 0.1f));
		HolypawLook::Paint(EyeR, FLinearColor(0.08f, 0.08f, 0.1f));
		if (EyeL) { EyeL->SetRelativeScale3D(FVector(0.18f, 0.16f, 0.16f)); }
		if (EyeR) { EyeR->SetRelativeScale3D(FVector(0.18f, 0.16f, 0.16f)); }
		break;
	case EFluffyId::Fox:
		if (EarL) { EarL->SetRelativeScale3D(FVector(0.18f, 0.12f, 0.48f)); }
		if (EarR) { EarR->SetRelativeScale3D(FVector(0.18f, 0.12f, 0.48f)); }
		if (Tail) { Tail->SetRelativeScale3D(FVector(0.48f, 0.20f, 0.20f)); }
		HolypawLook::Paint(Snout, FLinearColor(0.95f, 0.92f, 0.88f));
		HolypawLook::Paint(Tail, HolypawLook::Cream);
		break;
	case EFluffyId::Dragon:
		if (EarL) { EarL->SetRelativeRotation(FRotator(0.f, 0.f, -28.f)); }
		if (EarR) { EarR->SetRelativeRotation(FRotator(0.f, 0.f, 28.f)); }
		if (WingL) { WingL->SetHiddenInGame(false); WingL->SetRelativeScale3D(FVector(0.16f, 0.55f, 0.10f)); }
		if (WingR) { WingR->SetHiddenInGame(false); WingR->SetRelativeScale3D(FVector(0.16f, 0.55f, 0.10f)); }
		HolypawLook::Paint(Tail, HolypawLook::Gold);
		HolypawLook::Paint(WingL, HolypawLook::Velvet);
		HolypawLook::Paint(WingR, HolypawLook::Velvet);
		break;
	default:
		break;
	}
	WanderT = FMath::FRandRange(0.5f, 2.f);
	MeshBase = Mesh ? Mesh->GetRelativeLocation() : FVector::ZeroVector;
	if (EarL)
	{
		FluffyRest.EarL = EarL->GetRelativeRotation();
	}
	if (EarR)
	{
		FluffyRest.EarR = EarR->GetRelativeRotation();
	}
	if (Tail)
	{
		FluffyRest.TailLoc = Tail->GetRelativeLocation();
	}
	if (Mesh)
	{
		FluffyRest.BodyScale = Mesh->GetRelativeScale3D();
	}
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
	AnimClock += DeltaSeconds;
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
	const HolypawAnim::FFluffyPose Pose = HolypawAnim::EvaluateFluffy(FluffyRest, AnimClock, Vel, bRecruited);
	if (EarL)
	{
		EarL->SetRelativeRotation(Pose.EarL);
	}
	if (EarR)
	{
		EarR->SetRelativeRotation(Pose.EarR);
	}
	if (Tail)
	{
		Tail->SetRelativeLocation(Pose.TailLoc);
		Tail->SetRelativeRotation(Pose.TailRot);
	}
	if (Mesh)
	{
		Mesh->SetRelativeScale3D(Pose.BodyScale);
		Mesh->SetRelativeLocation(MeshBase + FVector(0.f, 0.f, Pose.BobZ));
	}
}
