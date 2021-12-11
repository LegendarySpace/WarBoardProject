// Fill out your copyright notice in the Description page of Project Settings.


#include "TileTypeManager.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/TextRenderComponent.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;

// Sets default values
ATileTypeManager::ATileTypeManager()
{
 	// Never tick
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	InstancedMeshComp = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMeshComp"));
	InstancedMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ATileTypeManager::SetupInstance(ETileType TileType, UStaticMesh * Mesh, float TileSize)
{
	if (InstancedMeshComp == nullptr) return;

	Type = TileType;
	Size = TileSize;
	InstancedMeshComp->SetStaticMesh(Mesh);
}

void ATileTypeManager::SetMesh(UStaticMesh * Mesh)
{
	if (InstancedMeshComp == nullptr) return;
	InstancedMeshComp->SetStaticMesh(Mesh);
}

void ATileTypeManager::SetMat(UMaterial * Mat)
{
	if (InstancedMeshComp == nullptr) return;
	InstancedMeshComp->SetMaterial(0, Mat);
}

void ATileTypeManager::AddTile(int32 Index)
{
	if (!InstanceIndexes.Contains(Index))
	{
		InstanceIndexes.AddUnique(Index);

		BuildTile(Index);
	}

}

void ATileTypeManager::BuildTile(int32 Index)
{
	if (InstancedMeshComp == nullptr) return;
	FVector loc;
	loc = IndexToWorld(Index);
	// Tiles are assumed to have a thickness 1/10th its TileSize
	loc += FVector(0.f, 0.f, -0.05 * Size);
	// Tile Mesh's longest dimension is assumed to be 100
	InstancedMeshComp->AddInstance(FTransform(FRotator(0.f), FVector(loc), FVector(Size / MeshSize)));
}

void ATileTypeManager::RemoveTile(int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("Remove tile called at %d"), Index);
	if (InstanceIndexes.Contains(Index) && InstancedMeshComp != nullptr)
	{
		int32 i = InstanceIndexes.Find(Index);
		if (i == INDEX_NONE) return;

		FTransform InstanceTransform;
		int32 InstanceTile;


		if (InstancedMeshComp->GetInstanceTransform(i, InstanceTransform, true))
		{
			WorldToIndex(InstanceTransform.GetLocation(), InstanceTile);
			UE_LOG(LogTemp, Warning, TEXT("Attempting to remove tile at Index %d"), InstanceTile);
		}

		if (InstancedMeshComp->RemoveInstance(i))
		{
			UE_LOG(LogTemp, Warning, TEXT("Tile at Index %d Successfully Removed"), InstanceTile);
			
			InstanceIndexes.RemoveAt(i, 1, true);
			InstanceIndexes.Shrink();
		}
		else UE_LOG(LogTemp, Warning, TEXT("Remove instance failed"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("Remove tile failed: Tile not in InstanceIndexes"));
}

void ATileTypeManager::DisplayInstanceIndexes()
{
	UE_LOG(LogTemp, Warning, TEXT("     Instance Indexes     "));
	auto &insta = InstancedMeshComp->InstanceBodies;

	FTransform InstanceTransform;
	int32 InstanceTile;
	for (int InstanceBodiesIndex = 0; InstanceBodiesIndex < insta.Num(); InstanceBodiesIndex++)
	{
		if (InstancedMeshComp->GetInstanceTransform(InstanceBodiesIndex, InstanceTransform, true))
		{
			WorldToIndex(InstanceTransform.GetLocation(), InstanceTile);
			UE_LOG(LogTemp, Warning, TEXT("Instance %d is at index %d"), InstanceBodiesIndex, InstanceTile);
		}
	}
}

// Called when the game starts or when spawned
void ATileTypeManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// TODO: On Tick if debug mode call show on screen debug and display Index

