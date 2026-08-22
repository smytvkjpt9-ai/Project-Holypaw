#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HolypawInteractable.generated.h"

class UStaticMesh;
class UMaterialInterface;

UCLASS()
class HOLYPAW_API AHolypawInteractable : public AActor
{
	GENERATED_BODY()

public:
	AHolypawInteractable();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	virtual FText GetPrompt() const { return FText::FromString(TEXT("Interact")); }

	UFUNCTION(BlueprintCallable, Category = "Holypaw")
	virtual bool Interact(class AHolypawCharacter* InstigatorPawn);

	void SetSolidColor(const FLinearColor& Color);

protected:
	UStaticMesh* CubeMesh = nullptr;
	UStaticMesh* SphereMesh = nullptr;
	UStaticMesh* ConeMesh = nullptr;
	UStaticMesh* CylMesh = nullptr;
	UMaterialInterface* ShapeMat = nullptr;
};
