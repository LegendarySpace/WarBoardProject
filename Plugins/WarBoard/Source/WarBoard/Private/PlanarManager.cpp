// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanarManager.h"

#include "Components/InstancedStaticMeshComponent.h"

#include "HelperStructs.h"
#include "WarBoardLibrary.h"

// Sets default values
APlanarManager::APlanarManager()
{
 	// Never tick
	PrimaryActorTick.bCanEverTick = false;

	// Setup defaults
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Planes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Planes"));
	Planes->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	if (GetWorld()) Planes->RegisterComponent();

	PlaneMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/WarBoard/Material/TileMat.TileMat'")).Object;

}

void APlanarManager::SetPlaneType(ETileShape Shape)
{
	// Determine Base Tile
	UStaticMesh* mesh;
	switch (Shape)
	{
	case ETileShape::Square:
		mesh = LoadObject<UStaticMesh>(NULL, TEXT("/WarBoard/StaticMesh/SquarePlane.SquarePlane"), NULL, LOAD_None, NULL);
		break;
	case ETileShape::Hex_Hor:
		mesh = LoadObject<UStaticMesh>(NULL, TEXT("/WarBoard/StaticMesh/HexPlane_Hori.HexPlane_Hori"), NULL, LOAD_None, NULL);
		break; 
	case ETileShape::Hex_Vert:
		mesh = LoadObject<UStaticMesh>(NULL, TEXT("/WarBoard/StaticMesh/HexPlane_Vert.HexPlane_Vert"), NULL, LOAD_None, NULL);
		break; 
	case ETileShape::Triangle:
	default:
		mesh = nullptr;
		break;
	}
	Planes->SetStaticMesh(mesh);
	if (PlaneMaterial) Planes->SetMaterial(0, PlaneMaterial);
}

void APlanarManager::Populate(TArray<FTile> Choices)
{
	Clear();
	// TODO: Add optional padding
	float size = WarBoardLib::GetTileSize();
	if (Padding >= 1) size -= Padding;
	else if (Padding > 0 && Padding < 1) size -= size * Padding;
	Scale = FVector(size / 100.f);

	Populate_Implementation(Choices);
}

void APlanarManager::Populate_Implementation(TArray<FTile> Choices)
{
	for (auto Tile : Choices)
	{
		Planes->AddInstance(FTransform(FRotator(0.0), Tile.ToWorld(), Scale));
	}
}

void APlanarManager::Clear()
{
	Planes->ClearInstances();
}

// Called when the game starts or when spawned
void APlanarManager::BeginPlay()
{
	Super::BeginPlay();
	
	SetPlaneType(WarBoardLib::GetTileShape());
}

