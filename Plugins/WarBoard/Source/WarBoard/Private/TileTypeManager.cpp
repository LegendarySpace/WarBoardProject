// Fill out your copyright notice in the Description page of Project Settings.


#include "TileTypeManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "WarBoardLibrary.h"
using namespace WarBoardLib;

// Sets default values
ATileTypeManager::ATileTypeManager()
{
 	// Never tick
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	HISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HISM"));
	HISM->AttachTo(RootComponent);
}

void ATileTypeManager::SetupInstance(ETileType TileType, UStaticMesh * Mesh, float TileSize)
{
	Type = TileType;
	Size = TileSize;
	HISM->SetStaticMesh(Mesh);
}

void ATileTypeManager::SetMesh(UStaticMesh * Mesh)
{
	HISM->SetStaticMesh(Mesh);
}

void ATileTypeManager::SetMat(UMaterial * Mat)
{
	HISM->SetMaterial(0, Mat);
}

void ATileTypeManager::Add(int32 Index)
{
	if (!IDs.Contains(Index))
	{
		IDs.AddUnique(Index);

		FVector loc;
		loc = IndexToWorld(Index);
		// Tiles are assumed to have a thickness 1/10th its TileSize
		loc += FVector(0.f, 0.f, -0.05 * GetTileSize());
		// Tile Mesh's longest dimension is assumed to be 100
		HISM->AddInstance(FTransform(FRotator(0.f), FVector(loc), FVector(GetTileSize()/100.f)));
	}
}

void ATileTypeManager::Remove(int32 Index)
{
	if (IDs.Contains(Index))
	{
		int32 i = IDs.Find(Index);
		IDs.Swap(i, IDs.Num()-1);
		IDs.Remove(IDs.Num()-1);

		HISM->RemoveInstance(i);
	}
}

// Called when the game starts or when spawned
void ATileTypeManager::BeginPlay()
{
	Super::BeginPlay();
	
}

