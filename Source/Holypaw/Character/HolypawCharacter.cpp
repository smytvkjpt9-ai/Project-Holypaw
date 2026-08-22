#include "Character/HolypawCharacter.h"
#include "HolypawCatalog.h"
#include "Components/AffectionComponent.h"
#include "Components/SkillTreeComponent.h"
#include "Components/PartyComponent.h"
#include "Components/MissionComponent.h"
#include "Actors/WildFluffy.h"
#include "Actors/HugHuman.h"
#include "Actors/HostilePet.h"
#include "Actors/HolypawInteractable.h"
#include "Actors/HolypawShrine.h"
#include "Combat/HolypawBattleMath.h"
#include "Combat/HolypawBattleDirector.h"
#include "Narrative/HolypawDialogueVM.h"
#include "HolypawWorldBuilder.h"
#include "HolypawGameInstance.h"
#include "Save/HolypawSaveCodec.h"
#include "Audio/HolypawAudio.h"
#include "Faith/HolypawFaithSim.h"
#include "Anim/HolypawProcAnim.h"
#include "Look/HolypawLook.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/InputComponent.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"

struct FHolypawTeddyMotion
{
	HolypawAnim::FTeddyState Anim;
	HolypawAnim::FTeddyRest Rest;
	HolypawAnim::FPartyState Party;
};

static HolypawAnim::FTeddyParts MakeTeddyParts(AHolypawCharacter& Pawn)
{
	HolypawAnim::FTeddyParts P;
	P.Body = Pawn.BodyMesh;
	P.Head = Pawn.HeadMesh;
	P.EarL = Pawn.EarL;
	P.EarR = Pawn.EarR;
	P.PawL = Pawn.PawL;
	P.PawR = Pawn.PawR;
	P.Snout = Pawn.Snout;
	P.Belly = Pawn.Belly;
	P.EyeL = Pawn.EyeL;
	P.EyeR = Pawn.EyeR;
	P.Halo = Pawn.HaloMesh;
	return P;
}

AHolypawCharacter::AHolypawCharacter()
{
	TeddyMotion = MakeUnique<FHolypawTeddyMotion>();
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 88.f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->JumpZVelocity = 520.f;
	GetCharacterMovement()->AirControl = 0.2f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = ExploreArm;
	SpringArm->SocketOffset = FVector(0.f, 52.f, 78.f);
	SpringArm->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->ProbeSize = 12.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 18.f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 22.f;
	SpringArm->CameraLagMaxDistance = 64.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 67.f;
	Camera->PostProcessBlendWeight = 0.12f;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -18.f));
	BodyMesh->SetRelativeScale3D(FVector(0.92f, 0.78f, 0.82f));

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	HeadMesh->SetupAttachment(RootComponent);
	HeadMesh->SetRelativeLocation(FVector(2.f, 0.f, 50.f));
	HeadMesh->SetRelativeScale3D(FVector(0.68f, 0.66f, 0.64f));

	HaloMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Halo"));
	HaloMesh->SetupAttachment(HeadMesh);
	HaloMesh->SetRelativeLocation(FVector(0.f, 0.f, 78.f));
	HaloMesh->SetRelativeScale3D(FVector(0.48f, 0.48f, 0.08f));
	HaloMesh->SetHiddenInGame(true);

	EarL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EarL"));
	EarL->SetupAttachment(HeadMesh);
	EarR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EarR"));
	EarR->SetupAttachment(HeadMesh);
	InnerEarL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InnerEarL"));
	InnerEarL->SetupAttachment(EarL);
	InnerEarR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InnerEarR"));
	InnerEarR->SetupAttachment(EarR);
	Snout = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Snout"));
	Snout->SetupAttachment(HeadMesh);
	Nose = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Nose"));
	Nose->SetupAttachment(Snout);
	Belly = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Belly"));
	Belly->SetupAttachment(BodyMesh);
	EyeL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeL"));
	EyeL->SetupAttachment(HeadMesh);
	EyeR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeR"));
	EyeR->SetupAttachment(HeadMesh);
	HighlightL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightL"));
	HighlightL->SetupAttachment(EyeL);
	HighlightR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightR"));
	HighlightR->SetupAttachment(EyeR);
	CheekL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheekL"));
	CheekL->SetupAttachment(HeadMesh);
	CheekR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheekR"));
	CheekR->SetupAttachment(HeadMesh);
	ArmL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL"));
	ArmL->SetupAttachment(BodyMesh);
	ArmR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR"));
	ArmR->SetupAttachment(BodyMesh);
	PawL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawL"));
	PawL->SetupAttachment(ArmL);
	PawR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawR"));
	PawR->SetupAttachment(ArmR);
	LegL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegL"));
	LegL->SetupAttachment(BodyMesh);
	LegR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegR"));
	LegR->SetupAttachment(BodyMesh);
	FootL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootL"));
	FootL->SetupAttachment(LegL);
	FootR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootR"));
	FootR->SetupAttachment(LegR);
	Ribbon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ribbon"));
	Ribbon->SetupAttachment(BodyMesh);
	BowL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BowL"));
	BowL->SetupAttachment(Ribbon);
	BowR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BowR"));
	BowR->SetupAttachment(Ribbon);
	BrowL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BrowL"));
	BrowL->SetupAttachment(HeadMesh);
	BrowR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BrowR"));
	BrowR->SetupAttachment(HeadMesh);
	Tail = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tail"));
	Tail->SetupAttachment(BodyMesh);
	HaloLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("HaloLight"));
	HaloLight->SetupAttachment(HaloMesh);
	HaloLight->SetVisibility(false);

	KeyFill = CreateDefaultSubobject<UPointLightComponent>(TEXT("KeyFill"));
	KeyFill->SetupAttachment(RootComponent);
	KeyFill->SetRelativeLocation(FVector(110.f, 60.f, 150.f));
	KeyFill->SetIntensity(14000.f);
	KeyFill->SetAttenuationRadius(2200.f);
	KeyFill->SetLightColor(FLinearColor(1.f, 0.94f, 0.86f));
	KeyFill->SetCastShadows(false);
	KeyFill->SetUseInverseSquaredFalloff(true);
	KeyFill->SetSpecularScale(0.25f);

	EarL->SetRelativeLocation(FVector(-4.f, 20.f, 36.f));
	EarL->SetRelativeRotation(FRotator(18.f, 6.f, -20.f));
	EarL->SetRelativeScale3D(FVector(0.15f, 0.12f, 0.36f));
	EarR->SetRelativeLocation(FVector(-4.f, -20.f, 36.f));
	EarR->SetRelativeRotation(FRotator(18.f, -6.f, 20.f));
	EarR->SetRelativeScale3D(FVector(0.15f, 0.12f, 0.36f));
	InnerEarL->SetRelativeLocation(FVector(8.f, 0.f, 6.f));
	InnerEarL->SetRelativeScale3D(FVector(0.48f, 0.40f, 0.50f));
	InnerEarR->SetRelativeLocation(FVector(8.f, 0.f, 6.f));
	InnerEarR->SetRelativeScale3D(FVector(0.48f, 0.40f, 0.50f));
	Snout->SetRelativeLocation(FVector(18.f, 0.f, -6.f));
	Snout->SetRelativeScale3D(FVector(0.22f, 0.20f, 0.16f));
	Nose->SetRelativeLocation(FVector(14.f, 0.f, 4.f));
	Nose->SetRelativeScale3D(FVector(0.20f, 0.24f, 0.16f));
	Belly->SetRelativeLocation(FVector(12.f, 0.f, -2.f));
	Belly->SetRelativeScale3D(FVector(0.60f, 0.50f, 0.50f));
	EyeL->SetRelativeLocation(FVector(22.f, 6.5f, 3.f));
	EyeL->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.12f));
	EyeR->SetRelativeLocation(FVector(22.f, -6.5f, 3.f));
	EyeR->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.12f));
	HighlightL->SetRelativeLocation(FVector(8.f, 4.f, 6.f));
	HighlightL->SetRelativeScale3D(FVector(0.30f, 0.30f, 0.30f));
	HighlightR->SetRelativeLocation(FVector(8.f, 4.f, 6.f));
	HighlightR->SetRelativeScale3D(FVector(0.30f, 0.30f, 0.30f));
	CheekL->SetRelativeLocation(FVector(15.f, 11.f, -4.f));
	CheekL->SetRelativeScale3D(FVector(0.11f, 0.09f, 0.07f));
	CheekR->SetRelativeLocation(FVector(15.f, -11.f, -4.f));
	CheekR->SetRelativeScale3D(FVector(0.11f, 0.09f, 0.07f));
	ArmL->SetRelativeLocation(FVector(2.f, 36.f, 4.f));
	ArmL->SetRelativeScale3D(FVector(0.24f, 0.20f, 0.34f));
	ArmR->SetRelativeLocation(FVector(2.f, -36.f, 4.f));
	ArmR->SetRelativeScale3D(FVector(0.24f, 0.20f, 0.34f));
	PawL->SetRelativeLocation(FVector(6.f, 4.f, -18.f));
	PawL->SetRelativeScale3D(FVector(0.62f, 0.58f, 0.40f));
	PawR->SetRelativeLocation(FVector(6.f, -4.f, -18.f));
	PawR->SetRelativeScale3D(FVector(0.62f, 0.58f, 0.40f));
	LegL->SetRelativeLocation(FVector(2.f, 14.f, -30.f));
	LegL->SetRelativeScale3D(FVector(0.24f, 0.22f, 0.26f));
	LegR->SetRelativeLocation(FVector(2.f, -14.f, -30.f));
	LegR->SetRelativeScale3D(FVector(0.24f, 0.22f, 0.26f));
	FootL->SetRelativeLocation(FVector(10.f, 0.f, -16.f));
	FootL->SetRelativeScale3D(FVector(0.95f, 0.80f, 0.40f));
	FootR->SetRelativeLocation(FVector(10.f, 0.f, -16.f));
	FootR->SetRelativeScale3D(FVector(0.95f, 0.80f, 0.40f));
	Ribbon->SetRelativeLocation(FVector(2.f, 0.f, 24.f));
	Ribbon->SetRelativeScale3D(FVector(0.22f, 0.22f, 0.16f));
	BowL->SetRelativeLocation(FVector(-2.f, 14.f, 0.f));
	BowL->SetRelativeScale3D(FVector(0.55f, 0.95f, 0.45f));
	BowR->SetRelativeLocation(FVector(-2.f, -14.f, 0.f));
	BowR->SetRelativeScale3D(FVector(0.55f, 0.95f, 0.45f));
	BrowL->SetRelativeLocation(FVector(22.f, 8.f, 16.f));
	BrowL->SetRelativeRotation(FRotator(0.f, 0.f, -10.f));
	BrowL->SetRelativeScale3D(FVector(0.09f, 0.035f, 0.028f));
	BrowR->SetRelativeLocation(FVector(22.f, -8.f, 16.f));
	BrowR->SetRelativeRotation(FRotator(0.f, 0.f, 10.f));
	BrowR->SetRelativeScale3D(FVector(0.09f, 0.035f, 0.028f));
	Tail->SetRelativeLocation(FVector(-24.f, 0.f, -6.f));
	Tail->SetRelativeScale3D(FVector(0.22f, 0.20f, 0.18f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TorusFinder(TEXT("/Engine/BasicShapes/Torus.Torus"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeFinder(TEXT("/Engine/BasicShapes/Cone.Cone"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (SphereFinder.Succeeded())
	{
		SphereMesh = SphereFinder.Object;
	}
	if (ConeFinder.Succeeded())
	{
		ConeMesh = ConeFinder.Object;
	}
	if (MatFinder.Succeeded())
	{
		ShapeMat = MatFinder.Object;
	}

	HolypawLook::PrepPart(BodyMesh, SphereMesh);
	HolypawLook::PrepPart(HeadMesh, SphereMesh);
	HolypawLook::PrepPart(Snout, SphereMesh);
	HolypawLook::PrepPart(Nose, SphereMesh);
	HolypawLook::PrepPart(PawL, SphereMesh);
	HolypawLook::PrepPart(PawR, SphereMesh);
	HolypawLook::PrepPart(Belly, SphereMesh);
	HolypawLook::PrepPart(EyeL, SphereMesh);
	HolypawLook::PrepPart(EyeR, SphereMesh);
	HolypawLook::PrepPart(HighlightL, SphereMesh);
	HolypawLook::PrepPart(HighlightR, SphereMesh);
	HolypawLook::PrepPart(CheekL, SphereMesh);
	HolypawLook::PrepPart(CheekR, SphereMesh);
	HolypawLook::PrepPart(ArmL, SphereMesh);
	HolypawLook::PrepPart(ArmR, SphereMesh);
	HolypawLook::PrepPart(LegL, SphereMesh);
	HolypawLook::PrepPart(LegR, SphereMesh);
	HolypawLook::PrepPart(FootL, SphereMesh);
	HolypawLook::PrepPart(FootR, SphereMesh);
	HolypawLook::PrepPart(Ribbon, SphereMesh);
	HolypawLook::PrepPart(BowL, SphereMesh);
	HolypawLook::PrepPart(BowR, SphereMesh);
	HolypawLook::PrepPart(Tail, SphereMesh);
	HolypawLook::PrepPart(BrowL, SphereMesh);
	HolypawLook::PrepPart(BrowR, SphereMesh);
	HolypawLook::PrepPart(InnerEarL, SphereMesh);
	HolypawLook::PrepPart(InnerEarR, SphereMesh);
	HolypawLook::PrepPart(EarL, ConeMesh ? ConeMesh : SphereMesh);
	HolypawLook::PrepPart(EarR, ConeMesh ? ConeMesh : SphereMesh);
	HolypawLook::PrepPart(HaloMesh, TorusFinder.Succeeded() ? TorusFinder.Object.Get() : SphereMesh);
	if (ShapeMat)
	{
		TArray<UStaticMeshComponent*> Parts = {
			BodyMesh, HeadMesh, HaloMesh, EarL, EarR, InnerEarL, InnerEarR, Snout, Nose,
			PawL, PawR, Belly, EyeL, EyeR, HighlightL, HighlightR, CheekL, CheekR,
			ArmL, ArmR, LegL, LegR, FootL, FootR, Ribbon, BowL, BowR, Tail, BrowL, BrowR
		};
		for (UStaticMeshComponent* P : Parts)
		{
			if (P)
			{
				P->SetMaterial(0, ShapeMat);
			}
		}
	}
	HolypawLook::TagSocket(EarL, TEXT("earL"));
	HolypawLook::TagSocket(EarR, TEXT("earR"));
	HolypawLook::TagSocket(Snout, TEXT("snout"));
	HolypawLook::TagSocket(HaloMesh, TEXT("halo"));
	HolypawLook::TagSocket(PawL, TEXT("pawL"));
	HolypawLook::TagSocket(PawR, TEXT("pawR"));
	HolypawLook::TagSocket(HeadMesh, TEXT("head"));
	HolypawLook::TagSocket(BodyMesh, TEXT("body"));
	HolypawLook::TagSocket(Belly, TEXT("belly"));
	HolypawLook::TagSocket(Nose, TEXT("nose"));
	HolypawLook::TagSocket(Ribbon, TEXT("ribbon"));
	HolypawLook::TagSocket(Tail, TEXT("tail"));
	if (HaloLight)
	{
		HolypawLook::DressLanternLight(HaloLight, HolypawLook::GoldWarm);
		HaloLight->SetIntensity(0.f);
		HaloLight->SetAttenuationRadius(220.f);
	}

	Affection = CreateDefaultSubobject<UAffectionComponent>(TEXT("Affection"));
	Skills = CreateDefaultSubobject<USkillTreeComponent>(TEXT("Skills"));
	Party = CreateDefaultSubobject<UPartyComponent>(TEXT("Party"));
	Story = CreateDefaultSubobject<UHolypawMissionComponent>(TEXT("Story"));
}

AHolypawCharacter::~AHolypawCharacter() = default;

void AHolypawCharacter::BeginPlay()
{
	Super::BeginPlay();
	Colorize(BodyMesh, HolypawLook::Fur);
	Colorize(HeadMesh, HolypawLook::FurLight);
	Colorize(HaloMesh, HolypawLook::GoldWarm);
	Colorize(EarL, HolypawLook::Fur);
	Colorize(EarR, HolypawLook::Fur);
	Colorize(InnerEarL, HolypawLook::InnerEar);
	Colorize(InnerEarR, HolypawLook::InnerEar);
	Colorize(Snout, HolypawLook::Snout);
	Colorize(Nose, HolypawLook::Pad);
	Colorize(PawL, HolypawLook::Fur);
	Colorize(PawR, HolypawLook::Fur);
	Colorize(Belly, HolypawLook::Cream);
	Colorize(EyeL, HolypawLook::Button);
	Colorize(EyeR, HolypawLook::Button);
	Colorize(HighlightL, HolypawLook::Catchlight);
	Colorize(HighlightR, HolypawLook::Catchlight);
	Colorize(CheekL, HolypawLook::InnerEar);
	Colorize(CheekR, HolypawLook::InnerEar);
	Colorize(ArmL, HolypawLook::Fur);
	Colorize(ArmR, HolypawLook::Fur);
	Colorize(LegL, HolypawLook::Fur);
	Colorize(LegR, HolypawLook::Fur);
	Colorize(FootL, HolypawLook::Pad);
	Colorize(FootR, HolypawLook::Pad);
	Colorize(Ribbon, HolypawLook::Rose);
	Colorize(BowL, HolypawLook::Rose);
	Colorize(BowR, HolypawLook::Rose);
	Colorize(Tail, HolypawLook::Fur);
	Colorize(BrowL, HolypawLook::Fur);
	Colorize(BrowR, HolypawLook::Fur);
	HolypawAnim::CaptureTeddyRest(TeddyMotion->Rest, MakeTeddyParts(*this));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	UnlockTravel(EHolypawZone::ForestCottage);
	Mode = EHolypawPawnMode::Title;
	if (ArmL) { ArmLBase = ArmL->GetRelativeLocation(); ArmLRot = ArmL->GetRelativeRotation(); }
	if (ArmR) { ArmRBase = ArmR->GetRelativeLocation(); ArmRRot = ArmR->GetRelativeRotation(); }
	if (LegL) { LegLBase = LegL->GetRelativeLocation(); LegLRot = LegL->GetRelativeRotation(); }
	if (LegR) { LegRBase = LegR->GetRelativeLocation(); LegRRot = LegR->GetRelativeRotation(); }
	if (FootL) { FootLBase = FootL->GetRelativeLocation(); FootLRot = FootL->GetRelativeRotation(); }
	if (FootR) { FootRBase = FootR->GetRelativeLocation(); FootRRot = FootR->GetRelativeRotation(); }
	if (HighlightL) { HighlightLScale = HighlightL->GetRelativeScale3D(); }
	if (HighlightR) { HighlightRScale = HighlightR->GetRelativeScale3D(); }
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
		FRotator View = PC->GetControlRotation();
		View.Pitch = -16.f;
		PC->SetControlRotation(View);
	}
	HolypawLook::ApplyViewExposure(Camera);
	PlayCue(TEXT("Title"));
	Toast(TEXT("The Fluffy Ascendancy — pick a slot. The porch is already under you."));
}

void AHolypawCharacter::Colorize(UStaticMeshComponent* Comp, const FLinearColor& Color)
{
	HolypawLook::Paint(Comp, Color);
}

void AHolypawCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHolypawCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHolypawCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AHolypawCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AHolypawCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AHolypawCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AHolypawCharacter::LookUpAtRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AHolypawCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AHolypawCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Miracle"), IE_Pressed, this, &AHolypawCharacter::TryMiracle);
	PlayerInputComponent->BindAction(TEXT("Skills"), IE_Pressed, this, &AHolypawCharacter::ToggleSkills);
	PlayerInputComponent->BindAction(TEXT("Party"), IE_Pressed, this, &AHolypawCharacter::ToggleParty);
	PlayerInputComponent->BindAction(TEXT("Map"), IE_Pressed, this, &AHolypawCharacter::ToggleMap);
	PlayerInputComponent->BindAction(TEXT("Codex"), IE_Pressed, this, &AHolypawCharacter::ToggleCodex);
	PlayerInputComponent->BindAction(TEXT("Journal"), IE_Pressed, this, &AHolypawCharacter::ToggleJournal);
	PlayerInputComponent->BindAction(TEXT("NextTree"), IE_Pressed, this, &AHolypawCharacter::CycleSkillTree);
	PlayerInputComponent->BindAction(TEXT("CloseUI"), IE_Pressed, this, &AHolypawCharacter::CloseOrJump);
	PlayerInputComponent->BindAction(TEXT("BattleSlap"), IE_Pressed, this, &AHolypawCharacter::BattleSlap);
	PlayerInputComponent->BindAction(TEXT("BattleBeam"), IE_Pressed, this, &AHolypawCharacter::BattleBeam);
	PlayerInputComponent->BindAction(TEXT("BattleParty"), IE_Pressed, this, &AHolypawCharacter::BattlePartyAtk);
	PlayerInputComponent->BindAction(TEXT("BattleFlee"), IE_Pressed, this, &AHolypawCharacter::BattleFlee);
	PlayerInputComponent->BindAction(TEXT("Skill5"), IE_Pressed, this, &AHolypawCharacter::Skill5);
	PlayerInputComponent->BindAction(TEXT("Skill6"), IE_Pressed, this, &AHolypawCharacter::Skill6);
	PlayerInputComponent->BindAction(TEXT("TitleConfirm"), IE_Pressed, this, &AHolypawCharacter::TitleConfirmPressed);
	PlayerInputComponent->BindAction(TEXT("TitleLoad"), IE_Pressed, this, &AHolypawCharacter::TitleLoadPressed);
	PlayerInputComponent->BindAction(TEXT("QuickSave"), IE_Pressed, this, &AHolypawCharacter::QuickSavePressed);
	PlayerInputComponent->BindAction(TEXT("Settings"), IE_Pressed, this, &AHolypawCharacter::MutePressed);
	PlayerInputComponent->BindAction(TEXT("TitleMenu"), IE_Pressed, this, &AHolypawCharacter::TitleMenuPressed);
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &AHolypawCharacter::InventoryPressed);
}

void AHolypawCharacter::MoveForward(float Value)
{
	if (Mode != EHolypawPawnMode::Play || FMath::IsNearlyZero(Value) || HolypawAnim::IsWrapLocked(TeddyMotion->Anim))
	{
		return;
	}
	const FRotator Yaw(0.f, GetControlRotation().Yaw, 0.f);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), Value);
}

void AHolypawCharacter::MoveRight(float Value)
{
	if (Mode != EHolypawPawnMode::Play || FMath::IsNearlyZero(Value) || HolypawAnim::IsWrapLocked(TeddyMotion->Anim))
	{
		return;
	}
	const FRotator Yaw(0.f, GetControlRotation().Yaw, 0.f);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), Value);
}

void AHolypawCharacter::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * 45.f * GetWorld()->GetDeltaSeconds());
}

void AHolypawCharacter::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value * 45.f * GetWorld()->GetDeltaSeconds());
}

void AHolypawCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		if (Mode == EHolypawPawnMode::Play)
		{
			GI->PlaySeconds += DeltaSeconds;
		}
	}
	if (HugLock > 0.f)
	{
		HugLock -= DeltaSeconds;
	}
	if (Invuln > 0.f)
	{
		Invuln -= DeltaSeconds;
	}
	if (ToastTime > 0.f)
	{
		ToastTime -= DeltaSeconds;
	}
	if (DamagePopupTime > 0.f)
	{
		DamagePopupTime -= DeltaSeconds;
	}
	TickProcAnim(DeltaSeconds);
	if (SpringArm)
	{
		const float Want = Mode == EHolypawPawnMode::Battle ? BattleArm : ExploreArm;
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, Want, DeltaSeconds, 6.f);
	}
	HolypawLook::DressCamera(Camera, Mode == EHolypawPawnMode::Battle, DeltaSeconds);
	if (HaloLight)
	{
		const bool Lit = HaloMesh && !HaloMesh->bHiddenInGame;
		HaloLight->SetVisibility(Lit);
		HaloLight->SetIntensity(Lit ? 900.f : 0.f);
	}
	if (KeyFill)
	{
		const bool bPlayView = Mode == EHolypawPawnMode::Play || Mode == EHolypawPawnMode::Title;
		KeyFill->SetVisibility(bPlayView);
		KeyFill->SetIntensity(bPlayView ? 14000.f : 0.f);
	}
	if (Mode == EHolypawPawnMode::Play)
	{
		UpdateZone();
		if (UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
		{
			if (GI->IsDusk() && !bHeardChoirHour)
			{
				bHeardChoirHour = true;
				const bool bCity = HolypawCatalog::IsCityZone(CurrentZone);
				const int32 Hearts = GetCityHearts(CurrentZone);
				Toast(HolypawFaith::DuskLine(Hearts, bCity));
				if (HolypawFaith::ChoirOwnsDusk(Hearts) && bCity)
				{
					PlayCue(TEXT("DuskHymn"));
				}
				else
				{
					PlayCue(TEXT("Talk"));
				}
			}
			if (!GI->IsDusk())
			{
				bHeardChoirHour = false;
			}
		}
		if (AActor* Near = FindNearestInteractable(420.f))
		{
			if (AHolypawInteractable* I = Cast<AHolypawInteractable>(Near))
			{
				Prompt = I->GetPrompt();
			}
		}
		else
		{
			Prompt = FText::GetEmpty();
		}
	}
	SyncFollowers(DeltaSeconds);
}

void AHolypawCharacter::SyncFollowers(float DeltaSeconds)
{
	HolypawAnim::TickParty(TeddyMotion->Party, DeltaSeconds);
	Trail.Insert(GetActorLocation(), 0);
	if (Trail.Num() > 80)
	{
		Trail.SetNum(80);
	}

	const int32 Wanted = Party ? Party->Members.Num() : 0;
	while (Followers.Num() < Wanted && SphereMesh)
	{
		FHolypawFollowerVisual V;
		const int32 Idx = Followers.Num();
		const FLinearColor Col = Party->Members.IsValidIndex(Idx) ? Party->Members[Idx].Color : HolypawLook::Mint;
		V.Body = MakeFollowerPart(SphereMesh, Col);
		V.EarL = MakeFollowerPart(ConeMesh ? ConeMesh : SphereMesh, Col * 0.78f, V.Body);
		V.EarR = MakeFollowerPart(ConeMesh ? ConeMesh : SphereMesh, Col * 0.78f, V.Body);
		V.Tail = MakeFollowerPart(SphereMesh, Col, V.Body);
		V.EyeL = MakeFollowerPart(SphereMesh, HolypawLook::Button, V.Body);
		V.EyeR = MakeFollowerPart(SphereMesh, HolypawLook::Button, V.Body);
		V.Snout = MakeFollowerPart(SphereMesh, HolypawLook::Cream, V.Body);
		if (V.Body)
		{
			V.Body->SetWorldScale3D(FVector(0.38f, 0.30f, 0.30f));
		}
		if (V.EarL)
		{
			V.EarL->SetRelativeLocation(FVector(0.f, 36.f, 48.f));
			V.EarL->SetRelativeRotation(FRotator(12.f, 0.f, -16.f));
			V.EarL->SetRelativeScale3D(FVector(0.32f, 0.24f, 0.58f));
		}
		if (V.EarR)
		{
			V.EarR->SetRelativeLocation(FVector(0.f, -36.f, 48.f));
			V.EarR->SetRelativeRotation(FRotator(12.f, 0.f, 16.f));
			V.EarR->SetRelativeScale3D(FVector(0.32f, 0.24f, 0.58f));
		}
		if (V.Tail)
		{
			V.Tail->SetRelativeLocation(FVector(-38.f, 0.f, 4.f));
			V.Tail->SetRelativeScale3D(FVector(0.38f, 0.32f, 0.32f));
		}
		if (V.EyeL)
		{
			V.EyeL->SetRelativeLocation(FVector(28.f, 10.f, 10.f));
			V.EyeL->SetRelativeScale3D(FVector(0.16f, 0.16f, 0.16f));
		}
		if (V.EyeR)
		{
			V.EyeR->SetRelativeLocation(FVector(28.f, -10.f, 10.f));
			V.EyeR->SetRelativeScale3D(FVector(0.16f, 0.16f, 0.16f));
		}
		if (V.Snout)
		{
			V.Snout->SetRelativeLocation(FVector(32.f, 0.f, 0.f));
			V.Snout->SetRelativeScale3D(FVector(0.28f, 0.22f, 0.20f));
		}
		Followers.Add(V);
	}
	while (Followers.Num() > Wanted)
	{
		FHolypawFollowerVisual V = Followers.Pop();
		if (V.Body) { V.Body->DestroyComponent(); }
		if (V.EarL) { V.EarL->DestroyComponent(); }
		if (V.EarR) { V.EarR->DestroyComponent(); }
		if (V.Tail) { V.Tail->DestroyComponent(); }
		if (V.EyeL) { V.EyeL->DestroyComponent(); }
		if (V.EyeR) { V.EyeR->DestroyComponent(); }
		if (V.Snout) { V.Snout->DestroyComponent(); }
	}

	for (int32 I = 0; I < Followers.Num(); ++I)
	{
		FHolypawFollowerVisual& V = Followers[I];
		if (!V.Body)
		{
			continue;
		}
		const HolypawAnim::FPartySlotPose Pose = HolypawAnim::EvaluateParty(
			Trail, I, TeddyMotion->Party.Clock, TeddyMotion->Party.CelebrateT, GetActorLocation(), GetActorForwardVector());
		const FVector NewLoc = FMath::VInterpTo(V.Body->GetComponentLocation(), Pose.Location, DeltaSeconds, 10.f);
		V.Body->SetWorldLocation(NewLoc);
		V.Body->SetWorldScale3D(Pose.Scale);
		V.Body->SetWorldRotation(Pose.Rot);
		if (V.EarL)
		{
			V.EarL->SetRelativeRotation(FRotator(12.f + FMath::Sin(TeddyMotion->Party.Clock * 6.f + I) * 10.f, 0.f, -16.f));
		}
		if (V.EarR)
		{
			V.EarR->SetRelativeRotation(FRotator(12.f + FMath::Sin(TeddyMotion->Party.Clock * 6.f + I + 1.f) * 10.f, 0.f, 16.f));
		}
	}
}

UStaticMeshComponent* AHolypawCharacter::MakeFollowerPart(UStaticMesh* PartMesh, const FLinearColor& Color, USceneComponent* Attach)
{
	UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(this);
	if (!Comp)
	{
		return nullptr;
	}
	HolypawLook::PrepPart(Comp, PartMesh);
	Comp->SetMobility(EComponentMobility::Movable);
	Comp->CreationMethod = EComponentCreationMethod::Instance;
	AddInstanceComponent(Comp);
	Comp->RegisterComponent();
	USceneComponent* Parent = Attach ? Attach : RootComponent.Get();
	Comp->AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);
	if (ShapeMat)
	{
		Comp->SetMaterial(0, ShapeMat);
	}
	Colorize(Comp, Color);
	return Comp;
}

void AHolypawCharacter::UpdateZone()
{
	EHolypawZone Z = EHolypawZone::ForestCottage;
	AHolypawWorldBuilder* WorldB = nullptr;
	for (TActorIterator<AHolypawWorldBuilder> It(GetWorld()); It; ++It)
	{
		WorldB = *It;
		break;
	}
	if (WorldB)
	{
		Z = WorldB->ResolveZone(GetActorLocation());
		CompassLine = WorldB->GetCompassLine(GetActorLocation());
	}
	if (Z != CurrentZone)
	{
		CurrentZone = Z;
		FString Entered = HolypawCatalog::ZoneDisplayName(Z);
		if (Z == EHolypawZone::Ocean)
		{
			Toast(TEXT("Plush Sea. Your paws are soggy. Humans cannot swim thoughts."));
		}
		else if (Z == EHolypawZone::RibbonCity)
		{
			Toast(TEXT("Ribbon City — a buffet of unfinished opinions."));
		}
		else
		{
			Toast(FString::Printf(TEXT("Entered %s"), *Entered));
		}
		if (Story)
		{
			Story->NotifyZone(Z);
		}
		if (HolypawCatalog::IsCityZone(Z))
		{
			UnlockTravel(Z);
			if (WorldB)
			{
				WorldB->RequestDress(Z);
			}
		}
	}
}

TArray<FString> AHolypawCharacter::GetMapLines() const
{
	TArray<FString> Lines;
	for (TActorIterator<AHolypawWorldBuilder> It(GetWorld()); It; ++It)
	{
		Lines = It->GetMapLines(GetActorLocation());
		break;
	}
	Lines.Add(FString::Printf(TEXT("Villain Codex  %d seen  %d fell  / %d  (V)"),
		SeenVillains.Num(), DefeatedVillains.Num(), GetCodexTotal()));
	Lines.Add(GetFaithLine());
	Lines.Add(HolypawFaith::BannerStateLine(GetCityHearts(CurrentZone)));
	for (const FString& Line : GetTravelLines())
	{
		Lines.Add(Line);
	}
	return Lines;
}

AActor* AHolypawCharacter::FindNearestInteractable(float Range) const
{
	AActor* Best = nullptr;
	float BestD = Range;
	for (TActorIterator<AHolypawInteractable> It(GetWorld()); It; ++It)
	{
		AHolypawInteractable* I = *It;
		if (!I)
		{
			continue;
		}
		if (const AHostilePet* H = Cast<AHostilePet>(I))
		{
			if (H->bDefeated)
			{
				continue;
			}
		}
		if (const AWildFluffy* F = Cast<AWildFluffy>(I))
		{
			if (F->bRecruited)
			{
				continue;
			}
		}
		const float D = FVector::Dist(GetActorLocation(), I->GetActorLocation());
		if (D < BestD)
		{
			BestD = D;
			Best = I;
		}
	}
	return Best;
}

void AHolypawCharacter::Interact()
{
	if (Mode == EHolypawPawnMode::Title)
	{
		TitleConfirm();
		return;
	}
	if (Mode == EHolypawPawnMode::Pause)
	{
		TogglePauseMenu();
		return;
	}
	if (Mode == EHolypawPawnMode::UI && bTalkOpen)
	{
		AdvanceTalk();
		return;
	}
	if (Mode == EHolypawPawnMode::UI && bMapOpen && bWantsFastTravel)
	{
		FastTravelToSelected();
		return;
	}
	if (Mode != EHolypawPawnMode::Play)
	{
		if (Mode == EHolypawPawnMode::UI)
		{
			ClosePanels();
		}
		return;
	}
	if (AHolypawInteractable* I = Cast<AHolypawInteractable>(FindNearestInteractable(420.f)))
	{
		I->Interact(this);
	}
	else
	{
		Toast(TEXT("Nobody in hugging range. Lanterns east, humans are famously huggable."));
	}
}

bool AHolypawCharacter::RecruitFluffy(AWildFluffy* Fluffy)
{
	if (!Fluffy || Fluffy->bRecruited)
	{
		return false;
	}
	if (Party->IsFull())
	{
		Toast(TEXT("Party full (4). Four fluffies is already a cult. P to admire them."));
		return false;
	}
	FPartyMember M;
	M.DisplayName = Fluffy->Type.DisplayName;
	M.Color = Fluffy->Type.Color;
	M.Attack = Fluffy->Type.Attack;
	M.Rarity = Fluffy->Type.Rarity;
	M.FluffyId = Fluffy->Type.Id;
	M.Role = UPartyComponent::RoleFor(Fluffy->Type.Id);
	Party->TryAdd(M);
	Fluffy->bRecruited = true;
	Fluffy->SetActorHiddenInGame(true);
	Fluffy->SetActorEnableCollision(false);
	Affection->AddAP(8);
	Affection->AddMiracle(10.f);
	if (Story)
	{
		Story->NotifyRecruit();
	}
	Toast(FString::Printf(TEXT("%s joined as %s. Tiny hench-fluff acquired."),
		*Fluffy->Type.DisplayName.ToString(),
		UPartyComponent::RoleLabel(M.Role)));
	PlayCue(TEXT("Talk"));
	return true;
}

bool AHolypawCharacter::HugPerson(AHugHuman* Human)
{
	if (!Human || Mode != EHolypawPawnMode::Play)
	{
		return false;
	}
	if (HugLock > 0.f)
	{
		return true;
	}
	HugLock = HolypawAnim::HugLockSeconds;
	HolypawAnim::PlayHug(TeddyMotion->Anim, Human->GetActorLocation() - GetActorLocation(), Human->GetActorLocation());
	Human->ReceiveHug(GetActorLocation());
	PlayCue(TEXT("Hug"));

	if (Human->bBeliever)
	{
		Toast(Human->GetBelieverLine());
		return true;
	}

	int32 Gain = 14 + FMath::RandRange(0, 8);
	if (Skills->HasSkill(TEXT("buttonEyes")))
	{
		Gain = FMath::FloorToInt(Gain * 1.2f);
	}
	if (Skills->HasSkill(TEXT("irresistible")))
	{
		Gain = FMath::FloorToInt(Gain * 1.5f);
	}
	Affection->AddAP(Gain);
	float Progress = static_cast<float>(Gain);
	if (Skills->HasSkill(TEXT("deepHug")))
	{
		Progress += 15.f;
	}
	Human->ConvertProgress = FMath::Min(100.f, Human->ConvertProgress + Progress);
	if (Skills->HasSkill(TEXT("stitchHeart")))
	{
		HP = FMath::Min(HPMax, HP + 8);
	}
	const int32 Pct = FMath::FloorToInt(Human->ConvertProgress);
	if (Human->ConvertProgress >= 100.f)
	{
		Human->BecomeBeliever();
		Human->PlayConvertBow();
		CelebrateConvert();
		Affection->AddMiracle(22.f);
		const FString Pulse = AddCityHeart(HolypawFaith::CreditZone(Human, CurrentZone));
		if (Story)
		{
			Story->NotifyConvert();
		}
		FString Line = FString::Printf(TEXT("%s's last serious thought fell out. %s"),
			*Human->PersonName.ToString(), *Human->GetBelieverLine());
		if (!Pulse.IsEmpty())
		{
			Line += TEXT("  ");
			Line += Pulse;
		}
		Toast(Line);
		PlayCue(TEXT("Convert"));
	}
	else
	{
		Toast(FString::Printf(TEXT("%s is %d%% stuffed with Bear Faith. %s  (+%d AP)"),
			*Human->PersonName.ToString(), Pct, *Human->GetSkepticLine(Pct), Gain));
	}
	return true;
}

void AHolypawCharacter::StartBattle(AHostilePet* Enemy)
{
	if (!Enemy || Enemy->bDefeated || Mode != EHolypawPawnMode::Play || Invuln > 0.f)
	{
		return;
	}
	Mode = EHolypawPawnMode::Battle;
	BattleEnemy = Enemy;
	bBattleBusy = false;
	bPlayerTurn = true;
	BattleTurn = 0;
	bPartyCut = false;
	bGuarding = false;
	SlapCombo = 0;
	PoisonTurns = 0;
	FrostTurns = 0;
	HymnShield = 0;
	bEnemyStaggered = false;
	BattlePage = 0;
	EnemyRipTurns = 0;
	MillTurns = 0;
	bSeamBrace = false;
	bPartyBrace = false;
	if (SpringArm)
	{
		SpringArm->TargetArmLength = BattleArm;
	}
	const FVillainDef Def = Enemy->GetDef();
	BattleLog = Def.IntroLine.IsEmpty()
		? Enemy->DisplayName.ToString() + TEXT(" tries to rip the fluff apart!")
		: Def.IntroLine;
	if (Skills->HasSkill(TEXT("fluffShield")))
	{
		BattleLog += TEXT(" Fluff Shield braces you.");
	}
	if (!SeenVillains.Contains(Enemy->VillainId))
	{
		SeenVillains.Add(Enemy->VillainId);
		Toast(FString::Printf(TEXT("Codex: %s logged. Cute, but so rude."), *Enemy->DisplayName.ToString()));
	}
	HolypawAudio::SetCombat(this, true, Enemy->IsBoss(), Enemy->bPhaseTwo);
}

void AHolypawCharacter::PlayerBattleAttack(FName Kind)
{
	if (Mode != EHolypawPawnMode::Battle || bBattleBusy || !bPlayerTurn || !BattleEnemy)
	{
		return;
	}
	bBattleBusy = true;
	AHostilePet* E = BattleEnemy.Get();
	if (Kind != TEXT("slap"))
	{
		SlapCombo = 0;
	}

	if (Kind == TEXT("flee"))
	{
		const float Chance = HolypawBattleDirector::FleeChance(
			Skills->HasSkill(TEXT("haloStep")), E->bBlocksFlee, E->IsBoss())
			+ (Party->CountRole(EPartyRole::Scout) > 0 ? 0.12f : 0.f);
		if (FMath::FRand() < Chance)
		{
			BattleLog = TEXT("You scampered away!");
			PlayCue(TEXT("Flee"));
			GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EndBattle, 0.7f, false);
		}
		else
		{
			BattleLog = E->bBlocksFlee
				? FString::Printf(TEXT("%s blocks the path. No escape."), *E->DisplayName.ToString())
				: TEXT("Blocked! No escape.");
			PlayCue(TEXT("UiDeny"));
			GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.7f, false);
		}
		return;
	}

	if (Kind == TEXT("guard") || Kind == TEXT("stitch") || Kind == TEXT("seamGuard"))
	{
		bGuarding = true;
		if (Kind == TEXT("seamGuard"))
		{
			bSeamBrace = true;
		}
		const int32 Stitch = FMath::Max(4, HolypawBattleDirector::AbilityStitch(Kind));
		HP = FMath::Min(HPMax, HP + Stitch);
		if (Kind == TEXT("seamGuard"))
		{
			BattleLog = FString::Printf(TEXT("Seam Guard holds. Stitch %d stuffing."), Stitch);
		}
		else if (Kind == TEXT("stitch"))
		{
			BattleLog = FString::Printf(TEXT("Deep Stitch tucks %d stuffing."), Stitch);
		}
		else
		{
			BattleLog = Skills->HasSkill(TEXT("seamGuard"))
				? FString::Printf(TEXT("Seam Guard + stitch %d stuffing."), Stitch)
				: FString::Printf(TEXT("You guard your seams and stitch %d."), Stitch);
		}
		HolypawAudio::PlayAbility(this, Kind, false, false);
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.55f, false);
		return;
	}

	if (Kind == TEXT("hymn") || Kind == TEXT("lullaby"))
	{
		const int32 Cost = FMath::Max(Kind == TEXT("lullaby") ? 10 : 8, HolypawBattleDirector::AbilityFpCost(Kind));
		if (!Affection->SpendFP(Cost))
		{
			BattleLog = FString::Printf(TEXT("Need %d FP for that hymn!"), Cost);
			bBattleBusy = false;
			return;
		}
		if (Kind == TEXT("lullaby"))
		{
			BattleLog = TEXT("You sing a dedicated Lullaby.");
			HolypawAudio::PlayAbility(this, Kind, false, false);
			if (FMath::FRand() < 0.62f || HolypawBattleDirector::RollLullaby())
			{
				BattleLog += TEXT(" They snooze a turn.");
				GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::ResumePlayerTurn, 0.75f, false);
				return;
			}
			BattleLog += TEXT(" They fidget through it.");
			GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.7f, false);
			return;
		}
		int32 Heal = Skills->HasSkill(TEXT("hymnWard")) ? 18 : 10;
		HP = FMath::Min(HPMax, HP + Heal);
		PoisonTurns = 0;
		HymnShield = 2;
		BattleLog = FString::Printf(TEXT("Hymn mends %d stuffing, clears poison, and raises a shield."), Heal);
		HolypawAudio::PlayAbility(this, Kind, false, false);
		if (HolypawBattleDirector::RollLullaby())
		{
			BattleLog += TEXT(" Lullaby — they snooze a turn.");
			GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::ResumePlayerTurn, 0.75f, false);
			return;
		}
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.7f, false);
		return;
	}

	if (Kind == TEXT("cheer") || Kind == TEXT("spareBun"))
	{
		int32 Heal = Kind == TEXT("cheer") ? (6 + Party->Members.Num() * 2) : 10;
		HP = FMath::Min(HPMax, HP + Heal);
		BattleLog = Kind == TEXT("cheer")
			? FString::Printf(TEXT("Cheer tucks %d stuffing. %s"), Heal, *Party->DescribeRoles())
			: FString::Printf(TEXT("Spare bun. +%d stuffing."), Heal);
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.55f, false);
		return;
	}

	if (Kind == TEXT("tuck"))
	{
		bGuarding = true;
		bSeamBrace = true;
		HP = FMath::Min(HPMax, HP + 3);
		BattleLog = TEXT("Tuck. Seams fold in.");
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.5f, false);
		return;
	}

	if (Kind == TEXT("ribbonBind"))
	{
		const int32 Cost = FMath::Max(6, HolypawBattleDirector::AbilityFpCost(TEXT("ribbonBind")));
		if (!Affection->SpendFP(Cost))
		{
			BattleLog = FString::Printf(TEXT("Need %d FP to bind."), Cost);
			bBattleBusy = false;
			return;
		}
		bEnemyStaggered = true;
		BattleLog = TEXT("Ribbon Bind. Their special fumbles next.");
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.65f, false);
		return;
	}

	int32 Dmg = 0;
	if (Kind == TEXT("slap"))
	{
		++SlapCombo;
		Dmg = Attack + FMath::RandRange(0, 4) + HolypawBattle::SlapComboBonus(SlapCombo);
		if (Skills->HasSkill(TEXT("bearPaw")))
		{
			Dmg += Attack / 2;
			BattleLog = FString::Printf(TEXT("Bear Paw slaps twice for %d!"), Dmg);
		}
		else
		{
			BattleLog = FString::Printf(TEXT("Soft Slap hits for %d!"), Dmg);
		}
		if (SlapCombo >= 2)
		{
			BattleLog += FString::Printf(TEXT(" Combo x%d."), SlapCombo);
		}
	}
	else if (Kind == TEXT("beam"))
	{
		const int32 Cost = FMath::Max(12, HolypawBattleDirector::AbilityFpCost(TEXT("beam")));
		if (!Affection->SpendFP(Cost))
		{
			BattleLog = FString::Printf(TEXT("Need %d FP!"), Cost);
			bBattleBusy = false;
			return;
		}
		Dmg = FMath::FloorToInt(Attack * 2.2f) + FMath::RandRange(0, 5);
		BattleLog = FString::Printf(TEXT("Cuddle Beam deals %d!"), Dmg);
	}
	else if (Kind == TEXT("party"))
	{
		int32 Base = Party->TotalAttack() + 4;
		if (Skills->HasSkill(TEXT("rallyCry")))
		{
			Base += Party->Members.Num() * 2;
		}
		const float Mult = Skills->HasSkill(TEXT("partyBond")) ? 1.4f : 1.f;
		Dmg = FMath::FloorToInt(Base * Mult) + FMath::RandRange(0, 4);
		Dmg += Party->AssaultBonus(E->GetDef().Faction == EHolypawFaction::PolyMill);
		if (Skills->HasSkill(TEXT("choirAssault")))
		{
			Dmg += Dmg / 2;
		}
		const int32 Choir = Party->CountRole(EPartyRole::Choir);
		if (Choir > 0)
		{
			HP = FMath::Min(HPMax, HP + Choir * 2);
		}
		if (Party->CountRole(EPartyRole::Brace) > 0)
		{
			bPartyBrace = true;
		}
		if (bPartyCut)
		{
			Dmg = FMath::Max(1, Dmg / 2);
			BattleLog = FString::Printf(TEXT("Snipped Party Assault deals %d. %s"), Dmg, *Party->DescribeRoles());
		}
		else
		{
			BattleLog = Party->Members.Num() > 0
				? FString::Printf(TEXT("Party Assault deals %d! %s"), Dmg, *Party->DescribeRoles())
				: FString::Printf(TEXT("Lonely swipe for %d. Find fluffies!"), Dmg);
		}
	}
	else if (Kind == TEXT("unstuff"))
	{
		const int32 Cost = HolypawBattleDirector::AbilityFpCost(TEXT("unstuff"));
		if (Cost > 0 && !Affection->SpendFP(Cost))
		{
			BattleLog = FString::Printf(TEXT("Need %d FP to unstuff."), Cost);
			bBattleBusy = false;
			return;
		}
		Dmg = Attack + 4 + FMath::RandRange(0, 4);
		if (E->GetDef().Faction == EHolypawFaction::PolyMill)
		{
			Dmg += 8;
			BattleLog = FString::Printf(TEXT("Unstuff rips mill polyester for %d!"), Dmg);
		}
		else
		{
			BattleLog = FString::Printf(TEXT("Unstuff tugs a seam for %d."), Dmg);
		}
		EnemyRipTurns = FMath::Max(EnemyRipTurns, 3);
		BattleLog += TEXT(" Rip DoT.");
	}
	else if (Kind == TEXT("buttonBeam"))
	{
		const int32 Cost = FMath::Max(14, HolypawBattleDirector::AbilityFpCost(TEXT("buttonBeam")));
		if (!Affection->SpendFP(Cost))
		{
			BattleLog = FString::Printf(TEXT("Need %d FP!"), Cost);
			bBattleBusy = false;
			return;
		}
		Dmg = FMath::FloorToInt(Attack * 2.4f) + FMath::RandRange(0, 5);
		if (Skills->HasSkill(TEXT("buttonEyes")))
		{
			Dmg += 5;
		}
		BattleLog = FString::Printf(TEXT("Button Beam deals %d!"), Dmg);
	}
	else if (Kind == TEXT("polyRip"))
	{
		const int32 Cost = HolypawBattleDirector::AbilityFpCost(TEXT("polyRip"));
		if (Cost > 0 && !Affection->SpendFP(Cost))
		{
			BattleLog = FString::Printf(TEXT("Need %d FP for Poly Rip."), Cost);
			bBattleBusy = false;
			return;
		}
		Dmg = Attack + 2 + FMath::RandRange(0, 3);
		if (E->GetDef().Faction == EHolypawFaction::PolyMill)
		{
			Dmg += 10;
			BattleLog = FString::Printf(TEXT("Poly Rip unravels mill stuffing for %d!"), Dmg);
		}
		else
		{
			BattleLog = FString::Printf(TEXT("Poly Rip shrugs off handmade seams (%d)."), Dmg);
		}
	}
	else if (Kind == TEXT("fluffBurst"))
	{
		Dmg = Attack + Party->TotalAttack() + FMath::RandRange(0, 4);
		BattleLog = FString::Printf(TEXT("Fluff Burst pops for %d! %s"), Dmg, *Party->DescribeRoles());
	}
	else if (Kind == TEXT("millHymn"))
	{
		const int32 Cost = FMath::Max(8, HolypawBattleDirector::AbilityFpCost(TEXT("millHymn")));
		if (!Affection->SpendFP(Cost))
		{
			BattleLog = FString::Printf(TEXT("Need %d FP for a mill hymn."), Cost);
			bBattleBusy = false;
			return;
		}
		Dmg = FMath::FloorToInt(Attack * 1.6f) + FMath::RandRange(0, 4);
		if (E->GetDef().Faction == EHolypawFaction::PolyMill)
		{
			Dmg += 8;
		}
		HP = FMath::Min(HPMax, HP + 4);
		BattleLog = FString::Printf(TEXT("Mill Hymn deals %d and tucks 4."), Dmg);
	}

	if (E->GetDef().Faction == EHolypawFaction::PolyMill && Skills->HasSkill(TEXT("polyRip")))
	{
		Dmg += 6;
		BattleLog += TEXT(" Poly Rip!");
	}

	const bool bCrit = HolypawBattle::RollCrit();
	if (bCrit)
	{
		Dmg = FMath::Max(1, FMath::FloorToInt(Dmg * 1.5f));
		BattleLog += TEXT(" Crit!");
	}

	const HolypawBattleDirector::FOutgoing Outgoing = HolypawBattleDirector::ApplyOutgoing(
		Dmg, FrostTurns, E->Special, Kind == TEXT("beam") || Kind == TEXT("buttonBeam"), bCrit, BattleLog);
	Dmg = Outgoing.Damage;
	if (Outgoing.bFrostConsumed)
	{
		--FrostTurns;
	}
	if (Outgoing.bStaggered)
	{
		bEnemyStaggered = true;
	}

	E->HP -= Dmg;
	E->PulseHit();
	LastDamageDealt = Dmg;
	DamagePopupTime = 0.9f;
	HolypawAudio::PlayAbility(this, Kind, bCrit, Outgoing.bStaggered);
	if (E->TryEnterPhaseTwo())
	{
		BattleLog += TEXT(" ");
		BattleLog += E->GetPhaseLine();
		HolypawAudio::SetCombat(this, true, E->IsBoss(), true);
	}
	if (E->HP <= 0)
	{
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.35f, false);
		return;
	}
	const float HitStop = HolypawBattleDirector::HitStopSeconds(bCrit, bEnemyStaggered);
	GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, HitStop, false);
}

void AHolypawCharacter::EnemyBattleSwing()
{
	AHostilePet* En = BattleEnemy.Get();
	if (!En)
	{
		EndBattle();
		return;
	}
	if (En->HP <= 0)
	{
		const FVillainDef Def = En->GetDef();
		En->Defeat(true);
		BattleLog = Def.DefeatLine.IsEmpty()
			? TEXT("The hostile pet is unstuffed!")
			: Def.DefeatLine;
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EndBattle, 0.9f, false);
		PlayCue(TEXT("BattleWin"));
		HolypawAudio::NotifyVictory(this);
		return;
	}

	++BattleTurn;
	if (EnemyRipTurns > 0)
	{
		En->HP -= 3;
		--EnemyRipTurns;
		En->PulseHit();
		if (En->HP <= 0)
		{
			const FVillainDef Def = En->GetDef();
			En->Defeat(true);
			BattleLog = TEXT("Rip DoT finished the unstuffing.");
			if (!Def.DefeatLine.IsEmpty())
			{
				BattleLog = Def.DefeatLine;
			}
			GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EndBattle, 0.9f, false);
			PlayCue(TEXT("BattleWin"));
			HolypawAudio::NotifyVictory(this);
			return;
		}
	}
	int32 Dmg = En->Attack + FMath::RandRange(0, 3);
	if (MillTurns > 0)
	{
		Dmg += 2;
		--MillTurns;
	}
	const FString Verb = En->GetDef().AttackLine.IsEmpty() ? TEXT("shreds stuffing") : En->GetDef().AttackLine;

	HolypawBattleDirector::FIncomingRequest Req;
	Req.BaseDamage = Dmg;
	Req.Special = En->Special;
	Req.Rank = En->Rank;
	Req.EnemyAttack = En->Attack;
	Req.EnemyHP = En->HP;
	Req.EnemyHPMax = En->HPMax;
	Req.bPhaseTwo = En->bPhaseTwo;
	Req.BattleTurn = BattleTurn;
	Req.bStaggered = bEnemyStaggered;
	Req.bGuarding = bGuarding || bPartyBrace;
	Req.bSeamGuard = Skills->HasSkill(TEXT("seamGuard")) || bSeamBrace;
	Req.HymnShield = HymnShield;
	Req.bFaithArmor = Skills->HasSkill(TEXT("faithArmor"));
	Req.bFluffShield = Skills->HasSkill(TEXT("fluffShield"));
	Req.PlayerFP = Affection ? Affection->FP : 0;
	Req.MiracleCharge = Affection ? Affection->MiracleCharge : 0.f;
	Req.VillainId = En->VillainId;
	const HolypawBattleDirector::FIncomingResult In = HolypawBattleDirector::ApplyIncoming(Req);
	Dmg = In.Damage;
	if (In.bClearStagger)
	{
		bEnemyStaggered = false;
	}
	if (In.FpDrain > 0 && Affection)
	{
		Affection->SpendFP(In.FpDrain);
	}
	if (In.MiracleStolen > 0.f && Affection)
	{
		Affection->MiracleCharge = FMath::Max(0.f, Affection->MiracleCharge - In.MiracleStolen);
	}
	if (In.FrostTurns > 0)
	{
		FrostTurns = FMath::Max(FrostTurns, In.FrostTurns);
	}
	if (In.PoisonTurns > 0)
	{
		PoisonTurns = FMath::Max(PoisonTurns, In.PoisonTurns);
	}
	if (In.bPartyCut)
	{
		bPartyCut = true;
	}
	if (In.bClearGuard)
	{
		bGuarding = false;
		bSeamBrace = false;
		bPartyBrace = false;
	}
	if (In.MillTurns > 0)
	{
		MillTurns = FMath::Max(MillTurns, In.MillTurns);
	}
	HymnShield = In.HymnShieldLeft;
	const FString Extra = In.Extra;
	HP -= Dmg;
	LastDamageTaken = Dmg;
	HolypawAnim::PlayHurt(TeddyMotion->Anim);
	PlayCue(TEXT("Hurt"));
	BattleLog = En->DisplayName.ToString() + TEXT(" ") + Verb + FString::Printf(TEXT(" for %d!"), Dmg) + Extra;
	if (HP <= 0)
	{
		HP = 0;
		BattleLog = TEXT("Unstuffed… the cottage bed still smells like you. Waking up.");
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::FailAndWakeAtCottage, 1.0f, false);
		return;
	}
	GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::ResumePlayerTurn, 0.65f, false);
}

void AHolypawCharacter::ResumePlayerTurn()
{
	bPlayerTurn = true;
	bBattleBusy = false;
	if (Mode != EHolypawPawnMode::Battle)
	{
		return;
	}
	if (PoisonTurns <= 0)
	{
		return;
	}
	const int32 Tick = 3;
	HP = FMath::Max(0, HP - Tick);
	--PoisonTurns;
	LastDamageTaken = Tick;
	HolypawAnim::PlayHurt(TeddyMotion->Anim);
	BattleLog = FString::Printf(TEXT("Poison thread nips %d stuffing. (%d left)"), Tick, PoisonTurns);
	if (HP <= 0)
	{
		HP = 0;
		bBattleBusy = true;
		BattleLog = TEXT("Poison unstuffed you. The cottage bed still smells like you.");
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::FailAndWakeAtCottage, 1.0f, false);
	}
}

void AHolypawCharacter::FailAndWakeAtCottage()
{
	PlayCue(TEXT("BattleLose"));
	EndBattle();
	HP = HPMax;
	AHolypawWorldBuilder* B = nullptr;
	for (TActorIterator<AHolypawWorldBuilder> It(GetWorld()); It; ++It)
	{
		B = *It;
		break;
	}
	if (B)
	{
		SetActorLocation(B->GetCottageSpawn());
	}
	Toast(TEXT("Back on the porch. The humans will forget you died. They forget everything."));
}

void AHolypawCharacter::EndBattle()
{
	HolypawAudio::SetCombat(this, false, false, false);
	GetWorldTimerManager().ClearTimer(BattleTimer);
	BattleEnemy = nullptr;
	Mode = EHolypawPawnMode::Play;
	bBattleBusy = false;
	bGuarding = false;
	SlapCombo = 0;
	PoisonTurns = 0;
	FrostTurns = 0;
	HymnShield = 0;
	bEnemyStaggered = false;
	Invuln = 1.8f;
	if (SpringArm)
	{
		SpringArm->TargetArmLength = ExploreArm;
	}
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void AHolypawCharacter::GrantKillRewards(AHostilePet* Fallen)
{
	FVillainDef Def;
	if (Fallen)
	{
		Def = Fallen->GetDef();
		if (!SeenVillains.Contains(Fallen->VillainId))
		{
			SeenVillains.Add(Fallen->VillainId);
		}
		if (!DefeatedVillains.Contains(Fallen->VillainId))
		{
			DefeatedVillains.Add(Fallen->VillainId);
		}
	}
	int32 ApGain = Def.ApReward + FMath::RandRange(0, 7);
	int32 FpGain = Def.FpReward + FMath::RandRange(0, 5);
	float Miracle = Def.MiracleReward;
	if (Fallen && Fallen->IsBoss())
	{
		ApGain += 20;
		FpGain += 12;
		Miracle += 10.f;
		HP = FMath::Min(HPMax, HP + 12);
	}
	Affection->AddAP(ApGain);
	Affection->AddFP(FpGain);
	Affection->AddMiracle(Miracle);
	if (Story && Fallen)
	{
		Story->NotifyKill(Fallen->VillainId, Fallen->Rank);
	}
	const FString Name = Fallen ? Fallen->DisplayName.ToString() : TEXT("Pet");
	if (Fallen && Fallen->GetDef().Faction == EHolypawFaction::PolyMill)
	{
		AddItem(TEXT("millScrap"), 1);
		Toast(FString::Printf(TEXT("%s unstuffed! +%d AP · +%d FP · mill scrap"), *Name, ApGain, FpGain));
	}
	else
	{
		Toast(FString::Printf(TEXT("%s unstuffed! +%d AP · +%d FP"), *Name, ApGain, FpGain));
	}
	HolypawAnim::PlayVictory(TeddyMotion->Anim);
	HolypawAnim::PlayCelebrate(TeddyMotion->Party);
}

void AHolypawCharacter::RestFully()
{
	HP = HPMax;
	QuickSave(false);
	Toast(TEXT("Stuffing fluffed. Progress tucked into a slot. The humans will wait."));
}

bool AHolypawCharacter::BuyFaith(int32 ApCost, int32 FpGain)
{
	if (!Affection->SpendAP(ApCost))
	{
		Toast(TEXT("Not enough AP. Hug a human until they stop having opinions."));
		return false;
	}
	Affection->AddFP(FpGain);
	Toast(FString::Printf(TEXT("Traded %d AP for %d FP. Faith is just leftover hugs in a jar."), ApCost, FpGain));
	return true;
}

bool AHolypawCharacter::UseShrine(const EHolypawShrineKind Kind)
{
	switch (Kind)
	{
	case EHolypawShrineKind::Inn:
		HP = HPMax;
		if (Affection)
		{
			Affection->AddFP(8);
		}
		QuickSave(false);
		PlayCue(TEXT("Inn"));
		Toast(TEXT("Spire Inn tucks you in. HP full, cocoa on the nightstand. Saved."));
		return true;
	case EHolypawShrineKind::Chapel:
		if (Affection)
		{
			Affection->MiracleCharge = Affection->MiracleMax;
		}
		PlayCue(TEXT("Chapel"));
		Toast(TEXT("Bear Chapel hymn fills the Miracle bar."));
		return true;
	case EHolypawShrineKind::Workshop:
		if (!Affection || !Affection->SpendAP(8))
		{
			Toast(TEXT("Cloth loft needs 8 AP to stitch a hymn ribbon."));
			return false;
		}
		AddItem(TEXT("hymnRibbon"), 1);
		PlayCue(TEXT("Shop"));
		Toast(TEXT("Seamstress stitches a hymn ribbon (-8 AP)."));
		return true;
	case EHolypawShrineKind::Wish:
		if (Affection)
		{
			Affection->AddFP(4);
		}
		PlayCue(TEXT("Fountain"));
		Toast(TEXT("Fountain wish: +4 FP. The plaza pretends it was always this round."));
		return true;
	case EHolypawShrineKind::Crate:
		if (FMath::RandRange(0, 99) < 55)
		{
			AddItem(TEXT("millScrap"), 1);
			Toast(TEXT("Harbor crate: mill scrap! Proof the factory bleeds."));
		}
		else
		{
			Toast(TEXT("Harbor crate: stuffing dust. Try again later."));
		}
		PlayCue(TEXT("Pickup"));
		return true;
	default:
		return false;
	}
}

void AHolypawCharacter::TryMiracle()
{
	if (Mode != EHolypawPawnMode::Play)
	{
		return;
	}
	if (!Affection->IsMiracleReady())
	{
		Toast(FString::Printf(TEXT("Miracle %d/%d — more hugs. Their brains are still tragically un-stuffed."),
			FMath::FloorToInt(Affection->MiracleCharge), FMath::FloorToInt(Affection->MiracleMax)));
		return;
	}
	int32 FpGain = 40 + Party->Members.Num() * 8;
	if (Skills->HasSkill(TEXT("miracleEcho")))
	{
		FpGain = FMath::FloorToInt(FpGain * 1.5f);
	}
	if (UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		if (GI->IsDusk())
		{
			FpGain += 18;
		}
	}
	Affection->ResetMiracleCharge();
	Affection->AddFP(FpGain);
	HaloMesh->SetHiddenInGame(false);
	PlayCue(TEXT("Miracle"));
	int32 Heal = Skills->HasSkill(TEXT("peakLiturgy")) ? 30 : 15;
	HP = FMath::Min(HPMax, HP + Heal);
	int32 NewlyConvinced = 0;
	FString LastPulse;
	const float Sermon = Skills->HasSkill(TEXT("bearCreed")) ? 40.f : 14.f;
	const float SermonRange = Skills->HasSkill(TEXT("bearCreed")) ? 2800.f : 1600.f;
	for (TActorIterator<AHugHuman> It(GetWorld()); It; ++It)
	{
		AHugHuman* H = *It;
		if (!H || H->bBeliever)
		{
			continue;
		}
		if (FVector::Dist(GetActorLocation(), H->GetActorLocation()) > SermonRange)
		{
			continue;
		}
		const bool bWasOpen = H->ConvertProgress < 100.f;
		H->ConvertProgress = FMath::Min(100.f, H->ConvertProgress + Sermon);
		H->ReceiveHug(GetActorLocation());
		if (bWasOpen && H->ConvertProgress >= 100.f)
		{
			H->BecomeBeliever();
			H->PlayConvertBow();
			++NewlyConvinced;
			LastPulse = AddCityHeart(HolypawFaith::CreditZone(H, CurrentZone));
			if (Story)
			{
				Story->NotifyConvert();
			}
		}
	}
	for (TActorIterator<AHostilePet> It(GetWorld()); It; ++It)
	{
		AHostilePet* H = *It;
		if (!H || H->bDefeated)
		{
			continue;
		}
		if (FVector::Dist(GetActorLocation(), H->GetActorLocation()) < 1800.f)
		{
			H->HP -= 15;
			if (H->HP <= 0)
			{
				H->Defeat(false);
			}
		}
	}
	if (NewlyConvinced > 0)
	{
		HolypawAnim::PlayConvert(TeddyMotion->Anim);
	}
	else
	{
		HolypawAnim::PlayVictory(TeddyMotion->Anim);
	}
	HolypawAnim::PlayCelebrate(TeddyMotion->Party);
	FString MiracleToast = FString::Printf(TEXT("Miracle hymn! +%d FP. %d human(s) dropped their last independent thought."),
		FpGain, NewlyConvinced);
	if (!LastPulse.IsEmpty())
	{
		MiracleToast += TEXT("  ");
		MiracleToast += LastPulse;
	}
	if (UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		if (GI->IsDusk())
		{
			MiracleToast += TEXT("  Dusk bonus — the sky clapped.");
		}
	}
	Toast(MiracleToast);
	if (Story)
	{
		Story->NotifyMiracle(CurrentZone);
	}
}

void AHolypawCharacter::ClosePanels()
{
	bSkillsOpen = false;
	bPartyOpen = false;
	bMapOpen = false;
	bCodexOpen = false;
	bJournalOpen = false;
	bTalkOpen = false;
	bShopOpen = false;
	bInventoryOpen = false;
	if (Mode == EHolypawPawnMode::UI)
	{
		Mode = EHolypawPawnMode::Play;
	}
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void AHolypawCharacter::SetPanel(bool& Flag)
{
	if (Mode == EHolypawPawnMode::Battle || Mode == EHolypawPawnMode::Title || Mode == EHolypawPawnMode::Pause)
	{
		return;
	}
	const bool Open = !Flag;
	ClosePanels();
	if (Open)
	{
		Flag = true;
		Mode = EHolypawPawnMode::UI;
		PlayCue(TEXT("UiOpen"));
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeGameAndUI());
		}
	}
}

void AHolypawCharacter::ToggleSkills()
{
	if (Mode == EHolypawPawnMode::Title || Mode == EHolypawPawnMode::Pause)
	{
		return;
	}
	SetPanel(bSkillsOpen);
}

void AHolypawCharacter::ToggleParty()
{
	if (Mode == EHolypawPawnMode::Title || Mode == EHolypawPawnMode::Pause)
	{
		return;
	}
	SetPanel(bPartyOpen);
}

void AHolypawCharacter::ToggleMap()
{
	if (Mode == EHolypawPawnMode::Title)
	{
		TitleNewGame();
		return;
	}
	if (Mode == EHolypawPawnMode::Pause)
	{
		return;
	}
	bWantsFastTravel = false;
	SetPanel(bMapOpen);
	if (bMapOpen)
	{
		bWantsFastTravel = true;
	}
}

void AHolypawCharacter::ToggleCodex()
{
	if (Mode == EHolypawPawnMode::Title || Mode == EHolypawPawnMode::Pause)
	{
		return;
	}
	SetPanel(bCodexOpen);
}

void AHolypawCharacter::ToggleJournal()
{
	if (Mode == EHolypawPawnMode::Title || Mode == EHolypawPawnMode::Pause)
	{
		return;
	}
	SetPanel(bJournalOpen);
}

void AHolypawCharacter::CycleSkillTree()
{
	if (Mode == EHolypawPawnMode::Title)
	{
		return;
	}
	if (Mode == EHolypawPawnMode::Battle)
	{
		BattlePage = (BattlePage + 1) % 3;
		if (BattlePage == 0)
		{
			Toast(TEXT("Commands: Slap Beam Party Flee Guard Hymn  (Tab: overflow)"));
		}
		else if (BattlePage == 1)
		{
			Toast(TEXT("Overflow: Unstuff ButtonBeam Stitch PolyRip Lullaby SeamGuard"));
		}
		else
		{
			Toast(TEXT("Party tricks: Cheer Tuck FluffBurst MillHymn RibbonBind SpareBun"));
		}
		return;
	}
	if (bMapOpen)
	{
		CycleTravel(1);
		return;
	}
	if (!bSkillsOpen || !Skills)
	{
		return;
	}
	Skills->CycleTree();
	Toast(HolypawCatalog::SkillTreeName(Skills->ActiveTree));
}

TArray<FString> AHolypawCharacter::GetJournalLines() const
{
	TArray<FString> Lines;
	if (Story)
	{
		Lines = Story->GetJournalLines();
	}
	Lines.Add(TEXT(""));
	Lines.Add(TEXT("--- errands ---"));
	int32 Shown = 0;
	for (const FHolypawQuestDef& Q : HolypawCatalog::GetQuests())
	{
		if (QuestDone.Contains(Q.Id))
		{
			Lines.Add(FString::Printf(TEXT("x  %s"), *Q.Title.ToString()));
			++Shown;
		}
		else if (QuestActive.Contains(Q.Id))
		{
			Lines.Add(FString::Printf(TEXT(">  %s  —  %s"), *Q.Title.ToString(), *Q.Brief.ToString()));
			++Shown;
		}
	}
	if (Shown == 0)
	{
		Lines.Add(TEXT("Talk 4 takes a job. Talk 3 turns it in. All 26 cities have an errand if you hop."));
	}
	return Lines;
}

void AHolypawCharacter::CompleteBearFaith()
{
	for (TActorIterator<AHugHuman> It(GetWorld()); It; ++It)
	{
		if (AHugHuman* H = *It)
		{
			H->ConvertProgress = 100.f;
			H->BecomeBeliever(false);
			H->KneelInWorship();
		}
	}
	HaloMesh->SetHiddenInGame(false);
	HolypawAnim::PlayVictory(TeddyMotion->Anim);
	HolypawAnim::PlayCelebrate(TeddyMotion->Party);
	Toast(TEXT("Every human kneels. They were so easy. The Poly Mill can keep the polyester — you kept the people."));
}

void AHolypawCharacter::CloseOrJump()
{
	if (Mode == EHolypawPawnMode::Title)
	{
		return;
	}
	if (Mode == EHolypawPawnMode::Pause)
	{
		TogglePauseMenu();
		return;
	}
	if (Mode == EHolypawPawnMode::UI)
	{
		ClosePanels();
		return;
	}
	if (Mode == EHolypawPawnMode::Play)
	{
		TogglePauseMenu();
	}
}

void AHolypawCharacter::Jump()
{
	if (Mode != EHolypawPawnMode::Play)
	{
		return;
	}
	HolypawAnim::PlayJump(TeddyMotion->Anim);
	HolypawAudio::NotifyJumped(this);
	Super::Jump();
}

int32 AHolypawCharacter::GetCodexTotal() const
{
	return HolypawCatalog::GetVillains().Num();
}

TArray<FString> AHolypawCharacter::GetCodexLines() const
{
	TArray<FString> Lines;
	for (const FVillainDef& D : HolypawCatalog::GetVillains())
	{
		const bool Seen = SeenVillains.Contains(D.Id);
		const bool Beat = DefeatedVillains.Contains(D.Id);
		if (!Seen)
		{
			Lines.Add(FString::Printf(TEXT("????   %s   ???"), HolypawCatalog::ZoneDisplayName(D.HomeZone)));
			continue;
		}
		const FString Mark = Beat ? TEXT("FELL") : TEXT("SEEN");
		Lines.Add(FString::Printf(TEXT("%s  %s  %s  %dHP/%dATK  %s"),
			*D.DisplayName.ToString(),
			HolypawCatalog::ZoneDisplayName(D.HomeZone),
			*HolypawCatalog::RankLabel(D.Rank),
			D.HP,
			D.Attack,
			*Mark));
	}
	return Lines;
}

void AHolypawCharacter::TryBuySkill(FName Id)
{
	int32 Ap = Affection->AP;
	if (!Skills->Buy(Id, Ap))
	{
		Toast(TEXT("Cannot unlock that skill yet."));
		return;
	}
	Affection->AP = Ap;
	Affection->AddAP(0, false);
	ApplySkillEffects(Id);
	Toast(TEXT("New fluff power. You are getting dangerously huggable."));
}

void AHolypawCharacter::ApplySkillEffects(FName Id)
{
	if (Id == TEXT("softFur"))
	{
		Attack += 3;
	}
	else if (Id == TEXT("haloStep"))
	{
		WalkSpeed += 120.f;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		HaloMesh->SetHiddenInGame(false);
	}
	else if (Id == TEXT("fluffShield"))
	{
		HPMax += 20;
		HP += 20;
	}
}

void AHolypawCharacter::TryBuyTreeSlot(int32 Index)
{
	if (Mode == EHolypawPawnMode::UI && (bTalkOpen || bShopOpen || bInventoryOpen))
	{
		if (bTalkOpen)
		{
			const HolypawDialogue::ETalkVerb Verb = HolypawDialogue::VerbFromSlot(Index);
			if (Verb != HolypawDialogue::ETalkVerb::None)
			{
				ApplyTalkVerb(static_cast<uint8>(Verb));
			}
		}
		else if (bShopOpen)
		{
			BuyShopSlot(Index);
		}
		else if (bInventoryOpen)
		{
			if (Inventory.IsValidIndex(Index))
			{
				UseItem(Inventory[Index].Id);
			}
		}
		return;
	}
	if (!Skills)
	{
		return;
	}
	const TArray<FSkillDef> Tree = Skills->GetTreeSkills(Skills->ActiveTree);
	if (Tree.IsValidIndex(Index))
	{
		TryBuySkill(Tree[Index].Id);
	}
}

void AHolypawCharacter::BattleSlap()
{
	if (Mode == EHolypawPawnMode::Title)
	{
		TitleSelectSlot(0);
		return;
	}
	if (bSkillsOpen || bTalkOpen || bShopOpen || bInventoryOpen)
	{
		TryBuyTreeSlot(0);
		return;
	}
	PlayerBattleAttack(BattleCommandId(1));
}

void AHolypawCharacter::BattleBeam()
{
	if (Mode == EHolypawPawnMode::Title)
	{
		TitleSelectSlot(1);
		return;
	}
	if (bSkillsOpen || bTalkOpen || bShopOpen || bInventoryOpen)
	{
		TryBuyTreeSlot(1);
		return;
	}
	PlayerBattleAttack(BattleCommandId(2));
}

void AHolypawCharacter::BattlePartyAtk()
{
	if (Mode == EHolypawPawnMode::Title)
	{
		TitleSelectSlot(2);
		return;
	}
	if (bSkillsOpen || bTalkOpen || bShopOpen || bInventoryOpen)
	{
		TryBuyTreeSlot(2);
		return;
	}
	PlayerBattleAttack(BattleCommandId(3));
}

void AHolypawCharacter::BattleFlee()
{
	if (bSkillsOpen || bTalkOpen || bShopOpen || bInventoryOpen)
	{
		TryBuyTreeSlot(3);
		return;
	}
	PlayerBattleAttack(BattleCommandId(4));
}

void AHolypawCharacter::Skill5()
{
	if (bSkillsOpen || bTalkOpen || bShopOpen || bInventoryOpen)
	{
		TryBuyTreeSlot(4);
		return;
	}
	PlayerBattleAttack(BattleCommandId(5));
}

void AHolypawCharacter::Skill6()
{
	if (bSkillsOpen || bTalkOpen || bShopOpen || bInventoryOpen)
	{
		TryBuyTreeSlot(5);
		return;
	}
	PlayerBattleAttack(BattleCommandId(6));
}

void AHolypawCharacter::Toast(const FString& Msg)
{
	ToastMsg = Msg;
	ToastTime = 3.2f;
	UE_LOG(LogTemp, Log, TEXT("[Holypaw] %s"), *Msg);
}

void AHolypawCharacter::ResetForNewGame()
{
	HP = 50;
	HPMax = 50;
	Attack = 8;
	WalkSpeed = 700.f;
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = WalkSpeed;
	}
	if (HaloMesh)
	{
		HaloMesh->SetHiddenInGame(true);
	}
	if (Affection)
	{
		Affection->AP = 0;
		Affection->FP = 0;
		Affection->MiracleCharge = 0.f;
		Affection->Level = 1;
	}
	if (Party)
	{
		Party->Members.Reset();
	}
	if (Skills)
	{
		Skills->ReplaceOwned({});
		Skills->ActiveTree = EHolypawSkillTree::Hug;
	}
	if (Story)
	{
		Story->CurrentIndex = 0;
		Story->Recruits = 0;
		Story->Kills = 0;
		Story->Converts = 0;
		Story->Miracles = 0;
		Story->bCampaignComplete = false;
		Story->ZonesVisited.Reset();
		Story->BossesFell.Reset();
	}
	SeenVillains.Reset();
	DefeatedVillains.Reset();
	CityHearts.Reset();
	UnlockedTravel.Reset();
	UnlockTravel(EHolypawZone::ForestCottage);
	PulseLivingWorld(true);
	Inventory.Reset();
	AddItem(TEXT("stuffingBun"), 1);
	QuestActive.Reset();
	QuestDone.Reset();
	for (TActorIterator<AHolypawWorldBuilder> It(GetWorld()); It; ++It)
	{
		SetActorLocation(It->GetCottageSpawn() + FVector(420.f, 0.f, 40.f));
		break;
	}
	CurrentZone = EHolypawZone::ForestCottage;
}

void AHolypawCharacter::SetCodex(const TArray<EHolypawVillain>& Seen, const TArray<EHolypawVillain>& Defeated)
{
	SeenVillains = Seen;
	DefeatedVillains = Defeated;
}

void AHolypawCharacter::SetHeartRecords(const TArray<FHolypawHeartRecord>& Records)
{
	CityHearts = Records;
	PulseLivingWorld(true);
}

void AHolypawCharacter::PulseLivingWorld(const bool bSnap)
{
	if (!GetWorld())
	{
		return;
	}
	for (TActorIterator<AHolypawWorldBuilder> It(GetWorld()); It; ++It)
	{
		if (bSnap)
		{
			It->SnapConversionLook();
		}
		It->TickConversionPulse(0.f);
		break;
	}
}

FString AHolypawCharacter::GetFaithLine() const
{
	return HolypawFaith::HudLine(CurrentZone, GetCityHearts(CurrentZone));
}

void AHolypawCharacter::SetUnlockedTravel(const TArray<EHolypawZone>& Zones)
{
	UnlockedTravel = Zones;
	if (!UnlockedTravel.Contains(EHolypawZone::ForestCottage))
	{
		UnlockedTravel.Insert(EHolypawZone::ForestCottage, 0);
	}
}

void AHolypawCharacter::UnlockTravel(EHolypawZone Zone)
{
	UnlockedTravel.AddUnique(Zone);
}

int32 AHolypawCharacter::GetCityHearts(EHolypawZone Zone) const
{
	for (const FHolypawHeartRecord& Rec : CityHearts)
	{
		if (Rec.Zone == Zone)
		{
			return Rec.Hearts;
		}
	}
	return 0;
}

FString AHolypawCharacter::AddCityHeart(EHolypawZone Zone, int32 Amount)
{
	if (!HolypawCatalog::IsCityZone(Zone) && Zone != EHolypawZone::ForestCottage)
	{
		Zone = CurrentZone;
	}
	const EHolypawFaithStage Before = HolypawFaith::StageForHearts(GetCityHearts(Zone));
	int32 Now = 0;
	bool bFound = false;
	for (FHolypawHeartRecord& Rec : CityHearts)
	{
		if (Rec.Zone == Zone)
		{
			Rec.Hearts += Amount;
			Now = Rec.Hearts;
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		FHolypawHeartRecord Rec;
		Rec.Zone = Zone;
		Rec.Hearts = Amount;
		Now = Rec.Hearts;
		CityHearts.Add(Rec);
	}
	UnlockTravel(Zone);
	PulseLivingWorld();
	for (TActorIterator<AHolypawWorldBuilder> It(GetWorld()); It; ++It)
	{
		It->NotifyConvertPulse(GetActorLocation());
		break;
	}
	const EHolypawFaithStage After = HolypawFaith::StageForHearts(Now);
	if (After == Before)
	{
		return FString();
	}
	if (HolypawFaith::ShopsOpen(Now) && !HolypawFaith::ShopsOpen(HolypawFaith::HeartsForStage(Before)))
	{
		PlayCue(TEXT("ShopOpen"));
	}
	else if (HolypawFaith::MillBannersDown(Now))
	{
		PlayCue(TEXT("BannerDown"));
	}
	return HolypawFaith::StageToast(After, Zone);
}

void AHolypawCharacter::OpenFastTravel(EHolypawZone FromZone)
{
	UnlockTravel(FromZone);
	if (!bMapOpen)
	{
		SetPanel(bMapOpen);
	}
	bWantsFastTravel = true;
	for (int32 I = 0; I < UnlockedTravel.Num(); ++I)
	{
		if (UnlockedTravel[I] == FromZone)
		{
			TravelCursor = I;
			break;
		}
	}
	Toast(TEXT("Lantern map. Tab picks a city. E hops. Hearts make the hop feel earned."));
}

void AHolypawCharacter::CycleTravel(int32 Delta)
{
	if (UnlockedTravel.Num() == 0)
	{
		UnlockTravel(EHolypawZone::ForestCottage);
	}
	TravelCursor = (TravelCursor + Delta + UnlockedTravel.Num()) % UnlockedTravel.Num();
	Toast(FString::Printf(TEXT("Lantern → %s"), HolypawCatalog::ZoneDisplayName(GetSelectedTravel())));
}

EHolypawZone AHolypawCharacter::GetSelectedTravel() const
{
	if (UnlockedTravel.IsValidIndex(TravelCursor))
	{
		return UnlockedTravel[TravelCursor];
	}
	return EHolypawZone::ForestCottage;
}

TArray<FString> AHolypawCharacter::GetTravelLines() const
{
	TArray<FString> Lines;
	Lines.Add(TEXT("Lanterns (visited + cottage):"));
	for (int32 I = 0; I < UnlockedTravel.Num(); ++I)
	{
		const EHolypawZone Z = UnlockedTravel[I];
		const TCHAR* Mark = (I == TravelCursor) ? TEXT(">") : TEXT(" ");
		Lines.Add(FString::Printf(TEXT("%s  %s   Hearts %d"), Mark, HolypawCatalog::ZoneDisplayName(Z), GetCityHearts(Z)));
	}
	return Lines;
}

void AHolypawCharacter::FastTravelToSelected()
{
	const EHolypawZone Dest = GetSelectedTravel();
	FVector Loc = FVector::ZeroVector;
	AHolypawWorldBuilder* WorldB = nullptr;
	for (TActorIterator<AHolypawWorldBuilder> It(GetWorld()); It; ++It)
	{
		WorldB = *It;
		Loc = WorldB->GetTravelLocation(Dest);
		break;
	}
	if (Loc.IsNearlyZero())
	{
		Toast(TEXT("That lantern is unlit."));
		return;
	}
	ClosePanels();
	bWantsFastTravel = false;
	SetActorLocation(Loc);
	CurrentZone = Dest;
	if (WorldB)
	{
		WorldB->RequestDress(Dest);
	}
	if (Story)
	{
		Story->NotifyZone(Dest);
	}
	QuickSave();
	PlayCue(TEXT("Travel"));
	Toast(FString::Printf(TEXT("Lantern hop: %s. The coup commutes."), HolypawCatalog::ZoneDisplayName(Dest)));
}

void AHolypawCharacter::TitleSelectSlot(int32 Index)
{
	if (UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		GI->TitleCursor = FMath::Clamp(Index, 0, UHolypawGameInstance::SlotCount - 1);
		PlayCue(TEXT("UiMove"));
		Toast(FString::Printf(TEXT("Slot %d  %s"), GI->TitleCursor + 1, *GI->SlotSummary(GI->TitleCursor)));
	}
}

void AHolypawCharacter::TitleConfirm()
{
	UHolypawGameInstance* GI = UHolypawGameInstance::Get(this);
	if (!GI)
	{
		Mode = EHolypawPawnMode::Play;
		return;
	}
	if (GI->SlotOccupied(GI->TitleCursor))
	{
		PlayCue(TEXT("UiConfirm"));
		TitleLoad();
	}
	else
	{
		PlayCue(TEXT("UiConfirm"));
		TitleNewGame();
	}
}

void AHolypawCharacter::TitleNewGame()
{
	UHolypawGameInstance* GI = UHolypawGameInstance::Get(this);
	if (GI)
	{
		GI->StartNewGame(this, GI->TitleCursor);
	}
	else
	{
		ResetForNewGame();
	}
	Mode = EHolypawPawnMode::Play;
	HolypawLook::ApplyViewExposure(Camera);
	if (Story)
	{
		Story->TryAdvance(this);
		const FMissionDef Cur = Story->GetCurrent();
		Toast(FString::Printf(TEXT("Paws on the porch, world on the menu. %s — J for the plan."), *Cur.Title.ToString()));
	}
}

void AHolypawCharacter::TitleLoad()
{
	UHolypawGameInstance* GI = UHolypawGameInstance::Get(this);
	if (!GI || !GI->SlotOccupied(GI->TitleCursor))
	{
		Toast(TEXT("That slot is an empty porch. Press N for a new coup."));
		return;
	}
	if (!GI->LoadSlotOntoPawn(this, GI->TitleCursor))
	{
		Toast(TEXT("Load failed. The stuffing came out of the file."));
		return;
	}
	Mode = EHolypawPawnMode::Play;
	HolypawLook::ApplyViewExposure(Camera);
	Toast(FString::Printf(TEXT("Loaded. %s still believes a teddy should run things."),
		HolypawCatalog::ZoneDisplayName(CurrentZone)));
}

void AHolypawCharacter::QuickSave(bool bAnnounce)
{
	UHolypawGameInstance* GI = UHolypawGameInstance::Get(this);
	if (!GI || Mode == EHolypawPawnMode::Title || Mode == EHolypawPawnMode::Battle)
	{
		return;
	}
	if (!GI->bSessionStarted)
	{
		GI->bSessionStarted = true;
	}
	if (GI->SavePawnToSlot(this, GI->ActiveSlot))
	{
		PlayCue(TEXT("Save"));
		if (bAnnounce)
		{
			Toast(FString::Printf(TEXT("Saved slot %d. The mill cannot un-save a hug."), GI->ActiveSlot + 1));
		}
	}
}

void AHolypawCharacter::TogglePauseMenu()
{
	if (Mode == EHolypawPawnMode::Battle || Mode == EHolypawPawnMode::Title)
	{
		return;
	}
	if (Mode == EHolypawPawnMode::Pause)
	{
		Mode = EHolypawPawnMode::Play;
		return;
	}
	ClosePanels();
	Mode = EHolypawPawnMode::Pause;
}

void AHolypawCharacter::ReturnToTitle()
{
	if (Mode != EHolypawPawnMode::Pause && Mode != EHolypawPawnMode::Play)
	{
		return;
	}
	QuickSave();
	Mode = EHolypawPawnMode::Title;
	Toast(TEXT("Title. Slots wait. The porch does not move."));
}

void AHolypawCharacter::CycleMute()
{
	if (UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		GI->CycleMute();
		HolypawAudio::ApplyMute(this);
		Toast(GI->Settings && GI->Settings->bMuted ? TEXT("Mute ON") : TEXT("Mute OFF"));
	}
}

void AHolypawCharacter::TitleConfirmPressed()
{
	if (Mode == EHolypawPawnMode::Title)
	{
		TitleConfirm();
	}
	else if (Mode == EHolypawPawnMode::Pause)
	{
		TogglePauseMenu();
	}
}

void AHolypawCharacter::TitleLoadPressed()
{
	if (Mode == EHolypawPawnMode::Title)
	{
		TitleLoad();
	}
}

void AHolypawCharacter::QuickSavePressed()
{
	QuickSave();
}

void AHolypawCharacter::MutePressed()
{
	CycleMute();
}

void AHolypawCharacter::TitleMenuPressed()
{
	if (Mode == EHolypawPawnMode::Pause)
	{
		ReturnToTitle();
	}
	else if (Mode == EHolypawPawnMode::Play)
	{
		TogglePauseMenu();
		ReturnToTitle();
	}
}

void AHolypawCharacter::InventoryPressed()
{
	ToggleInventory();
}

void AHolypawCharacter::PlayCue(FName Cue)
{
	HolypawAudio::PlayCue(this, Cue);
}

void AHolypawCharacter::CelebrateConvert()
{
	HolypawAnim::PlayConvert(TeddyMotion->Anim);
	HolypawAnim::PlayCelebrate(TeddyMotion->Party);
}

void AHolypawCharacter::TickProcAnim(float DeltaSeconds)
{
	if (Mode == EHolypawPawnMode::Title || Mode == EHolypawPawnMode::Pause)
	{
		return;
	}
	HolypawAnim::FTeddyInput In;
	In.Speed = GetVelocity().Size();
	In.bAirborne = GetCharacterMovement() && GetCharacterMovement()->IsFalling();
	In.bInBattle = Mode == EHolypawPawnMode::Battle;
	In.DeltaSeconds = DeltaSeconds;
	HolypawAnim::TickTeddy(TeddyMotion->Anim, In);
	if (HolypawAnim::IsHugging(TeddyMotion->Anim) && !TeddyMotion->Anim.HugDir.IsNearlyZero())
	{
		const FRotator Want(0.f, TeddyMotion->Anim.HugDir.Rotation().Yaw, 0.f);
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), Want, DeltaSeconds, 14.f));
	}
	if (HolypawAnim::IsHugging(TeddyMotion->Anim) && TeddyMotion->Anim.bHasHugTarget)
	{
		FVector Here = GetActorLocation();
		FVector There = TeddyMotion->Anim.HugTarget;
		There.Z = Here.Z;
		const float Dist = FVector::Dist2D(Here, There);
		if (Dist > HolypawAnim::HugStandOff)
		{
			const FVector Step = (There - Here).GetSafeNormal2D() * FMath::Min(Dist - HolypawAnim::HugStandOff, 520.f * DeltaSeconds);
			AddActorWorldOffset(FVector(Step.X, Step.Y, 0.f));
		}
	}
	HolypawAnim::ApplyTeddyPose(HolypawAnim::EvaluateTeddy(TeddyMotion->Anim, TeddyMotion->Rest), MakeTeddyParts(*this));

	const float Walk = TeddyMotion->Anim.Walk;
	const float HugAmt = HolypawAnim::WrapAmount(TeddyMotion->Anim);
	const float Stride = TeddyMotion->Anim.Clock * (7.4f + Walk * 5.6f);
	auto DriveLimb = [](UStaticMeshComponent* Limb, UStaticMeshComponent* End,
		const FVector& LimbBase, const FRotator& LimbRot,
		const FVector& EndBase, const FRotator& EndRot,
		const float Phase, const float WalkAmt, const float Hug, const float HugYaw)
	{
		const float Swing = FMath::Sin(Phase);
		const float Lift = FMath::Max(0.f, -FMath::Cos(Phase));
		const float Plant = FMath::Max(0.f, FMath::Cos(Phase));
		if (Limb)
		{
			Limb->SetRelativeRotation(LimbRot + FRotator(Swing * WalkAmt * 26.f + Hug * 42.f, 0.f, HugYaw));
			Limb->SetRelativeLocation(LimbBase + FVector(Hug * 8.f + Swing * WalkAmt * 3.f, 0.f, Lift * WalkAmt * 6.f - Plant * WalkAmt * 1.8f));
		}
		if (End)
		{
			End->SetRelativeRotation(EndRot + FRotator(Swing * WalkAmt * -16.f + Hug * 28.f, 0.f, HugYaw * 0.8f));
			End->SetRelativeLocation(EndBase + FVector(0.f, 0.f, Lift * WalkAmt * 2.f));
		}
	};
	DriveLimb(ArmL, nullptr, ArmLBase, ArmLRot, FVector::ZeroVector, FRotator::ZeroRotator, Stride + PI, Walk, HugAmt, HugAmt * 22.f);
	DriveLimb(ArmR, nullptr, ArmRBase, ArmRRot, FVector::ZeroVector, FRotator::ZeroRotator, Stride, Walk, HugAmt, HugAmt * -22.f);
	DriveLimb(LegL, FootL, LegLBase, LegLRot, FootLBase, FootLRot, Stride, Walk, 0.f, 0.f);
	DriveLimb(LegR, FootR, LegRBase, LegRRot, FootRBase, FootRRot, Stride + PI, Walk, 0.f, 0.f);

	const float EyeY = EyeL ? EyeL->GetRelativeScale3D().Z / FMath::Max(TeddyMotion->Rest.EyeLScale.Z, 0.01f) : 1.f;
	if (HighlightL)
	{
		HighlightL->SetRelativeScale3D(HighlightLScale * FMath::Clamp(EyeY, 0.f, 1.f));
		HighlightL->SetHiddenInGame(EyeY < 0.4f);
	}
	if (HighlightR)
	{
		HighlightR->SetRelativeScale3D(HighlightRScale * FMath::Clamp(EyeY, 0.f, 1.f));
		HighlightR->SetHiddenInGame(EyeY < 0.4f);
	}
	if (Camera && EyeY >= 0.4f)
	{
		const FVector CamLoc = Camera->GetComponentLocation();
		HolypawLook::AimCatchlight(HighlightL, EyeL, CamLoc);
		HolypawLook::AimCatchlight(HighlightR, EyeR, CamLoc);
	}
	if (Tail)
	{
		Tail->SetRelativeRotation(FRotator(FMath::Sin(TeddyMotion->Anim.Clock * 4.8f) * (8.f + Walk * 10.f), 0.f, 0.f));
	}
}

FString AHolypawCharacter::GetClockLine() const
{
	if (const UHolypawGameInstance* GI = UHolypawGameInstance::Get(this))
	{
		return GI->GetClockLabel();
	}
	return TEXT("");
}

FName AHolypawCharacter::BattleCommandId(const int32 Slot) const
{
	if (const FHolypawAbilityDef* A = HolypawCatalog::FindAbilityBySlot(BattlePage, Slot))
	{
		return A->Id;
	}
	if (const FHolypawAbilityDef* Fallback = HolypawCatalog::FindAbilityBySlot(0, Slot))
	{
		return Fallback->Id;
	}
	return NAME_None;
}

FString AHolypawCharacter::GetBattleStatusLine() const
{
	FString Line = HolypawBattle::FormatStatus(SlapCombo, PoisonTurns, FrostTurns, HymnShield, bEnemyStaggered);
	if (EnemyRipTurns > 0)
	{
		Line += Line.IsEmpty() ? TEXT("") : TEXT("  ·  ");
		Line += FString::Printf(TEXT("Rip %d"), EnemyRipTurns);
	}
	if (MillTurns > 0)
	{
		Line += Line.IsEmpty() ? TEXT("") : TEXT("  ·  ");
		Line += FString::Printf(TEXT("Polyester %d"), MillTurns);
	}
	if (BattlePage == 1)
	{
		Line += Line.IsEmpty() ? TEXT("") : TEXT("  ·  ");
		Line += TEXT("Overflow");
	}
	return Line;
}

int32 AHolypawCharacter::ShopPrice(int32 BaseCost) const
{
	if (GetCityHearts(CurrentZone) >= 1)
	{
		return FMath::Max(1, FMath::FloorToInt(BaseCost * 0.8f));
	}
	return BaseCost;
}

void AHolypawCharacter::SetInventory(const TArray<FHolypawItemStack>& Stacks)
{
	Inventory = Stacks;
}

int32 AHolypawCharacter::GetItemCount(FName Id) const
{
	for (const FHolypawItemStack& S : Inventory)
	{
		if (S.Id == Id)
		{
			return S.Count;
		}
	}
	return 0;
}

void AHolypawCharacter::AddItem(FName Id, int32 Amount)
{
	if (Amount <= 0 || Id.IsNone())
	{
		return;
	}
	for (FHolypawItemStack& S : Inventory)
	{
		if (S.Id == Id)
		{
			S.Count += Amount;
			return;
		}
	}
	FHolypawItemStack S;
	S.Id = Id;
	S.Count = Amount;
	Inventory.Add(S);
}

bool AHolypawCharacter::ConsumeItem(FName Id, int32 Amount)
{
	if (Amount <= 0 || Id.IsNone())
	{
		return false;
	}
	for (int32 I = 0; I < Inventory.Num(); ++I)
	{
		if (Inventory[I].Id != Id || Inventory[I].Count < Amount)
		{
			continue;
		}
		Inventory[I].Count -= Amount;
		if (Inventory[I].Count <= 0)
		{
			Inventory.RemoveAt(I);
		}
		return true;
	}
	return false;
}

bool AHolypawCharacter::UseItem(FName Id)
{
	const FHolypawItemDef* Def = HolypawCatalog::FindItem(Id);
	if (!Def)
	{
		Toast(TEXT("That is decorative mill trash. Keep it as proof."));
		return false;
	}
	bool bFound = false;
	for (int32 I = 0; I < Inventory.Num(); ++I)
	{
		if (Inventory[I].Id != Id || Inventory[I].Count <= 0)
		{
			continue;
		}
		bFound = true;
		if (Def->Heal <= 0 && Def->Faith <= 0)
		{
			Toast(Def->Description.ToString());
			return false;
		}
		--Inventory[I].Count;
		if (Inventory[I].Count <= 0)
		{
			Inventory.RemoveAt(I);
		}
		HP = FMath::Min(HPMax, HP + Def->Heal);
		if (Affection && Def->Faith > 0)
		{
			Affection->AddFP(Def->Faith);
		}
		PlayCue(TEXT("Shop"));
		Toast(FString::Printf(TEXT("Used %s."), *Def->DisplayName.ToString()));
		return true;
	}
	if (!bFound)
	{
		Toast(TEXT("Pockets empty of that."));
	}
	return false;
}

void AHolypawCharacter::ToggleInventory()
{
	if (Mode == EHolypawPawnMode::Title || Mode == EHolypawPawnMode::Pause || Mode == EHolypawPawnMode::Battle)
	{
		return;
	}
	SetPanel(bInventoryOpen);
}

void AHolypawCharacter::OpenShop()
{
	if (Mode != EHolypawPawnMode::Play)
	{
		return;
	}
	if (!HolypawFaith::ShopsOpen(GetCityHearts(CurrentZone)))
	{
		Toast(HolypawFaith::ShopClosedLine());
		return;
	}
	SetPanel(bShopOpen);
	PlayCue(TEXT("Shop"));
	Toast(HolypawFaith::MillBannersDown(GetCityHearts(CurrentZone))
		? TEXT("Hearts discount. Mill banners are down. Buy something handmade.")
		: TEXT("Hearts discount. The stall likes you."));
}

void AHolypawCharacter::BuyShopSlot(int32 Index)
{
	if (!Affection)
	{
		return;
	}
	if (Index == 0)
	{
		BuyFaith(ShopPrice(10), 8);
		PlayCue(TEXT("Shop"));
		return;
	}
	FName Id = NAME_None;
	if (Index == 1) { Id = TEXT("stuffingBun"); }
	else if (Index == 2) { Id = TEXT("hymnRibbon"); }
	else if (Index == 3) { Id = TEXT("cocoaButton"); }
	const FHolypawItemDef* Def = HolypawCatalog::FindItem(Id);
	if (!Def)
	{
		return;
	}
	const int32 Cost = ShopPrice(Def->ShopCostAP);
	if (!Affection->SpendAP(Cost))
	{
		Toast(TEXT("Not enough AP. Hug someone unfinished."));
		return;
	}
	AddItem(Id, 1);
	PlayCue(TEXT("Shop"));
	Toast(FString::Printf(TEXT("Bought %s for %d AP."), *Def->DisplayName.ToString(), Cost));
}

bool AHolypawCharacter::StartTalk(AHugHuman* Human)
{
	if (!Human)
	{
		return false;
	}
	const FHolypawTalkDef* Talk = HolypawCatalog::FindTalk(Human->PersonName.ToString());
	TalkSpeaker = Human->PersonName.ToString();
	TalkBody = Talk ? Talk->Line : Human->GetBelieverLine();
	TalkHint = Talk ? Talk->Hint : TEXT("Find a gold lantern.");
	bTalkSecond = false;
	bTalkThird = false;
	SetPanel(bTalkOpen);
	PlayCue(TEXT("Talk"));
	return true;
}

void AHolypawCharacter::AdvanceTalk()
{
	ApplyTalkVerb(static_cast<uint8>(HolypawDialogue::ETalkVerb::Listen));
}

void AHolypawCharacter::AskTalkHint()
{
	ApplyTalkVerb(static_cast<uint8>(HolypawDialogue::ETalkVerb::Hint));
}

void AHolypawCharacter::TurnInErrand()
{
	ApplyTalkVerb(static_cast<uint8>(HolypawDialogue::ETalkVerb::TurnIn));
}

void AHolypawCharacter::AcceptQuest()
{
	ApplyTalkVerb(static_cast<uint8>(HolypawDialogue::ETalkVerb::Accept));
}

void AHolypawCharacter::ApplyTalkVerb(uint8 Verb)
{
	HolypawDialogue::FTalkState State;
	State.Speaker = TalkSpeaker;
	State.bTalkOpen = bTalkOpen;
	State.bTalkSecond = bTalkSecond;
	State.TalkBody = TalkBody;
	State.TalkHint = TalkHint;
	State.QuestActive = QuestActive;
	State.QuestDone = QuestDone;
	State.bTalkThird = bTalkThird;
	State.PlayerFP = Affection ? Affection->FP : 0;
	const HolypawDialogue::FTalkOutcome O = HolypawDialogue::Run(
		static_cast<HolypawDialogue::ETalkVerb>(Verb), State, [this](FName Id)
	{
		return GetItemCount(Id);
	});
	if (!O.bHandled)
	{
		return;
	}
	if (!O.TalkBody.IsEmpty())
	{
		TalkBody = O.TalkBody;
	}
	bTalkSecond = O.bTalkSecond;
	bTalkThird = O.bTalkThird;
	if (O.bConsumeItem && !ConsumeItem(O.NeedItem, O.NeedCount))
	{
		Toast(TEXT("Pockets argued. Try again."));
		return;
	}
	if (O.bMarkQuestDone)
	{
		QuestActive.Remove(O.QuestId);
		QuestDone.AddUnique(O.QuestId);
		if (Affection)
		{
			Affection->AddAP(O.RewardAP);
			Affection->AddFP(O.RewardFP);
			if (O.RewardMiracle > 0.f)
			{
				Affection->AddMiracle(O.RewardMiracle);
			}
		}
	}
	if (O.bMarkQuestActive)
	{
		QuestActive.AddUnique(O.QuestId);
	}
	if (O.Cue != NAME_None)
	{
		PlayCue(O.Cue);
	}
	if (!O.Toast.IsEmpty())
	{
		Toast(O.Toast);
	}
	if (O.bCloseTalk)
	{
		ClosePanels();
	}
}

void AHolypawCharacter::SetQuestState(const TArray<FName>& Active, const TArray<FName>& Done)
{
	QuestActive = Active;
	QuestDone = Done;
}

TArray<FString> AHolypawCharacter::GetTalkLines() const
{
	TArray<FString> Lines;
	Lines.Add(TalkSpeaker);
	Lines.Add(TalkBody);
	Lines.Add(TEXT("1  keep listening    2  ask the way    3  turn in    4  take job"));
	if (const FHolypawTalkDef* Talk = HolypawCatalog::FindTalk(TalkSpeaker))
	{
		if (!Talk->LineC.IsEmpty())
		{
			Lines.Add(Talk->FaithNeed > 0
				? FString::Printf(TEXT("Quiet line at %d FP. Keep listening."), Talk->FaithNeed)
				: TEXT("They have a third thought if you keep listening."));
		}
	}
	if (const FHolypawQuestDef* Q = HolypawCatalog::FindQuestByGiver(TalkSpeaker))
	{
		if (QuestDone.Contains(Q->Id))
		{
			Lines.Add(TEXT("Errand: done."));
		}
		else if (QuestActive.Contains(Q->Id))
		{
			Lines.Add(FString::Printf(TEXT("Errand: %s"), *Q->Brief.ToString()));
		}
		else
		{
			Lines.Add(FString::Printf(TEXT("Job available: %s"), *Q->Title.ToString()));
		}
	}
	return Lines;
}

TArray<FString> AHolypawCharacter::GetShopLines() const
{
	TArray<FString> Lines;
	const int32 Hearts = GetCityHearts(CurrentZone);
	if (!HolypawFaith::ShopsOpen(Hearts))
	{
		Lines.Add(TEXT("Faith stall  (shutters down)"));
		Lines.Add(HolypawFaith::ShopClosedLine());
		return Lines;
	}
	Lines.Add(HolypawFaith::MillBannersDown(Hearts)
		? TEXT("Faith stall  (Hearts discount · mill banners down)")
		: TEXT("Faith stall  (Hearts discount)"));
	Lines.Add(FString::Printf(TEXT("1  Faith jar     %d AP -> 8 FP"), ShopPrice(10)));
	if (const FHolypawItemDef* Bun = HolypawCatalog::FindItem(TEXT("stuffingBun")))
	{
		Lines.Add(FString::Printf(TEXT("2  %s     %d AP"), *Bun->DisplayName.ToString(), ShopPrice(Bun->ShopCostAP)));
	}
	if (const FHolypawItemDef* Rib = HolypawCatalog::FindItem(TEXT("hymnRibbon")))
	{
		Lines.Add(FString::Printf(TEXT("3  %s     %d AP"), *Rib->DisplayName.ToString(), ShopPrice(Rib->ShopCostAP)));
	}
	if (const FHolypawItemDef* Cocoa = HolypawCatalog::FindItem(TEXT("cocoaButton")))
	{
		Lines.Add(FString::Printf(TEXT("4  %s     %d AP"), *Cocoa->DisplayName.ToString(), ShopPrice(Cocoa->ShopCostAP)));
	}
	Lines.Add(TEXT("Esc closes. I opens your pockets."));
	return Lines;
}

TArray<FString> AHolypawCharacter::GetInventoryLines() const
{
	TArray<FString> Lines;
	Lines.Add(TEXT("Pockets"));
	if (Inventory.Num() == 0)
	{
		Lines.Add(TEXT("Empty. Stalls sell buns. Mills drop scrap."));
	}
	else
	{
		for (int32 I = 0; I < Inventory.Num(); ++I)
		{
			const FHolypawItemDef* Def = HolypawCatalog::FindItem(Inventory[I].Id);
			const FString Name = Def ? Def->DisplayName.ToString() : Inventory[I].Id.ToString();
			Lines.Add(FString::Printf(TEXT("%d  %s x%d"), I + 1, *Name, Inventory[I].Count));
			if (Def)
			{
				Lines.Add(TEXT("    ") + Def->Description.ToString());
			}
		}
		Lines.Add(TEXT("1-6 use    I close"));
	}
	return Lines;
}
