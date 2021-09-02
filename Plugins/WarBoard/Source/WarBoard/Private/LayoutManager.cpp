// Fill out your copyright notice in the Description page of Project Settings.


#include "LayoutManager.h"
#include "TileTypeManager.h"
#include "WarBoardLibrary.h"
#include "Components/TextRenderComponent.h"
#include <string.h>

// Sets default values
ALayoutManager::ALayoutManager()
{
 	// Should never tick
	PrimaryActorTick.bCanEverTick = false;

	// Determine Base Tile (Used when no mesh is provided)
	if (HexTiles)
	{
		if (HexVert)
		{
			// Use Vertical Hex tile
			auto finder = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/WarBoard/StaticMesh/HexTile_Hori.HexTile_Hori'"));
			if (finder.Object) BaseMesh = finder.Object;
		}
		else
		{
			// Use Horizontal Hex tile
			auto finder = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/WarBoard/StaticMesh/HexTile_Hori.HexTile_Hori'"));
			if (finder.Object) BaseMesh = finder.Object;
		}
	}
	else
	{
		// Use Square tile
		auto finder = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/WarBoard/StaticMesh/HexTile_Hori.HexTile_Hori'"));
		if (finder.Object) BaseMesh = finder.Object;
	}

	// Iterate over each TileType and initialize a Manager for it
	TArray<ETileType> k;
	TileMeshes.GetKeys(k);
	for (auto type = ETileType::TT_Normal; type != ETileType::TT_Type_MAX; type = ETileType(type + 1))
	{
		// For each Tile Type create a default subobject to handle those tiles
		auto m = CreateDefaultSubobject<ATileTypeManager>(TEXT("Manager" + static_cast<int>(type)));
		m->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		Managers.Add(m);

		// Should BaseMesh be used?
		auto mesh = BaseMesh;
		if (k.Contains(type)) mesh = *(TileMeshes.Find(type));
		
		m->SetupInstance(type, BaseMesh, TileSize);
	}

	// Initialize Board
	AssembleTiles();
	AssembleGrid();
}

int32 ALayoutManager::GetOffset()
{
	return int32();
}

FVector ALayoutManager::GetBoardSize()
{
	return FVector();
}

bool ALayoutManager::ChangeTile(int32 Index, ETileType Type)
{
	// Remove Index from existing Manager
	// Add Index to Manager at location Type
	// Add Index to TileMap

	// Use TileMap to look up TileType of Index, TileType is == to position within Managers
	if (TileMap.Contains(Index)) Managers[TileMap[Index]]->Remove(Index);
	else { /** TODO Handle addition of path node if pathfinder exists **/ }
	Managers[Type]->Add(Index);
	TileMap.Add(Index, Type);
	return true;
}

bool ALayoutManager::RemoveTile(int32 Index)
{
	// Remove Index from existing Manager then remove Index from TileMap
	if (TileMap.Contains(Index))
	{
		Managers[TileMap[Index]]->Remove(Index);
		TileMap.Remove(Index);
		/** TODO Handle removal of path node if pathfinder exists **/
	}
	return true;
}

void ALayoutManager::AssembleTiles()
{
	// Iterate over BoardDefault, ChangeTile on each index
	TArray<int32> idxs;
	BoardDefault.GetKeys(idxs);
	for (auto& i : idxs)
	{
		ChangeTile(i, BoardDefault[i]);
		if (DebugTileIndexes)
		{
			auto t = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Tiletext" + i));
			t->AttachTo(RootComponent);
			t->SetText("" + i);
			t->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
			t->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);

			// Set text location and add to text map
			FVector loc;
			WarBoardLib::IndexToWorld(i, true, loc);
			t->SetWorldLocation(loc);
			TextMap.Add(i, t);
		}
	}
}

void ALayoutManager::AssembleGrid()
{
	// TODO::TODO::TODO
	// Iterate over the TileMap adding lines around the edge of each tile
	// Math for lines will be different for hex
}

TArray<int32> ALayoutManager::GetTiles()
{
	TArray<int32> t;
	TileMap.GetKeys(t);
	return t;
}

// Called when the game starts or when spawned
void ALayoutManager::BeginPlay()
{
	Super::BeginPlay();
	
}

