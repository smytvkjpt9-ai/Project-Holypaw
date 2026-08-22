#include "Character/HolypawCharacter.h"
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
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"

AHolypawCharacter::AHolypawCharacter()
{
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
	SpringArm->TargetArmLength = 480.f;
	SpringArm->SocketOffset = FVector(0.f, 50.f, 40.f);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->ProbeSize = 12.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 70.f;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	BodyMesh->SetRelativeScale3D(FVector(0.85f, 0.7f, 0.75f));

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	HeadMesh->SetupAttachment(RootComponent);
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadMesh->SetRelativeLocation(FVector(0.f, 0.f, 42.f));
	HeadMesh->SetRelativeScale3D(FVector(0.55f, 0.55f, 0.55f));

	HaloMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Halo"));
	HaloMesh->SetupAttachment(HeadMesh);
	HaloMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HaloMesh->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	HaloMesh->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.08f));
	HaloMesh->SetHiddenInGame(true);

	EarL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EarL"));
	EarL->SetupAttachment(HeadMesh);
	EarL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EarR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EarR"));
	EarR->SetupAttachment(HeadMesh);
	EarR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Snout = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Snout"));
	Snout->SetupAttachment(HeadMesh);
	Snout->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PawL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawL"));
	PawL->SetupAttachment(BodyMesh);
	PawL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PawR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawR"));
	PawR->SetupAttachment(BodyMesh);
	PawR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Belly = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Belly"));
	Belly->SetupAttachment(BodyMesh);
	Belly->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EyeL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeL"));
	EyeL->SetupAttachment(HeadMesh);
	EyeL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EyeR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeR"));
	EyeR->SetupAttachment(HeadMesh);
	EyeR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EarL->SetRelativeLocation(FVector(-8.f, 18.f, 28.f));
	EarL->SetRelativeRotation(FRotator(12.f, 0.f, -18.f));
	EarL->SetRelativeScale3D(FVector(0.18f, 0.12f, 0.32f));
	EarR->SetRelativeLocation(FVector(-8.f, -18.f, 28.f));
	EarR->SetRelativeRotation(FRotator(12.f, 0.f, 18.f));
	EarR->SetRelativeScale3D(FVector(0.18f, 0.12f, 0.32f));
	Snout->SetRelativeLocation(FVector(22.f, 0.f, -4.f));
	Snout->SetRelativeScale3D(FVector(0.22f, 0.18f, 0.16f));
	PawL->SetRelativeLocation(FVector(8.f, 22.f, -18.f));
	PawL->SetRelativeScale3D(FVector(0.22f, 0.18f, 0.12f));
	PawR->SetRelativeLocation(FVector(8.f, -22.f, -18.f));
	PawR->SetRelativeScale3D(FVector(0.22f, 0.18f, 0.12f));
	Belly->SetRelativeLocation(FVector(12.f, 0.f, -4.f));
	Belly->SetRelativeScale3D(FVector(0.55f, 0.45f, 0.45f));
	EyeL->SetRelativeLocation(FVector(18.f, 8.f, 6.f));
	EyeL->SetRelativeScale3D(FVector(0.08f, 0.08f, 0.08f));
	EyeR->SetRelativeLocation(FVector(18.f, -8.f, 6.f));
	EyeR->SetRelativeScale3D(FVector(0.08f, 0.08f, 0.08f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TorusFinder(TEXT("/Engine/BasicShapes/Torus.Torus"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeFinder(TEXT("/Engine/BasicShapes/Cone.Cone"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (SphereFinder.Succeeded())
	{
		SphereMesh = SphereFinder.Object;
		BodyMesh->SetStaticMesh(SphereFinder.Object);
		HeadMesh->SetStaticMesh(SphereFinder.Object);
		Snout->SetStaticMesh(SphereFinder.Object);
		PawL->SetStaticMesh(SphereFinder.Object);
		PawR->SetStaticMesh(SphereFinder.Object);
		Belly->SetStaticMesh(SphereFinder.Object);
		EyeL->SetStaticMesh(SphereFinder.Object);
		EyeR->SetStaticMesh(SphereFinder.Object);
	}
	if (ConeFinder.Succeeded())
	{
		ConeMesh = ConeFinder.Object;
		EarL->SetStaticMesh(ConeFinder.Object);
		EarR->SetStaticMesh(ConeFinder.Object);
	}
	else if (SphereFinder.Succeeded())
	{
		EarL->SetStaticMesh(SphereFinder.Object);
		EarR->SetStaticMesh(SphereFinder.Object);
	}
	if (TorusFinder.Succeeded())
	{
		HaloMesh->SetStaticMesh(TorusFinder.Object);
	}
	else if (SphereFinder.Succeeded())
	{
		HaloMesh->SetStaticMesh(SphereFinder.Object);
	}
	if (MatFinder.Succeeded())
	{
		ShapeMat = MatFinder.Object;
		TArray<UStaticMeshComponent*> Parts = { BodyMesh, HeadMesh, HaloMesh, EarL, EarR, Snout, PawL, PawR, Belly, EyeL, EyeR };
		for (UStaticMeshComponent* P : Parts)
		{
			if (P) { P->SetMaterial(0, ShapeMat); }
		}
	}

	Affection = CreateDefaultSubobject<UAffectionComponent>(TEXT("Affection"));
	Skills = CreateDefaultSubobject<USkillTreeComponent>(TEXT("Skills"));
	Party = CreateDefaultSubobject<UPartyComponent>(TEXT("Party"));
	Story = CreateDefaultSubobject<UHolypawMissionComponent>(TEXT("Story"));
}

void AHolypawCharacter::BeginPlay()
{
	Super::BeginPlay();
	Colorize(BodyMesh, FLinearColor(0.91f, 0.62f, 0.55f));
	Colorize(HeadMesh, FLinearColor(0.95f, 0.72f, 0.66f));
	Colorize(HaloMesh, FLinearColor(1.f, 0.9f, 0.45f));
	Colorize(EarL, FLinearColor(0.88f, 0.55f, 0.5f));
	Colorize(EarR, FLinearColor(0.88f, 0.55f, 0.5f));
	Colorize(Snout, FLinearColor(0.98f, 0.82f, 0.76f));
	Colorize(PawL, FLinearColor(0.85f, 0.52f, 0.48f));
	Colorize(PawR, FLinearColor(0.85f, 0.52f, 0.48f));
	Colorize(Belly, FLinearColor(0.98f, 0.9f, 0.82f));
	Colorize(EyeL, FLinearColor(0.12f, 0.1f, 0.12f));
	Colorize(EyeR, FLinearColor(0.12f, 0.1f, 0.12f));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	UnlockTravel(EHolypawZone::ForestCottage);
	Mode = EHolypawPawnMode::Title;
	if (BodyMesh) { BodyBase = BodyMesh->GetRelativeLocation(); }
	if (HeadMesh) { HeadBase = HeadMesh->GetRelativeLocation(); }
	if (EarL) { EarLBase = EarL->GetRelativeLocation(); EarLRot = EarL->GetRelativeRotation(); }
	if (EarR) { EarRBase = EarR->GetRelativeLocation(); EarRRot = EarR->GetRelativeRotation(); }
	if (PawL) { PawLBase = PawL->GetRelativeLocation(); PawLRot = PawL->GetRelativeRotation(); }
	if (PawR) { PawRBase = PawR->GetRelativeLocation(); PawRRot = PawR->GetRelativeRotation(); }
	if (EyeL) { EyeLScale = EyeL->GetRelativeScale3D(); }
	if (EyeR) { EyeRScale = EyeR->GetRelativeScale3D(); }
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
	PlayCue(TEXT("Title"));
	Toast(TEXT("The Fluffy Ascendancy — pick a slot. The porch is already under you."));
}

void AHolypawCharacter::Colorize(UStaticMeshComponent* Comp, const FLinearColor& Color)
{
	if (!Comp || !ShapeMat)
	{
		return;
	}
	if (UMaterialInstanceDynamic* Mid = Comp->CreateDynamicMaterialInstance(0, ShapeMat))
	{
		Mid->SetVectorParameterValue(TEXT("Color"), Color);
	}
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
	if (Mode != EHolypawPawnMode::Play || FMath::IsNearlyZero(Value))
	{
		return;
	}
	const FRotator Yaw(0.f, GetControlRotation().Yaw, 0.f);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), Value);
}

void AHolypawCharacter::MoveRight(float Value)
{
	if (Mode != EHolypawPawnMode::Play || FMath::IsNearlyZero(Value))
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
	if (HurtPulse > 0.f)
	{
		HurtPulse = FMath::Max(0.f, HurtPulse - DeltaSeconds * 3.f);
	}
	if (HugAnim > 0.f)
	{
		HugAnim = FMath::Max(0.f, HugAnim - DeltaSeconds * 2.2f);
	}
	TickProcAnim(DeltaSeconds);
	if (SpringArm)
	{
		const float Want = Mode == EHolypawPawnMode::Battle ? BattleArm : ExploreArm;
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, Want, DeltaSeconds, 6.f);
	}
	if (Mode == EHolypawPawnMode::Play)
	{
		UpdateZone();
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
	Trail.Insert(GetActorLocation(), 0);
	if (Trail.Num() > 80)
	{
		Trail.SetNum(80);
	}

	const int32 Wanted = Party ? Party->Members.Num() : 0;
	while (FollowerMeshes.Num() < Wanted && SphereMesh)
	{
		UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(this);
		Comp->SetStaticMesh(SphereMesh);
		Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Comp->SetMobility(EComponentMobility::Movable);
		Comp->CreationMethod = EComponentCreationMethod::Instance;
		AddInstanceComponent(Comp);
		Comp->RegisterComponent();
		Comp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		if (ShapeMat)
		{
			Comp->SetMaterial(0, ShapeMat);
		}
		const int32 Idx = FollowerMeshes.Num();
		if (Party->Members.IsValidIndex(Idx))
		{
			Colorize(Comp, Party->Members[Idx].Color);
		}
		Comp->SetWorldScale3D(FVector(0.35f));
		FollowerMeshes.Add(Comp);
	}
	while (FollowerMeshes.Num() > Wanted)
	{
		UStaticMeshComponent* Comp = FollowerMeshes.Pop();
		if (Comp)
		{
			Comp->DestroyComponent();
		}
	}

	for (int32 I = 0; I < FollowerMeshes.Num(); ++I)
	{
		UStaticMeshComponent* Comp = FollowerMeshes[I];
		if (!Comp)
		{
			continue;
		}
		const int32 Idx = FMath::Min(Trail.Num() - 1, (I + 1) * 10);
		if (!Trail.IsValidIndex(Idx))
		{
			continue;
		}
		const FVector Target = Trail[Idx] + FVector(0.f, 0.f, 30.f);
		const FVector NewLoc = FMath::VInterpTo(Comp->GetComponentLocation(), Target, DeltaSeconds, 8.f);
		Comp->SetWorldLocation(NewLoc);
	}
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
	Lines.Add(FString::Printf(TEXT("City Hearts here  %d    Tab cycle lanterns  E travel"), GetCityHearts(CurrentZone)));
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
		if (!I || I == this)
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
	Toast(FString::Printf(TEXT("%s joined! Tiny hench-fluff acquired."), *Fluffy->Type.DisplayName.ToString()));
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
	HugLock = 0.28f;
	HugAnim = 1.f;
	Human->ReceiveHug();
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
		Affection->AddMiracle(22.f);
		AddCityHeart(CurrentZone);
		if (Story)
		{
			Story->NotifyConvert();
		}
		Toast(FString::Printf(TEXT("%s's last serious thought fell out. %s"),
			*Human->PersonName.ToString(), *Human->GetBelieverLine()));
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
			Skills->HasSkill(TEXT("haloStep")), E->bBlocksFlee, E->IsBoss());
		if (FMath::FRand() < Chance)
		{
			BattleLog = TEXT("You scampered away!");
			GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EndBattle, 0.7f, false);
		}
		else
		{
			BattleLog = E->bBlocksFlee
				? FString::Printf(TEXT("%s blocks the path. No escape."), *E->DisplayName.ToString())
				: TEXT("Blocked! No escape.");
			GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.7f, false);
		}
		return;
	}

	if (Kind == TEXT("guard"))
	{
		bGuarding = true;
		const int32 Stitch = FMath::Max(4, HolypawBattleDirector::AbilityStitch(TEXT("guard")));
		HP = FMath::Min(HPMax, HP + Stitch);
		BattleLog = Skills->HasSkill(TEXT("seamGuard"))
			? FString::Printf(TEXT("Seam Guard + stitch %d stuffing."), Stitch)
			: FString::Printf(TEXT("You guard your seams and stitch %d."), Stitch);
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.55f, false);
		return;
	}

	if (Kind == TEXT("hymn"))
	{
		const int32 Cost = FMath::Max(8, HolypawBattleDirector::AbilityFpCost(TEXT("hymn")));
		if (!Affection->SpendFP(Cost))
		{
			BattleLog = FString::Printf(TEXT("Need %d FP for a Hymn!"), Cost);
			bBattleBusy = false;
			return;
		}
		int32 Heal = Skills->HasSkill(TEXT("hymnWard")) ? 18 : 10;
		HP = FMath::Min(HPMax, HP + Heal);
		PoisonTurns = 0;
		HymnShield = 2;
		BattleLog = FString::Printf(TEXT("Hymn mends %d stuffing, clears poison, and raises a shield."), Heal);
		if (HolypawBattleDirector::RollLullaby())
		{
			BattleLog += TEXT(" Lullaby — they snooze a turn.");
			GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::ResumePlayerTurn, 0.75f, false);
			return;
		}
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.7f, false);
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
		if (Skills->HasSkill(TEXT("choirAssault")))
		{
			Dmg += Dmg / 2;
		}
		if (bPartyCut)
		{
			Dmg = FMath::Max(1, Dmg / 2);
			BattleLog = FString::Printf(TEXT("Snipped Party Assault deals %d."), Dmg);
		}
		else
		{
			BattleLog = Party->Members.Num() > 0
				? FString::Printf(TEXT("Party Assault deals %d!"), Dmg)
				: FString::Printf(TEXT("Lonely swipe for %d. Find fluffies!"), Dmg);
		}
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
		Dmg, FrostTurns, E->Special, Kind == TEXT("beam"), bCrit, BattleLog);
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
	PlayCue(TEXT("BattleHit"));
	if (E->TryEnterPhaseTwo())
	{
		BattleLog += TEXT(" ");
		BattleLog += E->GetPhaseLine();
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
		return;
	}

	++BattleTurn;
	int32 Dmg = En->Attack + FMath::RandRange(0, 3);
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
	Req.bGuarding = bGuarding;
	Req.bSeamGuard = Skills->HasSkill(TEXT("seamGuard"));
	Req.HymnShield = HymnShield;
	Req.bFaithArmor = Skills->HasSkill(TEXT("faithArmor"));
	Req.bFluffShield = Skills->HasSkill(TEXT("fluffShield"));
	Req.PlayerFP = Affection ? Affection->FP : 0;
	Req.MiracleCharge = Affection ? Affection->MiracleCharge : 0.f;
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
	}
	HymnShield = In.HymnShieldLeft;
	const FString Extra = In.Extra;
	HP -= Dmg;
	LastDamageTaken = Dmg;
	HurtPulse = 1.f;
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
	HurtPulse = 0.7f;
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
		Toast(TEXT("Spire Inn tucks you in. HP full, cocoa on the nightstand. Saved."));
		return true;
	case EHolypawShrineKind::Chapel:
		if (Affection)
		{
			Affection->MiracleCharge = Affection->MiracleMax;
		}
		PlayCue(TEXT("Miracle"));
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
		PlayCue(TEXT("Miracle"));
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
		PlayCue(TEXT("Shop"));
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
		H->ReceiveHug();
		if (bWasOpen && H->ConvertProgress >= 100.f)
		{
			H->BecomeBeliever();
			++NewlyConvinced;
			AddCityHeart(CurrentZone);
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
	FString MiracleToast = FString::Printf(TEXT("Miracle hymn! +%d FP. %d human(s) dropped their last independent thought."),
		FpGain, NewlyConvinced);
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
		Lines.Add(TEXT("Talk 4 takes a job. Talk 3 turns it in. Home Sewn, Old World, Sand Belt, Globe Trek."));
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
			H->BecomeBeliever();
			H->KneelInWorship();
		}
	}
	HaloMesh->SetHiddenInGame(false);
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
	PlayerBattleAttack(TEXT("slap"));
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
	PlayerBattleAttack(TEXT("beam"));
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
	PlayerBattleAttack(TEXT("party"));
}

void AHolypawCharacter::BattleFlee()
{
	if (bSkillsOpen || bTalkOpen || bShopOpen || bInventoryOpen)
	{
		TryBuyTreeSlot(3);
		return;
	}
	PlayerBattleAttack(TEXT("flee"));
}

void AHolypawCharacter::Skill5()
{
	if (bSkillsOpen || bTalkOpen || bShopOpen || bInventoryOpen)
	{
		TryBuyTreeSlot(4);
		return;
	}
	PlayerBattleAttack(TEXT("guard"));
}

void AHolypawCharacter::Skill6()
{
	if (bSkillsOpen || bTalkOpen || bShopOpen || bInventoryOpen)
	{
		TryBuyTreeSlot(5);
		return;
	}
	PlayerBattleAttack(TEXT("hymn"));
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

void AHolypawCharacter::AddCityHeart(EHolypawZone Zone, int32 Amount)
{
	if (!HolypawCatalog::IsCityZone(Zone) && Zone != EHolypawZone::ForestCottage)
	{
		Zone = CurrentZone;
	}
	for (FHolypawHeartRecord& Rec : CityHearts)
	{
		if (Rec.Zone == Zone)
		{
			Rec.Hearts += Amount;
			UnlockTravel(Zone);
			return;
		}
	}
	FHolypawHeartRecord Rec;
	Rec.Zone = Zone;
	Rec.Hearts = Amount;
	CityHearts.Add(Rec);
	UnlockTravel(Zone);
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
	for (TActorIterator<AHolypawWorldBuilder> It(GetWorld()); It; ++It)
	{
		Loc = It->GetTravelLocation(Dest);
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
		TitleLoad();
	}
	else
	{
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

void AHolypawCharacter::TickProcAnim(float DeltaSeconds)
{
	if (Mode == EHolypawPawnMode::Title || Mode == EHolypawPawnMode::Pause)
	{
		return;
	}
	AnimT += DeltaSeconds;
	BlinkT -= DeltaSeconds;
	const float Speed = GetVelocity().Size();
	const float Walk = FMath::Clamp(Speed / 700.f, 0.f, 1.2f);
	const float Bob = FMath::Sin(AnimT * (7.f + Walk * 5.f)) * 6.f * FMath::Max(0.15f, Walk);
	const float Squash = HurtPulse * 0.16f;
	if (BodyMesh)
	{
		BodyMesh->SetRelativeLocation(BodyBase + FVector(0.f, 0.f, Bob - Squash * 8.f));
		BodyMesh->SetRelativeScale3D(FVector(0.85f + Squash, 0.7f + Squash, 0.75f - Squash));
	}
	if (HeadMesh)
	{
		HeadMesh->SetRelativeLocation(HeadBase + FVector(0.f, 0.f, Bob * 0.6f));
	}
	const float Ear = FMath::Sin(AnimT * 5.5f) * (8.f + Walk * 10.f);
	if (EarL)
	{
		EarL->SetRelativeRotation(EarLRot + FRotator(Ear, 0.f, HugAnim * -12.f));
	}
	if (EarR)
	{
		EarR->SetRelativeRotation(EarRRot + FRotator(-Ear, 0.f, HugAnim * 12.f));
	}
	if (PawL)
	{
		PawL->SetRelativeRotation(PawLRot + FRotator(HugAnim * 28.f, 0.f, HugAnim * 18.f));
	}
	if (PawR)
	{
		PawR->SetRelativeRotation(PawRRot + FRotator(HugAnim * 28.f, 0.f, HugAnim * -18.f));
	}
	float EyeY = 1.f;
	if (BlinkT < 0.12f)
	{
		EyeY = 0.12f;
	}
	if (BlinkT < 0.f)
	{
		BlinkT = 2.2f + FMath::FRandRange(0.f, 2.5f);
	}
	if (EyeL)
	{
		EyeL->SetRelativeScale3D(FVector(EyeLScale.X, EyeLScale.Y, EyeLScale.Z * EyeY));
	}
	if (EyeR)
	{
		EyeR->SetRelativeScale3D(FVector(EyeRScale.X, EyeRScale.Y, EyeRScale.Z * EyeY));
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

FString AHolypawCharacter::GetBattleStatusLine() const
{
	return HolypawBattle::FormatStatus(SlapCombo, PoisonTurns, FrostTurns, HymnShield, bEnemyStaggered);
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
	SetPanel(bShopOpen);
	PlayCue(TEXT("Shop"));
	Toast(GetCityHearts(CurrentZone) >= 1
		? TEXT("Hearts discount. The stall likes you.")
		: TEXT("Faith stall. Convert locals for cheaper buns."));
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
	Lines.Add(GetCityHearts(CurrentZone) >= 1 ? TEXT("Faith stall  (Hearts discount)") : TEXT("Faith stall"));
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
