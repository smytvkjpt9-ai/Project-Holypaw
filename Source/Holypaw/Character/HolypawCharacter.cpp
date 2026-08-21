#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"
#include "Components/SkillTreeComponent.h"
#include "Components/MissionComponent.h"
#include "Actors/WildFluffy.h"
#include "Actors/HugHuman.h"
#include "Actors/HostilePet.h"
#include "Actors/HolypawInteractable.h"
#include "HolypawWorldBuilder.h"
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

	auto MakePart = [this](TObjectPtr<UStaticMeshComponent>& Out, const TCHAR* Name, USceneComponent* Parent)
	{
		Out = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Out->SetupAttachment(Parent);
		Out->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	};
	MakePart(EarL, TEXT("EarL"), HeadMesh);
	MakePart(EarR, TEXT("EarR"), HeadMesh);
	MakePart(Snout, TEXT("Snout"), HeadMesh);
	MakePart(PawL, TEXT("PawL"), BodyMesh);
	MakePart(PawR, TEXT("PawR"), BodyMesh);
	MakePart(Belly, TEXT("Belly"), BodyMesh);
	MakePart(EyeL, TEXT("EyeL"), HeadMesh);
	MakePart(EyeR, TEXT("EyeR"), HeadMesh);
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
	if (Story)
	{
		Story->TryAdvance(this);
	}
	Toast(TEXT("Wake in Stuffed Park. J journal. K three skill trees. The Poly Mill sells cheap polyester — not handmade."));
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
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
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
	if (Invuln > 0.f)
	{
		Invuln -= DeltaSeconds;
	}
	if (ToastTime > 0.f)
	{
		ToastTime -= DeltaSeconds;
	}
	if (SpringArm)
	{
		const float Want = Mode == EHolypawPawnMode::Battle ? BattleArm : ExploreArm;
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, Want, DeltaSeconds, 6.f);
	}
	if (Mode == EHolypawPawnMode::Play)
	{
		UpdateZone();
		if (AActor* Near = FindNearestInteractable(220.f))
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
		Toast(FString::Printf(TEXT("Entered %s"), HolypawCatalog::ZoneDisplayName(Z)));
		if (Story)
		{
			Story->NotifyZone(Z);
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
	if (Mode != EHolypawPawnMode::Play)
	{
		if (Mode == EHolypawPawnMode::UI)
		{
			ClosePanels();
		}
		return;
	}
	if (AHolypawInteractable* I = Cast<AHolypawInteractable>(FindNearestInteractable(220.f)))
	{
		I->Interact(this);
	}
	else
	{
		Toast(TEXT("Nothing nearby. Follow the lanterns toward the city."));
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
		Toast(TEXT("Party full (4). Open Party with P."));
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
	Toast(FString::Printf(TEXT("%s joined your party!"), *Fluffy->Type.DisplayName.ToString()));
	return true;
}

bool AHolypawCharacter::HugPerson(AHugHuman* Human)
{
	if (!Human || Mode != EHolypawPawnMode::Play)
	{
		return false;
	}
	int32 Gain = 12 + FMath::RandRange(0, 7);
	if (Skills->HasSkill(TEXT("buttonEyes")))
	{
		Gain = FMath::FloorToInt(Gain * 1.2f);
	}
	if (Skills->HasSkill(TEXT("irresistible")))
	{
		Gain = FMath::FloorToInt(Gain * 1.5f);
	}
	Affection->AddAP(Gain);
	const bool bWasOpen = Human->ConvertProgress < 100.f;
	float Progress = Gain;
	if (Skills->HasSkill(TEXT("deepHug")))
	{
		Progress += 15.f;
	}
	Human->ConvertProgress = FMath::Min(100.f, Human->ConvertProgress + Progress);
	if (Skills->HasSkill(TEXT("stitchHeart")))
	{
		HP = FMath::Min(HPMax, HP + 8);
	}
	if (bWasOpen && Human->ConvertProgress >= 100.f)
	{
		Human->BecomeBeliever();
		Affection->AddMiracle(20.f);
		if (Story)
		{
			Story->NotifyConvert();
		}
		Toast(FString::Printf(TEXT("%s believes! Miracle Charge surges."), *Human->PersonName.ToString()));
	}
	else if (Human->ConvertProgress < 100.f)
	{
		Toast(FString::Printf(TEXT("+%d AP from a hug."), Gain));
	}
	else
	{
		Toast(FString::Printf(TEXT("%s already keeps the Bear Faith."), *Human->PersonName.ToString()));
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
		Toast(FString::Printf(TEXT("Codex: %s logged."), *Enemy->DisplayName.ToString()));
	}
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeGameAndUI());
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

	if (Kind == TEXT("flee"))
	{
		float Chance = 0.55f + (Skills->HasSkill(TEXT("haloStep")) ? 0.15f : 0.f);
		if (E->bBlocksFlee || E->IsBoss())
		{
			Chance = 0.12f + (Skills->HasSkill(TEXT("haloStep")) ? 0.08f : 0.f);
		}
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
		BattleLog = Skills->HasSkill(TEXT("seamGuard"))
			? TEXT("Seam Guard: you brace almost the whole next rip.")
			: TEXT("You guard your seams.");
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.55f, false);
		return;
	}

	if (Kind == TEXT("hymn"))
	{
		const int32 Cost = 8;
		if (!Affection->SpendFP(Cost))
		{
			BattleLog = TEXT("Need 8 FP for a Hymn!");
			bBattleBusy = false;
			return;
		}
		int32 Heal = Skills->HasSkill(TEXT("hymnWard")) ? 18 : 10;
		HP = FMath::Min(HPMax, HP + Heal);
		BattleLog = FString::Printf(TEXT("Hymn mends %d stuffing."), Heal);
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.7f, false);
		return;
	}

	int32 Dmg = 0;
	if (Kind == TEXT("slap"))
	{
		Dmg = Attack + FMath::RandRange(0, 4);
		if (Skills->HasSkill(TEXT("bearPaw")))
		{
			Dmg += Attack / 2;
			BattleLog = FString::Printf(TEXT("Bear Paw slaps twice for %d!"), Dmg);
		}
		else
		{
			BattleLog = FString::Printf(TEXT("Soft Slap hits for %d!"), Dmg);
		}
	}
	else if (Kind == TEXT("beam"))
	{
		if (!Affection->SpendFP(12))
		{
			BattleLog = TEXT("Need 12 FP!");
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

	if (E->Special == EVillainSpecial::ArmorPlates)
	{
		Dmg = FMath::Max(1, FMath::FloorToInt(Dmg * 0.72f));
		BattleLog += TEXT(" Armor plates dull it.");
	}

	E->HP -= Dmg;
	GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.7f, false);
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
		return;
	}

	++BattleTurn;
	int32 Dmg = En->Attack + FMath::RandRange(0, 3);
	FString Extra;
	const FString Verb = En->GetDef().AttackLine.IsEmpty() ? TEXT("shreds stuffing") : En->GetDef().AttackLine;

	switch (En->Special)
	{
	case EVillainSpecial::DrainFaith:
	{
		const int32 Drain = FMath::Min(6, Affection->FP);
		if (Drain > 0)
		{
			Affection->SpendFP(Drain);
			Extra = FString::Printf(TEXT("  -%d FP."), Drain);
		}
		break;
	}
	case EVillainSpecial::StealMiracle:
	{
		const float Stolen = FMath::Min(10.f, Affection->MiracleCharge);
		Affection->MiracleCharge = FMath::Max(0.f, Affection->MiracleCharge - Stolen);
		Extra = TEXT("  Miracle Charge nipped.");
		break;
	}
	case EVillainSpecial::DoubleStrike:
		Dmg += En->Attack / 2 + FMath::RandRange(0, 2);
		Extra = TEXT("  Twice!");
		break;
	case EVillainSpecial::FrostBite:
		Dmg += 2;
		Extra = TEXT("  Seams go numb.");
		break;
	case EVillainSpecial::PoisonThread:
		Dmg += 3;
		Extra = TEXT("  Poison thread itches.");
		break;
	case EVillainSpecial::Rage:
		if (En->HP * 2 <= En->HPMax)
		{
			Dmg = FMath::FloorToInt(Dmg * 1.5f);
			Extra = TEXT("  RAGE.");
		}
		break;
	case EVillainSpecial::ThreadCut:
		bPartyCut = true;
		Extra = TEXT("  Party ribbons snipped.");
		break;
	case EVillainSpecial::FaithBurn:
		Dmg += FMath::Max(0, Affection->FP / 8);
		Extra = TEXT("  Faith flares against you.");
		break;
	default:
		break;
	}

	if (En->Rank == EVillainRank::WorldBoss && BattleTurn >= 4)
	{
		Dmg += 4;
		Extra += TEXT("  The air unravels.");
	}

	if (Skills->HasSkill(TEXT("faithArmor")))
	{
		Dmg = FMath::Max(1, Dmg - 2);
	}
	if (Skills->HasSkill(TEXT("fluffShield")))
	{
		Dmg = FMath::Max(1, Dmg - 3);
	}
	if (bGuarding)
	{
		const float Keep = Skills->HasSkill(TEXT("seamGuard")) ? 0.25f : 0.45f;
		Dmg = FMath::Max(1, FMath::FloorToInt(Dmg * Keep));
		bGuarding = false;
		Extra += TEXT("  Guarded.");
	}
	HP -= Dmg;
	BattleLog = En->DisplayName.ToString() + TEXT(" ") + Verb + FString::Printf(TEXT(" for %d!"), Dmg) + Extra;
	if (HP <= 0)
	{
		HP = 0;
		BattleLog = TEXT("Unstuffed… waking at the cottage.");
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::FailAndWakeAtCottage, 1.0f, false);
		return;
	}
	GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::ResumePlayerTurn, 0.65f, false);
}

void AHolypawCharacter::ResumePlayerTurn()
{
	bPlayerTurn = true;
	bBattleBusy = false;
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
}

void AHolypawCharacter::EndBattle()
{
	GetWorldTimerManager().ClearTimer(BattleTimer);
	BattleEnemy = nullptr;
	Mode = EHolypawPawnMode::Play;
	bBattleBusy = false;
	bGuarding = false;
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
	Toast(FString::Printf(TEXT("%s unstuffed! +%d AP · +%d FP"), *Name, ApGain, FpGain));
}

void AHolypawCharacter::RestFully()
{
	HP = HPMax;
	Toast(TEXT("Your stuffing is mended."));
}

bool AHolypawCharacter::BuyFaith(int32 ApCost, int32 FpGain)
{
	if (!Affection->SpendAP(ApCost))
	{
		Toast(TEXT("Not enough AP for the faith stall."));
		return false;
	}
	Affection->AddFP(FpGain);
	Toast(FString::Printf(TEXT("Traded %d AP for %d FP."), ApCost, FpGain));
	return true;
}

void AHolypawCharacter::TryMiracle()
{
	if (Mode != EHolypawPawnMode::Play)
	{
		return;
	}
	if (!Affection->IsMiracleReady())
	{
		Toast(TEXT("Miracle not ready — hug and recruit to charge."));
		return;
	}
	int32 FpGain = 40 + Party->Members.Num() * 8;
	if (Skills->HasSkill(TEXT("miracleEcho")))
	{
		FpGain = FMath::FloorToInt(FpGain * 1.5f);
	}
	Affection->ResetMiracleCharge();
	Affection->AddFP(FpGain);
	HaloMesh->SetHiddenInGame(false);
	int32 Heal = Skills->HasSkill(TEXT("peakLiturgy")) ? 30 : 15;
	HP = FMath::Min(HPMax, HP + Heal);
	if (Skills->HasSkill(TEXT("bearCreed")))
	{
		for (TActorIterator<AHugHuman> It(GetWorld()); It; ++It)
		{
			AHugHuman* H = *It;
			if (!H || FVector::Dist(GetActorLocation(), H->GetActorLocation()) > 2200.f)
			{
				continue;
			}
			const bool bWasOpen = H->ConvertProgress < 100.f;
			H->ConvertProgress = FMath::Min(100.f, H->ConvertProgress + 40.f);
			if (bWasOpen && H->ConvertProgress >= 100.f)
			{
				H->BecomeBeliever();
				if (Story)
				{
					Story->NotifyConvert();
				}
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
	Toast(FString::Printf(TEXT("Miracle! Faith floods the land (+%d FP)."), FpGain));
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
	if (Mode == EHolypawPawnMode::Battle)
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
	SetPanel(bSkillsOpen);
}

void AHolypawCharacter::ToggleParty()
{
	SetPanel(bPartyOpen);
}

void AHolypawCharacter::ToggleMap()
{
	SetPanel(bMapOpen);
}

void AHolypawCharacter::ToggleCodex()
{
	SetPanel(bCodexOpen);
}

void AHolypawCharacter::ToggleJournal()
{
	SetPanel(bJournalOpen);
}

void AHolypawCharacter::CycleSkillTree()
{
	if (!bSkillsOpen || !Skills)
	{
		return;
	}
	Skills->CycleTree();
	Toast(HolypawCatalog::SkillTreeName(Skills->ActiveTree));
}

TArray<FString> AHolypawCharacter::GetJournalLines() const
{
	if (Story)
	{
		return Story->GetJournalLines();
	}
	return {};
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
	Toast(TEXT("The Bear Faith holds. People keep handmade fluff. The Poly Mill's cheap empire unravels."));
}

void AHolypawCharacter::CloseOrJump()
{
	if (Mode == EHolypawPawnMode::UI)
	{
		ClosePanels();
	}
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
	Toast(TEXT("Skill unlocked!"));
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
	if (bSkillsOpen)
	{
		TryBuyTreeSlot(0);
		return;
	}
	PlayerBattleAttack(TEXT("slap"));
}

void AHolypawCharacter::BattleBeam()
{
	if (bSkillsOpen)
	{
		TryBuyTreeSlot(1);
		return;
	}
	PlayerBattleAttack(TEXT("beam"));
}

void AHolypawCharacter::BattlePartyAtk()
{
	if (bSkillsOpen)
	{
		TryBuyTreeSlot(2);
		return;
	}
	PlayerBattleAttack(TEXT("party"));
}

void AHolypawCharacter::BattleFlee()
{
	if (bSkillsOpen)
	{
		TryBuyTreeSlot(3);
		return;
	}
	PlayerBattleAttack(TEXT("flee"));
}

void AHolypawCharacter::Skill5()
{
	if (bSkillsOpen)
	{
		TryBuyTreeSlot(4);
		return;
	}
	PlayerBattleAttack(TEXT("guard"));
}

void AHolypawCharacter::Skill6()
{
	if (bSkillsOpen)
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
