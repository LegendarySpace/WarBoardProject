// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanarManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "WarBoardLibrary.h"

// Sets default values
APlanarManager::APlanarManager()
{
 	// Never tick
	PrimaryActorTick.bCanEverTick = false;

	// Setup defaults
	HISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Planes"));

}

void APlanarManager::SetPlaneType(ETileShape Shape)
{
	// Determine Base Tile
	UStaticMesh* mesh;
	switch (Shape)
	{
	case ETileShape::Square:
		mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/WarBoard/StaticMesh/SquarePlane.SquarePlane'")).Object;
		break;
	case ETileShape::Hex_Hor:
		mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/WarBoard/StaticMesh/HexPlane_Hori.HexPlane_Hori'")).Object;
		break;
	case ETileShape::Hex_Vert:
		mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/WarBoard/StaticMesh/HexPlane_Vert.HexPlane_Vert'")).Object;
		break;
	default:
		mesh = nullptr;
		break;
	}
	HISM->SetStaticMesh(mesh);

}

void APlanarManager::Populate(TArray<FTile> Choices)
{
	Populate_Implementation(Choices);
}

void APlanarManager::Populate_Implementation(TArray<FTile> Choices)
{
	Clear();
	FVector sca = FVector(WarBoardLib::GetTileSize() / 100.f);
	// Override in children
	for (auto Tile : Choices)
	{
		HISM->AddInstance(FTransform(FRotator(0.0), Tile.ToWorld(), sca));
	}
}

void APlanarManager::Clear()
{
	HISM->ClearInstances();
}

// Called when the game starts or when spawned
void APlanarManager::BeginPlay()
{
	Super::BeginPlay();
	
}

