#pragma once

#include "CoreMinimal.h"
#include "Actors/HolypawInteractable.h"
#include "HolypawTypes.h"
#include "HostilePet.generated.h"

UCLASS()
class HOLYPAW_API AHostilePet : public AHolypawInteractable
{
	GENERATED_BODY()

public:
	AHostilePet();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	EHolypawVillain VillainId = EHolypawVillain::ScrapDog;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 HP = 28;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 HPMax = 28;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 Attack = 7;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	EVillainRank Rank = EVillainRank::Minion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	EVillainSpecial Special = EVillainSpecial::Rip;

	UPROPERTY(BlueprintReadOnly, Category = "Holypaw")
	bool bDefeated = false;

	UPROPERTY(EditAnywhere, Category = "Holypaw")
	float AggroRange = 900.f;

	UPROPERTY(EditAnywhere, Category = "Holypaw")
	bool bBlocksFlee = false;

	UPROPERTY(EditAnywhere, Category = "Holypaw")
	float RespawnSeconds = 25.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> AccentMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> AccentMeshB;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CrestMesh;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FText GetPrompt() const override;
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn) override;

	void Configure(EHolypawVillain Id);
	FVillainDef GetDef() const;
	bool IsBoss() const { return Rank == EVillainRank::Boss || Rank == EVillainRank::WorldBoss; }

	void Defeat(bool bGrantRewards);
	void RespawnLater();

	void PulseHit();
	bool TryEnterPhaseTwo();
	FString GetPhaseLine() const;
	float HitPulse = 0.f;
	FVector Home = FVector::ZeroVector;
	float BodyScale = 1.f;
	bool bPhaseTwo = false;
	int32 BaseAttack = 7;

private:
	void ApplyFromCatalog();
	void ApplySignatureSilhouette(const FVillainDef& D);
	void ColorPart(UStaticMeshComponent* Comp, const FLinearColor& Color);
	UStaticMesh* MeshForShape(EVillainShape Shape) const;

	FTimerHandle RespawnTimer;
	FVector WanderVel = FVector::ZeroVector;

	UStaticMesh* SphereMesh = nullptr;
	UStaticMesh* ConeMesh = nullptr;
	UStaticMesh* CylMesh = nullptr;
};
