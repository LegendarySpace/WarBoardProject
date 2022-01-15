// Fill out your copyright notice in the Description page of Project Settings.


#include "TileTypeManager.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"

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

	// Determine Base Tile (Used when no mesh is provided)
	UStaticMesh* mesh;
	switch (GetTileShape())
	{
	case ETileShape::Square:
		// Use Square tile
		mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/WarBoard/StaticMesh/SquareTile.SquareTile'")).Object;
		break;
	case ETileShape::Hex_Hor:
		// Use Horizontal Hex tile
		mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/WarBoard/StaticMesh/HexTile_Hori.HexTile_Hori'")).Object;
		break;
	case ETileShape::Hex_Vert:
		// Use Vertical Hex tile
		mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/WarBoard/StaticMesh/HexTile_Vert.HexTile_Vert'")).Object;
		break;
	case ETileShape::Triangle:
	default:
		mesh = nullptr;
		break;
	}
	UMaterial* mat = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("StaticMesh'/WarBoard/Material/TileMat.TileMat'")).Object;

	InstancedMeshComp->SetStaticMesh(mesh);
	InstancedMeshComp->SetMaterial(0, mat);
}

void ATileTypeManager::SetupInstance(ETileType TileType, UStaticMesh * Mesh, UMaterial* Mat)
{
	if (InstancedMeshComp == nullptr) return;

	Type = TileType;
	if (Mesh != nullptr) InstancedMeshComp->SetStaticMesh(Mesh);
	if (Mat != nullptr) InstancedMeshComp->SetMaterial(0, Mat);
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

void ATileTypeManager::AddTile(FTile Tile)
{
	if (!InstanceIndexes.Contains(Tile.ToIndex()))
	{
		InstanceIndexes.AddUnique(Tile.ToIndex());

		BuildTile(Tile.ToIndex());
	}

}

void ATileTypeManager::BuildTile(FTile Tile)
{
	if (!InstancedMeshComp) return;
	FVector loc = Tile.ToWorld();
	// Tiles are assumed to have a thickness 1/10th its TileSize
	loc += FVector(0, 0, -0.05 * WarBoardLib::GetTileSize());
	FVector scale = FVector(WarBoardLib::GetTileSize() / MeshSize);
	InstancedMeshComp->AddInstance(FTransform(FRotator(0), loc, scale));
}

void ATileTypeManager::RemoveTile(FTile TileToRemove)
{
	UE_LOG(LogTemp, Warning, TEXT("Remove tile called at %d"), TileToRemove.ToIndex());
	if (InstanceIndexes.Contains(TileToRemove.ToIndex()) && InstancedMeshComp != nullptr)
	{
		int32 InstanceIndex = InstanceIndexes.Find(TileToRemove.ToIndex());
		if (InstanceIndex == INDEX_NONE) return;

		// TODO: Removable once debugging is finished
		FTransform InstanceTransform;
		FTile InstanceTile;

		if (InstancedMeshComp->GetInstanceTransform(InstanceIndex, InstanceTransform, true))
		{
			InstanceTile = InstanceTransform.GetLocation();
			UE_LOG(LogTemp, Warning, TEXT("Attempting to remove tile at Index %d"), InstanceTile.ToIndex());
		}

		if (InstancedMeshComp->RemoveInstance(InstanceIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("Tile at Index %d Successfully Removed"), InstanceTile.ToIndex());
			
			InstanceIndexes.RemoveAt(InstanceIndex, 1, true);
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
	FTile InstanceTile;
	for (int InstanceBodiesIndex = 0; InstanceBodiesIndex < insta.Num(); InstanceBodiesIndex++)
	{
		if (InstancedMeshComp->GetInstanceTransform(InstanceBodiesIndex, InstanceTransform, true))
		{
			InstanceTile = InstanceTransform.GetLocation();
			UE_LOG(LogTemp, Warning, TEXT("Instance %d is at index %d"), InstanceBodiesIndex, InstanceTile.ToIndex());
		}
	}
}

// Called when the game starts or when spawned
void ATileTypeManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// TODO: On Tick if debug mode call show on screen debug and display Index

