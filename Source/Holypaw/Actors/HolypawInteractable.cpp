#include "Actors/HolypawInteractable.h"
#include "Character/HolypawCharacter.h"
#include "Look/HolypawLook.h"
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
	Mesh->SetCastShadow(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeFinder(TEXT("/Engine/BasicShapes/Cone.Cone"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
		Mesh->SetStaticMesh(CubeMesh);
	}
	if (SphereFinder.Succeeded()) { SphereMesh = SphereFinder.Object; }
	if (ConeFinder.Succeeded()) { ConeMesh = ConeFinder.Object; }
	if (CylFinder.Succeeded()) { CylMesh = CylFinder.Object; }
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
	HolypawLook::Paint(Mesh, Color);
}
