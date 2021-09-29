// Fill out your copyright notice in the Description page of Project Settings.


#include "LayoutManager.h"
#include "TileTypeManager.h"
#include "WarBoardLibrary.h"
#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ProceduralMeshComponent.h"
#include <string.h>

// Sets default values
ALayoutManager::ALayoutManager()
{
 	// Should never tick
	PrimaryActorTick.bCanEverTick = false;
	
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Grid Mesh"));

	// Determine Base Tile (Used when no mesh is provided)
	UStaticMesh* mesh;
	switch (TileShape)
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
	default:
		mesh = nullptr;
		break;
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
		if (k.Contains(type)) mesh = *(TileMeshes.Find(type));
		
		m->SetupInstance(type, mesh, TileSize);
	}

	// Initialize Board
	AssembleTiles();
	AssembleGrid();
}

int32 ALayoutManager::GetOffset()
{
	// TODO
	return int32();
}

FVector ALayoutManager::GetBoardSize()
{
	// TODO
	return FVector();
}

bool ALayoutManager::ChangeTile(int32 Index, ETileType Type)
{
	// Remove Index from existing Manager
	// Add Index to Manager at location Type
	// Add Index to TileMap

	// Use TileMap to look up TileType of Index, TileType is == to position within Managers
	if (TileMap.Contains(Index)) Managers[TileMap[Index]]->Remove(Index);
	else OnTileAdd.Broadcast(Index);
	Managers[Type]->Add(Index);
	TileMap.Add(Index, Type);
	OnTileChange.Broadcast(Index, Type);
	return true;
}

bool ALayoutManager::RemoveTile(int32 Index)
{
	// Remove Index from existing Manager then remove Index from TileMap
	if (TileMap.Contains(Index))
	{
		Managers[TileMap[Index]]->Remove(Index);
		TileMap.Remove(Index);
		OnTileRemove.Broadcast(Index);
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
			t->SetWorldLocation(WarBoardLib::IndexToWorld(i, true));
			TextMap.Add(i, t);
		}
	}
}

void ALayoutManager::AssembleGrid()
{
	// For each tile, create polygon at tile position
	TArray<FVector> Vertices = TArray<FVector>();
	TArray<int32> k, Triangles = TArray<int32>();
	TileMap.GetKeys(k);
	for (auto i : k)
	{
		Vertices.Empty();
		Triangles.Empty();
		UWarBoardLibrary::CreatePolygon(TileShape, UWarBoardLibrary::IndexToWorld(i), TileSize - (GridPadding * 2), GridThickness, Vertices, Triangles);
		ProcMesh->CreateMeshSection(i, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	}
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
	
	// Ensure any changes are passed along to library
	WarBoardLib::InitializeTiles(TileSize, TileShape);
}

