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
class AWildFluffy;
class AHugHuman;
class AHostilePet;

UENUM()
enum class EHolypawPawnMode : uint8
{
	Play,
	Battle,
	UI
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
	TObjectPtr<UAffectionComponent> Affection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkillTreeComponent> Skills;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPartyComponent> Party;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 HP = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 HPMax = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 Attack = 8;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	float WalkSpeed = 520.f;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	EHolypawPawnMode Mode = EHolypawPawnMode::Play;

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
	void TryMiracle();
	void ApplySkillEffects(FName Id);
	void Toast(const FString& Msg);
	FString GetToast() const { return ToastMsg; }
	float GetToastAlpha() const { return ToastTime > 0.f ? 1.f : 0.f; }
	FText GetPrompt() const { return Prompt; }

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

	void ToggleSkills();
	void ToggleParty();
	void ToggleMap();
	void ToggleCodex();
	void ClosePanels();
	bool IsSkillsOpen() const { return bSkillsOpen; }
	bool IsPartyOpen() const { return bPartyOpen; }
	bool IsMapOpen() const { return bMapOpen; }
	bool IsCodexOpen() const { return bCodexOpen; }
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

	UPROPERTY()
	TObjectPtr<AHostilePet> BattleEnemy;

	FString BattleLog;
	bool bBattleBusy = false;
	bool bPlayerTurn = true;
	int32 BattleTurn = 0;
	bool bPartyCut = false;
	FTimerHandle BattleTimer;

	float Invuln = 0.f;
	FString ToastMsg;
	float ToastTime = 0.f;
	FText Prompt;
	bool bSkillsOpen = false;
	bool bPartyOpen = false;
	bool bMapOpen = false;
	bool bCodexOpen = false;
	FString CompassLine;

	UPROPERTY()
	TArray<EHolypawVillain> SeenVillains;

	UPROPERTY()
	TArray<EHolypawVillain> DefeatedVillains;

	UMaterialInterface* ShapeMat = nullptr;

	void SyncFollowers(float DeltaSeconds);

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> FollowerMeshes;

	TArray<FVector> Trail;
	UStaticMesh* SphereMesh = nullptr;
};
