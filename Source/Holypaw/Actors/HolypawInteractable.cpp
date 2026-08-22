#include "Actors/HolypawInteractable.h"
#include "Character/HolypawCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHolypawInteractable::AHolypawInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Mesh->SetGenerateOverlapEvents(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
		Mesh->SetStaticMesh(CubeMesh);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MatFinder.Succeeded())
	{
		ShapeMat = MatFinder.Object;
		Mesh->SetMaterial(0, ShapeMat);
	}
}

bool AHolypawInteractable::Interact(AHolypawCharacter* InstigatorPawn)
{
	return InstigatorPawn != nullptr;
}

void AHolypawInteractable::SetSolidColor(const FLinearColor& Color)
{
	if (!ShapeMat)
	{
		return;
	}
	UMaterialInstanceDynamic* Mid = Mesh->CreateDynamicMaterialInstance(0, ShapeMat);
	if (Mid)
	{
		Mid->SetVectorParameterValue(TEXT("Color"), Color);
	}
}
