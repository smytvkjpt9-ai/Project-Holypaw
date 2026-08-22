#include "Actors/HostilePet.h"
#include "Character/HolypawCharacter.h"
#include "Combat/HolypawBattleMath.h"
#include "Look/HolypawLook.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

AHostilePet::AHostilePet()
{
	PrimaryActorTick.bCanEverTick = true;

	AccentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Accent"));
	AccentMesh->SetupAttachment(Root);

	AccentMeshB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AccentB"));
	AccentMeshB->SetupAttachment(Root);

	CrestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crest"));
	CrestMesh->SetupAttachment(Root);

	Cloak = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cloak"));
	Cloak->SetupAttachment(Mesh);
	Cloak->SetRelativeLocation(FVector(-22.f, 0.f, 2.f));
	Cloak->SetRelativeRotation(FRotator(12.f, 0.f, 0.f));
	Cloak->SetRelativeScale3D(FVector(0.72f, 0.95f, 0.22f));
	Cloak->SetHiddenInGame(true);

	HornR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HornR"));
	HornR->SetupAttachment(Root);
	HornR->SetRelativeLocation(FVector(-8.f, -22.f, 36.f));
	HornR->SetRelativeRotation(FRotator(12.f, -18.f, 0.f));
	HornR->SetRelativeScale3D(FVector(0.16f, 0.12f, 0.28f));

	EyeL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeL"));
	EyeL->SetupAttachment(Mesh);
	EyeL->SetRelativeLocation(FVector(38.f, 16.f, 26.f));
	EyeL->SetRelativeScale3D(FVector(0.14f, 0.14f, 0.14f));

	EyeR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeR"));
	EyeR->SetupAttachment(Mesh);
	EyeR->SetRelativeLocation(FVector(38.f, -16.f, 26.f));
	EyeR->SetRelativeScale3D(FVector(0.14f, 0.14f, 0.14f));

	GlowLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("GlowLight"));
	GlowLight->SetupAttachment(Root);
	GlowLight->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	GlowLight->SetVisibility(false);
}

void AHostilePet::BeginPlay()
{
	Super::BeginPlay();
	Home = GetActorLocation();
	ApplyFromCatalog();
	WanderVel = FVector(FMath::FRandRange(-70.f, 70.f), FMath::FRandRange(-70.f, 70.f), 0.f);
}

void AHostilePet::Configure(EHolypawVillain Id)
{
	VillainId = Id;
	ApplyFromCatalog();
	Home = GetActorLocation();
}

FVillainDef AHostilePet::GetDef() const
{
	return HolypawCatalog::GetVillain(VillainId);
}

UStaticMesh* AHostilePet::MeshForShape(EVillainShape Shape) const
{
	switch (Shape)
	{
	case EVillainShape::Sphere: return SphereMesh ? SphereMesh : CubeMesh;
	case EVillainShape::Cone: return ConeMesh ? ConeMesh : CubeMesh;
	case EVillainShape::Cylinder: return CylMesh ? CylMesh : CubeMesh;
	default: return CubeMesh;
	}
}

void AHostilePet::ColorPart(UStaticMeshComponent* Comp, const FLinearColor& Color)
{
	HolypawLook::Paint(Comp, Color);
}

void AHostilePet::ApplyFromCatalog()
{
	const FVillainDef D = HolypawCatalog::GetVillain(VillainId);
	DisplayName = D.DisplayName;
	HPMax = D.HP;
	HP = D.HP;
	Attack = D.Attack;
	BaseAttack = D.Attack;
	bPhaseTwo = false;
	Rank = D.Rank;
	Special = D.Special;
	AggroRange = D.AggroRange;
	bBlocksFlee = D.bBlocksFlee;
	RespawnSeconds = D.RespawnSeconds;
	SetActorScale3D(FVector(D.Scale));
	BodyScale = D.Scale;

	if (UStaticMesh* Body = MeshForShape(D.Shape))
	{
		Mesh->SetStaticMesh(Body);
	}
	Mesh->SetRelativeScale3D(FVector(0.72f, 0.52f, 0.58f));
	ColorPart(Mesh, D.Color);

	HolypawLook::PrepPart(AccentMesh, ConeMesh ? ConeMesh : CubeMesh);
	HolypawLook::PrepPart(AccentMeshB, ConeMesh ? ConeMesh : CubeMesh);
	HolypawLook::PrepPart(CrestMesh, (D.Rank == EVillainRank::Boss || D.Rank == EVillainRank::WorldBoss)
		? (CylMesh ? CylMesh : CubeMesh)
		: (SphereMesh ? SphereMesh : CubeMesh));
	HolypawLook::PrepPart(Cloak, ConeMesh ? ConeMesh : CubeMesh);
	HolypawLook::PrepPart(HornR, ConeMesh ? ConeMesh : CubeMesh);
	HolypawLook::PrepPart(EyeL, SphereMesh);
	HolypawLook::PrepPart(EyeR, SphereMesh);
	if (ShapeMat)
	{
		for (UStaticMeshComponent* P : { AccentMesh, AccentMeshB, CrestMesh, Cloak, HornR, EyeL, EyeR })
		{
			if (P) { P->SetMaterial(0, ShapeMat); }
		}
	}

	UStaticMesh* Snout = ConeMesh ? ConeMesh : CubeMesh;
	UStaticMesh* Ear = ConeMesh ? ConeMesh : CubeMesh;
	UStaticMesh* Extra = (D.Rank == EVillainRank::Boss || D.Rank == EVillainRank::WorldBoss)
		? (CylMesh ? CylMesh : CubeMesh)
		: (SphereMesh ? SphereMesh : CubeMesh);

	if (AccentMesh && Snout)
	{
		AccentMesh->SetStaticMesh(Snout);
		AccentMesh->SetRelativeLocation(FVector(38.f, 0.f, 8.f));
		AccentMesh->SetRelativeRotation(FRotator(-75.f, 0.f, 0.f));
		AccentMesh->SetRelativeScale3D(FVector(0.22f, 0.18f, 0.35f));
		ColorPart(AccentMesh, D.AccentColor);
	}
	if (AccentMeshB && Ear)
	{
		AccentMeshB->SetStaticMesh(Ear);
		AccentMeshB->SetRelativeLocation(FVector(-8.f, 22.f, 36.f));
		AccentMeshB->SetRelativeRotation(FRotator(12.f, 18.f, 0.f));
		AccentMeshB->SetRelativeScale3D(FVector(0.16f, 0.12f, 0.28f));
		ColorPart(AccentMeshB, D.AccentColor);
	}
	if (HornR && Ear)
	{
		HornR->SetStaticMesh(Ear);
		HornR->SetRelativeLocation(FVector(-8.f, -22.f, 36.f));
		HornR->SetRelativeRotation(FRotator(12.f, -18.f, 0.f));
		HornR->SetRelativeScale3D(FVector(0.16f, 0.12f, 0.28f));
		ColorPart(HornR, D.AccentColor);
	}
	if (Cloak)
	{
		const bool bCloak = D.Rank == EVillainRank::Boss || D.Rank == EVillainRank::WorldBoss || D.Rank == EVillainRank::Elite;
		Cloak->SetHiddenInGame(!bCloak);
		if (bCloak)
		{
			ColorPart(Cloak, D.Color * 0.55f);
		}
	}
	if (CrestMesh && Extra)
	{
		CrestMesh->SetStaticMesh(Extra);
		CrestMesh->SetRelativeLocation(FVector(0.f, 0.f, 48.f));
		const bool bBig = D.Rank == EVillainRank::Boss || D.Rank == EVillainRank::WorldBoss;
		CrestMesh->SetRelativeScale3D(bBig ? FVector(0.28f, 0.28f, 0.45f) : FVector(0.18f, 0.18f, 0.18f));
		CrestMesh->SetHiddenInGame(!bBig && D.Rank != EVillainRank::Elite);
		ColorPart(CrestMesh, bBig ? FLinearColor(0.95f, 0.78f, 0.4f) : D.AccentColor);
	}

	ApplySignatureSilhouette(D);

	ColorPart(EyeL, HolypawLook::Button);
	ColorPart(EyeR, HolypawLook::Button);
	if (GlowLight)
	{
		const bool bGlow = D.Rank == EVillainRank::Boss || D.Rank == EVillainRank::WorldBoss || D.Rank == EVillainRank::Elite;
		GlowLight->SetVisibility(bGlow);
		if (bGlow)
		{
			HolypawLook::DressLanternLight(GlowLight, D.AccentColor);
			GlowLight->SetIntensity(D.Rank == EVillainRank::Minion ? 0.f : 700.f);
			GlowLight->SetAttenuationRadius(260.f);
		}
	}
}

void AHostilePet::ApplySignatureSilhouette(const FVillainDef& D)
{
	if (!AccentMesh || !AccentMeshB || !CrestMesh)
	{
		return;
	}

	auto HideCrest = [&]()
	{
		CrestMesh->SetHiddenInGame(true);
	};

	switch (D.Id)
	{
	case EHolypawVillain::RazorPetbot:
		Mesh->SetRelativeScale3D(FVector(0.55f, 0.55f, 0.9f));
		if (CubeMesh) AccentMesh->SetStaticMesh(CubeMesh);
		AccentMesh->SetRelativeLocation(FVector(42.f, 18.f, 20.f));
		AccentMesh->SetRelativeScale3D(FVector(0.45f, 0.08f, 0.08f));
		AccentMeshB->SetRelativeLocation(FVector(42.f, -18.f, 20.f));
		AccentMeshB->SetRelativeScale3D(FVector(0.45f, 0.08f, 0.08f));
		CrestMesh->SetHiddenInGame(false);
		CrestMesh->SetRelativeLocation(FVector(0.f, 0.f, 55.f));
		if (HornR) { HornR->SetHiddenInGame(true); }
		break;
	case EHolypawVillain::SaltCrab:
	case EHolypawVillain::HarborHook:
		Mesh->SetRelativeScale3D(FVector(0.9f, 0.7f, 0.4f));
		if (CubeMesh)
		{
			AccentMesh->SetStaticMesh(CubeMesh);
			AccentMeshB->SetStaticMesh(CubeMesh);
		}
		AccentMesh->SetRelativeLocation(FVector(30.f, 28.f, 8.f));
		AccentMesh->SetRelativeScale3D(FVector(0.35f, 0.12f, 0.12f));
		AccentMeshB->SetRelativeLocation(FVector(30.f, -28.f, 8.f));
		AccentMeshB->SetRelativeScale3D(FVector(0.35f, 0.12f, 0.12f));
		HideCrest();
		if (HornR) { HornR->SetHiddenInGame(true); }
		break;
	case EHolypawVillain::FrostMoth:
	case EHolypawVillain::AuroraWisp:
	case EHolypawVillain::BrineGull:
		Mesh->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.45f));
		if (ConeMesh)
		{
			AccentMesh->SetStaticMesh(ConeMesh);
			AccentMeshB->SetStaticMesh(ConeMesh);
		}
		AccentMesh->SetRelativeLocation(FVector(0.f, 32.f, 10.f));
		AccentMesh->SetRelativeRotation(FRotator(0.f, 0.f, 70.f));
		AccentMesh->SetRelativeScale3D(FVector(0.15f, 0.55f, 0.08f));
		AccentMeshB->SetRelativeLocation(FVector(0.f, -32.f, 10.f));
		AccentMeshB->SetRelativeRotation(FRotator(0.f, 0.f, -70.f));
		AccentMeshB->SetRelativeScale3D(FVector(0.15f, 0.55f, 0.08f));
		HideCrest();
		if (HornR) { HornR->SetHiddenInGame(true); }
		break;
	case EHolypawVillain::SilkMagistrate:
	case EHolypawVillain::HarvestOverseer:
	case EHolypawVillain::BrineWarden:
	case EHolypawVillain::BogKing:
	case EHolypawVillain::AuroraWarden:
		Mesh->SetRelativeScale3D(FVector(0.85f, 0.7f, 1.05f));
		CrestMesh->SetHiddenInGame(false);
		CrestMesh->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
		CrestMesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.55f));
		ColorPart(CrestMesh, FLinearColor(0.98f, 0.82f, 0.4f));
		break;
	case EHolypawVillain::VelvetTyrant:
		Mesh->SetRelativeScale3D(FVector(1.0f, 0.75f, 0.85f));
		CrestMesh->SetHiddenInGame(false);
		if (ConeMesh) CrestMesh->SetStaticMesh(ConeMesh);
		CrestMesh->SetRelativeLocation(FVector(0.f, 0.f, 72.f));
		CrestMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.7f));
		ColorPart(CrestMesh, FLinearColor(0.95f, 0.72f, 0.35f));
		break;
	case EHolypawVillain::Unmaker:
		Mesh->SetRelativeScale3D(FVector(0.6f, 0.6f, 1.35f));
		CrestMesh->SetHiddenInGame(false);
		CrestMesh->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
		CrestMesh->SetRelativeScale3D(FVector(0.35f, 0.35f, 0.9f));
		ColorPart(Mesh, FLinearColor(0.08f, 0.07f, 0.1f));
		ColorPart(CrestMesh, FLinearColor(0.95f, 0.32f, 0.5f));
		break;
	case EHolypawVillain::VoidRat:
	case EHolypawVillain::SewerVoidRat:
	case EHolypawVillain::DockRat:
	case EHolypawVillain::GrainMite:
	case EHolypawVillain::ButtonThief:
		Mesh->SetRelativeScale3D(FVector(0.5f, 0.38f, 0.32f));
		AccentMesh->SetRelativeLocation(FVector(28.f, 0.f, 4.f));
		AccentMesh->SetRelativeScale3D(FVector(0.28f, 0.12f, 0.12f));
		HideCrest();
		if (HornR) { HornR->SetHiddenInGame(true); }
		break;
	default:
		break;
	}
}

FText AHostilePet::GetPrompt() const
{
	if (bDefeated)
	{
		return FText::GetEmpty();
	}
	if (IsBoss())
	{
		return FText::Format(NSLOCTEXT("Holypaw", "FaceBoss", "E  Face {0}"), DisplayName);
	}
	return FText::Format(NSLOCTEXT("Holypaw", "FaceVillain", "E  Challenge {0}"), DisplayName);
}

bool AHostilePet::Interact(AHolypawCharacter* InstigatorPawn)
{
	if (!InstigatorPawn || bDefeated)
	{
		return false;
	}
	InstigatorPawn->StartBattle(this);
	return true;
}

void AHostilePet::PulseHit()
{
	HitPulse = 1.f;
}

bool AHostilePet::TryEnterPhaseTwo()
{
	if (bPhaseTwo || bDefeated || (!IsBoss() && Rank != EVillainRank::Elite))
	{
		return false;
	}
	if (HP * 2 > HPMax)
	{
		return false;
	}
	bPhaseTwo = true;
	Attack = BaseAttack + FMath::Max(3, BaseAttack / 3);
	PulseHit();
	return true;
}

FString AHostilePet::GetPhaseLine() const
{
	return HolypawBattle::BossPhaseLine(VillainId, DisplayName.ToString());
}

void AHostilePet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HitPulse > 0.f)
	{
		HitPulse = FMath::Max(0.f, HitPulse - DeltaSeconds * 3.2f);
		SetActorScale3D(FVector(BodyScale * (1.f + HitPulse * 0.28f)));
	}
	else if (BodyScale > 0.f)
	{
		SetActorScale3D(FVector(BodyScale));
	}
	if (GlowLight && GlowLight->IsVisible())
	{
		const float Pulse = 0.75f + 0.25f * FMath::Sin(GetWorld()->GetTimeSeconds() * 3.4f);
		GlowLight->SetIntensity((bPhaseTwo ? 1400.f : 700.f) * Pulse);
	}

	if (bDefeated)
	{
		return;
	}

	AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!Pawn || Pawn->IsBusy())
	{
		return;
	}

	const float Dist = FVector::Dist(GetActorLocation(), Pawn->GetActorLocation());
	const float Touch = IsBoss() ? 170.f : 140.f;
	if (Dist < Touch && Pawn->GetInvuln() <= 0.f)
	{
		Pawn->StartBattle(this);
		return;
	}

	const float Speed = IsBoss() ? 220.f : (Rank == EVillainRank::Elite ? 300.f : 280.f);
	if (Dist < AggroRange)
	{
		const FVector Dir = (Pawn->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		AddActorWorldOffset(Dir * Speed * DeltaSeconds);
	}
	else
	{
		FVector Next = GetActorLocation() + WanderVel * DeltaSeconds;
		const float Leash = IsBoss() ? 280.f : 520.f;
		if (FVector::Dist2D(Next, Home) > Leash)
		{
			WanderVel *= -1.f;
		}
		else
		{
			SetActorLocation(Next);
		}
	}
}

void AHostilePet::Defeat(bool bGrantRewards)
{
	bDefeated = true;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	if (bGrantRewards)
	{
		if (AHolypawCharacter* Pawn = Cast<AHolypawCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
		{
			Pawn->GrantKillRewards(this);
		}
		GetWorldTimerManager().SetTimer(RespawnTimer, this, &AHostilePet::RespawnLater, RespawnSeconds, false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(RespawnTimer, this, &AHostilePet::RespawnLater, RespawnSeconds * 0.6f, false);
	}
}

void AHostilePet::RespawnLater()
{
	bDefeated = false;
	HP = HPMax;
	bPhaseTwo = false;
	Attack = BaseAttack;
	HitPulse = 0.f;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	const FVector Jitter(FMath::FRandRange(-200.f, 200.f), FMath::FRandRange(-200.f, 200.f), 0.f);
	SetActorLocation(Home + Jitter);
}
