// Fill out your copyright notice in the Description page of Project Settings.


#include "LayoutManager.h"

#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"
#include <string.h>

#include "TileTypeManager.h"
#include "WarBoardLibrary.h"

// Sets default values
ALayoutManager::ALayoutManager()
{
 	// Should never tick
	PrimaryActorTick.bCanEverTick = false;

	int h = 2;
	int w = 2;

	for (int32 i = -h; i <= h; i++)
	{
		for (int32 j = -w; j <= w; j++)
		{
			BoardDefault.AddUnique(FTileInstance(FGCoord(i, j)));
		}
	}

	BuildTileManagerArray();

	// Initialize Board
	AssembleTiles();
	// TODO: AssembleGrid();
}

bool ALayoutManager::ChangeTile(FTileInstance Instance)
{
	// Remove Index from existing Manager
	// Add Index to Manager at location Type
	// Add Index to TileMap

	// Use TileMap to look up TileType of Index, TileType is == to position within Managers
	if (TileMap.Contains(Instance)) Managers[Instance.Type]->RemoveTile(Instance.Coord);
	else OnTileAdd.Broadcast(Instance.Coord);
	Managers[Instance.Type]->AddTile(Instance.Coord);
	TileMap.Add(Instance);
	OnTileChange.Broadcast(Instance.Coord, Instance.Type);
	return true;
}

bool ALayoutManager::RemoveTile(FGCoord Coord)
{
	// Remove Index from existing Manager then remove Index from TileMap
	auto pInstance = TileMap.FindByPredicate([=](FTileInstance& Instance) { return Instance.Coord == Coord; });
	if (pInstance)
	{
		Managers[(*pInstance).Type]->RemoveTile(Coord);
		TileMap.Remove(*pInstance);
		OnTileRemove.Broadcast(Coord);
	}
	return true;
}

void ALayoutManager::AssembleTiles()
{
	// Iterate over BoardDefault, ChangeTile on each index
	for (auto& Instance : BoardDefault)
	{
		ChangeTile(Instance);

	}
}

TArray<FGCoord> ALayoutManager::GetTiles()
{
	TArray<FGCoord> Tiles;
	for (auto& Instance : TileMap) Tiles.Add(Instance.Coord);
	return Tiles;
}

void ALayoutManager::BuildTileManagerArray()
{
	Managers.Empty();
	// Iterate over each TileType and initialize a Manager for it
	for (auto type = ETileType::TT_Normal; type != ETileType::TT_Type_MAX; type = ETileType(type + 1))
	{
		// For each Tile Type create a default subobject to handle those tiles
		auto m = CreateDefaultSubobject<ATileTypeManager>(TEXT("Manager" + static_cast<uint8>(type)));
		m->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		Managers.Add(m);

		// Should BaseMesh be used?
		UStaticMesh* mesh = nullptr;
		UMaterial* mat = nullptr;
		if (TileMeshes.Contains(type))
		{
			FTileSetup* setup = TileMeshes.FindByPredicate([=](auto& TileSetup) { return TileSetup == type; });
			mesh = setup->Mesh;
			mat = setup->Mat;
		}

		m->SetupInstance(type, mesh, mat);
	}

}

// Called when the game starts or when spawned
void ALayoutManager::BeginPlay()
{
	Super::BeginPlay();
	
}

