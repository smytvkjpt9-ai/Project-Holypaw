#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HolypawTypes.h"
#include "HolypawCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAffectionComponent;
class USkillTreeComponent;
class UPartyComponent;
class UHolypawMissionComponent;
class UPointLightComponent;
class USceneComponent;
class AWildFluffy;
class AHugHuman;
class AHostilePet;
enum class EHolypawShrineKind : uint8;

USTRUCT()
struct FHolypawFollowerVisual
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> EarL;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> EarR;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Tail;
};

UENUM()
enum class EHolypawPawnMode : uint8
{
	Title,
	Play,
	Battle,
	UI,
	Pause
};

UCLASS()
class HOLYPAW_API AHolypawCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHolypawCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> HeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> HaloMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> EarL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> EarR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Snout;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> PawL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> PawR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Belly;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> EyeL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> EyeR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> InnerEarL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> InnerEarR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Nose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> HighlightL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> HighlightR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheekL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheekR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ArmL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ArmR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LegL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LegR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> FootL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> FootR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Ribbon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> BrowL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> BrowR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Tail;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> BowL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> BowR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPointLightComponent> HaloLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAffectionComponent> Affection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkillTreeComponent> Skills;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPartyComponent> Party;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHolypawMissionComponent> Story;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 HP = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 HPMax = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 Attack = 8;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	float WalkSpeed = 700.f;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	EHolypawPawnMode Mode = EHolypawPawnMode::Title;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	EHolypawZone CurrentZone = EHolypawZone::ForestCottage;

	UFUNCTION(BlueprintCallable)
	bool IsBusy() const { return Mode != EHolypawPawnMode::Play; }

	UFUNCTION(BlueprintCallable)
	float GetInvuln() const { return Invuln; }

	bool RecruitFluffy(AWildFluffy* Fluffy);
	bool HugPerson(AHugHuman* Human);
	void StartBattle(AHostilePet* Enemy);
	void GrantKillRewards(AHostilePet* Fallen);
	void RestFully();
	bool BuyFaith(int32 ApCost, int32 FpGain);
	bool UseShrine(EHolypawShrineKind Kind);
	FString GetBattleStatusLine() const;
	void TryMiracle();
	void CompleteBearFaith();
	void ApplySkillEffects(FName Id);
	bool IsGuarding() const { return bGuarding; }
	void Toast(const FString& Msg);
	FString GetToast() const { return ToastMsg; }
	float GetToastAlpha() const { return ToastTime > 0.f ? 1.f : 0.f; }
	FText GetPrompt() const { return Prompt; }

	void ResetForNewGame();
	const TArray<EHolypawVillain>& GetSeenVillains() const { return SeenVillains; }
	const TArray<EHolypawVillain>& GetDefeatedVillains() const { return DefeatedVillains; }
	void SetCodex(const TArray<EHolypawVillain>& Seen, const TArray<EHolypawVillain>& Defeated);
	const TArray<FHolypawHeartRecord>& GetHeartRecords() const { return CityHearts; }
	void SetHeartRecords(const TArray<FHolypawHeartRecord>& Records);
	const TArray<EHolypawZone>& GetUnlockedTravel() const { return UnlockedTravel; }
	void SetUnlockedTravel(const TArray<EHolypawZone>& Zones);
	void UnlockTravel(EHolypawZone Zone);
	int32 GetCityHearts(EHolypawZone Zone) const;
	void AddCityHeart(EHolypawZone Zone, int32 Amount = 1);
	const TArray<FHolypawItemStack>& GetInventory() const { return Inventory; }
	void SetInventory(const TArray<FHolypawItemStack>& Stacks);
	int32 GetItemCount(FName Id) const;
	void AddItem(FName Id, int32 Amount = 1);
	bool ConsumeItem(FName Id, int32 Amount = 1);
	bool UseItem(FName Id);
	void OpenShop();
	void BuyShopSlot(int32 Index);
	bool StartTalk(AHugHuman* Human);
	void AdvanceTalk();
	void AskTalkHint();
	void TurnInErrand();
	void AcceptQuest();
	void SetQuestState(const TArray<FName>& Active, const TArray<FName>& Done);
	const TArray<FName>& GetQuestActive() const { return QuestActive; }
	const TArray<FName>& GetQuestDone() const { return QuestDone; }
	void ToggleInventory();
	bool IsTalkOpen() const { return bTalkOpen; }
	bool IsShopOpen() const { return bShopOpen; }
	bool IsInventoryOpen() const { return bInventoryOpen; }
	TArray<FString> GetTalkLines() const;
	TArray<FString> GetShopLines() const;
	TArray<FString> GetInventoryLines() const;
	int32 GetLastDamageDealt() const { return LastDamageDealt; }
	float GetDamagePopupTime() const { return DamagePopupTime; }
	FString GetClockLine() const;
	int32 ShopPrice(int32 BaseCost) const;
	void OpenFastTravel(EHolypawZone FromZone);
	void FastTravelToSelected();
	void CycleTravel(int32 Delta);
	EHolypawZone GetSelectedTravel() const;
	TArray<FString> GetTravelLines() const;

	void TitleSelectSlot(int32 Index);
	void TitleConfirm();
	void TitleNewGame();
	void TitleLoad();
	void QuickSave(bool bAnnounce = true);
	void TogglePauseMenu();
	void ReturnToTitle();
	void CycleMute();
	virtual void Jump() override;

	void PlayerBattleAttack(FName Kind);
	UFUNCTION()
	void EndBattle();
	UFUNCTION()
	void EnemyBattleSwing();
	UFUNCTION()
	void ResumePlayerTurn();
	UFUNCTION()
	void FailAndWakeAtCottage();
	AHostilePet* GetBattleEnemy() const { return BattleEnemy.Get(); }
	FString GetBattleLog() const { return BattleLog; }
	bool IsBattleBusy() const { return bBattleBusy; }
	int32 GetBattlePage() const { return BattlePage; }
	FName BattleCommandId(int32 Slot) const;

	void ToggleSkills();
	void ToggleParty();
	void ToggleMap();
	void ToggleCodex();
	void ToggleJournal();
	void ClosePanels();
	bool IsSkillsOpen() const { return bSkillsOpen; }
	bool IsPartyOpen() const { return bPartyOpen; }
	bool IsMapOpen() const { return bMapOpen; }
	bool IsCodexOpen() const { return bCodexOpen; }
	bool IsJournalOpen() const { return bJournalOpen; }
	TArray<FString> GetJournalLines() const;
	FString GetCompassLine() const { return CompassLine; }
	TArray<FString> GetMapLines() const;
	TArray<FString> GetCodexLines() const;
	int32 GetCodexSeenCount() const { return SeenVillains.Num(); }
	int32 GetCodexDefeatedCount() const { return DefeatedVillains.Num(); }
	int32 GetCodexTotal() const;
	bool HasSeenVillain(EHolypawVillain Id) const { return SeenVillains.Contains(Id); }
	bool HasDefeatedVillain(EHolypawVillain Id) const { return DefeatedVillains.Contains(Id); }
	void TryBuySkill(FName Id);

	UFUNCTION()
	void BattleSlap();
	UFUNCTION()
	void BattleBeam();
	UFUNCTION()
	void BattlePartyAtk();
	UFUNCTION()
	void BattleFlee();
	UFUNCTION()
	void Skill5();
	UFUNCTION()
	void Skill6();
	UFUNCTION()
	void CloseOrJump();
	UFUNCTION()
	void CycleSkillTree();
	UFUNCTION()
	void TitleConfirmPressed();
	UFUNCTION()
	void TitleLoadPressed();
	UFUNCTION()
	void QuickSavePressed();
	UFUNCTION()
	void MutePressed();
	UFUNCTION()
	void TitleMenuPressed();
	UFUNCTION()
	void InventoryPressed();

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);
	void Interact();

	void UpdateZone();
	AActor* FindNearestInteractable(float Range) const;
	void Colorize(UStaticMeshComponent* Comp, const FLinearColor& Color);
	void SetPanel(bool& Flag);
	void TryBuyTreeSlot(int32 Index);
	void ApplyTalkVerb(uint8 Verb);

	UPROPERTY()
	TObjectPtr<AHostilePet> BattleEnemy;

	FString BattleLog;
	bool bBattleBusy = false;
	bool bPlayerTurn = true;
	int32 BattleTurn = 0;
	bool bPartyCut = false;
	bool bGuarding = false;
	int32 SlapCombo = 0;
	int32 PoisonTurns = 0;
	int32 FrostTurns = 0;
	int32 HymnShield = 0;
	bool bEnemyStaggered = false;
	int32 BattlePage = 0;
	int32 EnemyRipTurns = 0;
	int32 MillTurns = 0;
	bool bSeamBrace = false;
	float ExploreArm = 560.f;
	float BattleArm = 270.f;
	FTimerHandle BattleTimer;

	float Invuln = 0.f;
	float HugLock = 0.f;
	FString ToastMsg;
	float ToastTime = 0.f;
	FText Prompt;
	bool bSkillsOpen = false;
	bool bPartyOpen = false;
	bool bMapOpen = false;
	bool bCodexOpen = false;
	bool bJournalOpen = false;
	FString CompassLine;

	UPROPERTY()
	TArray<EHolypawVillain> SeenVillains;

	UPROPERTY()
	TArray<EHolypawVillain> DefeatedVillains;

	UPROPERTY()
	TArray<FHolypawHeartRecord> CityHearts;

	UPROPERTY()
	TArray<EHolypawZone> UnlockedTravel;

	int32 TravelCursor = 0;
	bool bWantsFastTravel = false;
	bool bTalkOpen = false;
	bool bShopOpen = false;
	bool bInventoryOpen = false;
	FString TalkSpeaker;
	FString TalkBody;
	FString TalkHint;
	bool bTalkSecond = false;
	bool bTalkThird = false;
	int32 LastDamageDealt = 0;
	int32 LastDamageTaken = 0;
	float DamagePopupTime = 0.f;
	float AnimT = 0.f;
	float BlinkT = 2.4f;
	float HurtPulse = 0.f;
	float HugAnim = 0.f;
	FVector BodyBase = FVector::ZeroVector;
	FVector HeadBase = FVector::ZeroVector;
	FVector EarLBase = FVector::ZeroVector;
	FVector EarRBase = FVector::ZeroVector;
	FVector PawLBase = FVector::ZeroVector;
	FVector PawRBase = FVector::ZeroVector;
	FVector ArmLBase = FVector::ZeroVector;
	FVector ArmRBase = FVector::ZeroVector;
	FVector LegLBase = FVector::ZeroVector;
	FVector LegRBase = FVector::ZeroVector;
	FVector EyeLScale = FVector::OneVector;
	FVector EyeRScale = FVector::OneVector;
	FVector HighlightLScale = FVector::OneVector;
	FVector HighlightRScale = FVector::OneVector;
	FRotator EarLRot = FRotator::ZeroRotator;
	FRotator EarRRot = FRotator::ZeroRotator;
	FRotator PawLRot = FRotator::ZeroRotator;
	FRotator PawRRot = FRotator::ZeroRotator;
	FRotator ArmLRot = FRotator::ZeroRotator;
	FRotator ArmRRot = FRotator::ZeroRotator;
	FRotator LegLRot = FRotator::ZeroRotator;
	FRotator LegRRot = FRotator::ZeroRotator;

	UPROPERTY()
	TArray<FHolypawItemStack> Inventory;

	UPROPERTY()
	TArray<FName> QuestActive;

	UPROPERTY()
	TArray<FName> QuestDone;

	void TickProcAnim(float DeltaSeconds);
	void PlayCue(FName Cue);

	UMaterialInterface* ShapeMat = nullptr;

	void SyncFollowers(float DeltaSeconds);
	UStaticMeshComponent* MakeFollowerPart(UStaticMesh* Mesh, const FLinearColor& Color, USceneComponent* Attach = nullptr);

	UPROPERTY()
	TArray<FHolypawFollowerVisual> Followers;

	TArray<FVector> Trail;
	UStaticMesh* SphereMesh = nullptr;
	UStaticMesh* ConeMesh = nullptr;
};
