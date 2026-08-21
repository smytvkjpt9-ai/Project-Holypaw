#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AffectionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHolypawStatsChanged);

UCLASS(ClassGroup = (Holypaw), meta = (BlueprintSpawnableComponent))
class HOLYPAW_API UAffectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAffectionComponent();

	UPROPERTY(BlueprintAssignable)
	FOnHolypawStatsChanged OnStatsChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 AP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 FP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	float MiracleCharge = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	float MiracleMax = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Holypaw")
	int32 Level = 1;

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	void AddAP(int32 Amount, bool bFillMiracle = true);

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	void AddFP(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	bool SpendAP(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	bool SpendFP(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	bool IsMiracleReady() const { return MiracleCharge >= MiracleMax; }

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	void ResetMiracleCharge();

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	void AddMiracle(float Amount);

protected:
	void Broadcast() const;
};
