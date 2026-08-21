#include "Character/HolypawCharacter.h"
#include "Components/AffectionComponent.h"
#include "Components/SkillTreeComponent.h"
#include "Components/PartyComponent.h"
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TorusFinder(TEXT("/Engine/BasicShapes/Torus.Torus"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (SphereFinder.Succeeded())
	{
		SphereMesh = SphereFinder.Object;
		BodyMesh->SetStaticMesh(SphereFinder.Object);
		HeadMesh->SetStaticMesh(SphereFinder.Object);
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
		BodyMesh->SetMaterial(0, ShapeMat);
		HeadMesh->SetMaterial(0, ShapeMat);
		HaloMesh->SetMaterial(0, ShapeMat);
	}

	Affection = CreateDefaultSubobject<UAffectionComponent>(TEXT("Affection"));
	Skills = CreateDefaultSubobject<USkillTreeComponent>(TEXT("Skills"));
	Party = CreateDefaultSubobject<UPartyComponent>(TEXT("Party"));
}

void AHolypawCharacter::BeginPlay()
{
	Super::BeginPlay();
	Colorize(BodyMesh, FLinearColor(0.91f, 0.62f, 0.55f));
	Colorize(HeadMesh, FLinearColor(0.95f, 0.72f, 0.66f));
	Colorize(HaloMesh, FLinearColor(1.f, 0.9f, 0.45f));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	Toast(TEXT("Wake at the cottage. Follow the lantern path to Ribbon City."));
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
	}
	if (Z != CurrentZone)
	{
		CurrentZone = Z;
		Toast(FString::Printf(TEXT("Entered %s"), HolypawCatalog::ZoneDisplayName(Z)));
	}
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
			bSkillsOpen = false;
			bPartyOpen = false;
			Mode = EHolypawPawnMode::Play;
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
	Affection->AddAP(Gain);
	Human->ConvertProgress = FMath::Min(100.f, Human->ConvertProgress + Gain);
	if (Human->ConvertProgress >= 100.f)
	{
		Affection->AddMiracle(20.f);
		Toast(FString::Printf(TEXT("%s believes! Miracle Charge surges."), *Human->PersonName.ToString()));
	}
	else
	{
		Toast(FString::Printf(TEXT("+%d AP from a hug."), Gain));
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
	BattleLog = Enemy->DisplayName.ToString() + TEXT(" tries to rip the fluff apart!");
	if (Skills->HasSkill(TEXT("fluffShield")))
	{
		BattleLog += TEXT(" Fluff Shield braces you.");
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
		const float Chance = 0.55f + (Skills->HasSkill(TEXT("haloStep")) ? 0.15f : 0.f);
		if (FMath::FRand() < Chance)
		{
			BattleLog = TEXT("You scampered away!");
			GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EndBattle, 0.7f, false);
		}
		else
		{
			BattleLog = TEXT("Blocked! No escape.");
			GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EnemyBattleSwing, 0.7f, false);
		}
		return;
	}

	int32 Dmg = 0;
	if (Kind == TEXT("slap"))
	{
		Dmg = Attack + FMath::RandRange(0, 4);
		BattleLog = FString::Printf(TEXT("Soft Slap hits for %d!"), Dmg);
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
		const int32 Base = Party->TotalAttack() + 4;
		const float Mult = Skills->HasSkill(TEXT("partyBond")) ? 1.4f : 1.f;
		Dmg = FMath::FloorToInt(Base * Mult) + FMath::RandRange(0, 4);
		BattleLog = Party->Members.Num() > 0
			? FString::Printf(TEXT("Party Assault deals %d!"), Dmg)
			: FString::Printf(TEXT("Lonely swipe for %d. Find fluffies!"), Dmg);
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
		En->Defeat(true);
		BattleLog = TEXT("The hostile pet is unstuffed!");
		GetWorldTimerManager().SetTimer(BattleTimer, this, &AHolypawCharacter::EndBattle, 0.8f, false);
		return;
	}

	int32 Dmg = En->Attack + FMath::RandRange(0, 3);
	if (Skills->HasSkill(TEXT("fluffShield")))
	{
		Dmg = FMath::Max(1, Dmg - 3);
	}
	HP -= Dmg;
	BattleLog = En->DisplayName.ToString() + FString::Printf(TEXT(" shreds stuffing for %d!"), Dmg);
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
	Invuln = 1.8f;
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void AHolypawCharacter::GrantKillRewards()
{
	const int32 ApGain = 10 + FMath::RandRange(0, 7);
	const int32 FpGain = 5 + FMath::RandRange(0, 5);
	Affection->AddAP(ApGain);
	Affection->AddFP(FpGain);
	Affection->AddMiracle(8.f);
	Toast(FString::Printf(TEXT("Pet defeated! +%d AP · +%d FP"), ApGain, FpGain));
}

void AHolypawCharacter::RestFully()
{
	HP = HPMax;
	Toast(TEXT("The cottage mends your stuffing."));
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
	HP = FMath::Min(HPMax, HP + 15);
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
}

void AHolypawCharacter::ToggleSkills()
{
	if (Mode == EHolypawPawnMode::Battle)
	{
		return;
	}
	bSkillsOpen = !bSkillsOpen;
	bPartyOpen = false;
	Mode = bSkillsOpen ? EHolypawPawnMode::UI : EHolypawPawnMode::Play;
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = bSkillsOpen;
		if (bSkillsOpen)
		{
			PC->SetInputMode(FInputModeGameAndUI());
		}
		else
		{
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AHolypawCharacter::ToggleParty()
{
	if (Mode == EHolypawPawnMode::Battle)
	{
		return;
	}
	bPartyOpen = !bPartyOpen;
	bSkillsOpen = false;
	Mode = bPartyOpen ? EHolypawPawnMode::UI : EHolypawPawnMode::Play;
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = bPartyOpen;
		if (bPartyOpen)
		{
			PC->SetInputMode(FInputModeGameAndUI());
		}
		else
		{
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
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

void AHolypawCharacter::BattleSlap()
{
	if (bSkillsOpen)
	{
		if (Skills->GetCatalog().IsValidIndex(0))
		{
			TryBuySkill(Skills->GetCatalog()[0].Id);
		}
		return;
	}
	PlayerBattleAttack(TEXT("slap"));
}

void AHolypawCharacter::BattleBeam()
{
	if (bSkillsOpen)
	{
		if (Skills->GetCatalog().IsValidIndex(1))
		{
			TryBuySkill(Skills->GetCatalog()[1].Id);
		}
		return;
	}
	PlayerBattleAttack(TEXT("beam"));
}

void AHolypawCharacter::BattlePartyAtk()
{
	if (bSkillsOpen)
	{
		if (Skills->GetCatalog().IsValidIndex(2))
		{
			TryBuySkill(Skills->GetCatalog()[2].Id);
		}
		return;
	}
	PlayerBattleAttack(TEXT("party"));
}

void AHolypawCharacter::BattleFlee()
{
	if (bSkillsOpen)
	{
		if (Skills->GetCatalog().IsValidIndex(3))
		{
			TryBuySkill(Skills->GetCatalog()[3].Id);
		}
		return;
	}
	PlayerBattleAttack(TEXT("flee"));
}

void AHolypawCharacter::Skill5()
{
	if (bSkillsOpen && Skills->GetCatalog().IsValidIndex(4))
	{
		TryBuySkill(Skills->GetCatalog()[4].Id);
	}
}

void AHolypawCharacter::Skill6()
{
	if (bSkillsOpen && Skills->GetCatalog().IsValidIndex(5))
	{
		TryBuySkill(Skills->GetCatalog()[5].Id);
	}
}

void AHolypawCharacter::Toast(const FString& Msg)
{
	ToastMsg = Msg;
	ToastTime = 3.2f;
	UE_LOG(LogTemp, Log, TEXT("[Holypaw] %s"), *Msg);
}
