// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"

#include "ProceduralMeshComponent.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;



void FGridCell::BuildCell(ETileShape Shape, float Size, float Thickness, float Padding)
{
	LineThickness = Thickness;
	CellSize = Size - Padding*2;
	// TODO: May get rid of size and just use tilesize
	Polygon = Tile.GetPolygon(Padding);
	BuildPolygonLines();
}

void FGridCell::ProjectVerticesOntoSurface(float Height)
{
	// TODO: if (!bProjectGrid) return;
	for (auto &Vertex : Polygon)
	{
		// TODO: LineTrace from height to find surface to project onto
		// for each vert in poly vert = hit location
	}

}

void FGridCell::BuildPolygonLines()
{
	if (LineThickness <= 0 || CellSize <= 0) return;
	ProjectVerticesOntoSurface(100/*ProjectionHeight*/);
	int32 EndIndex;
	for (int32 StartIndex = 0; StartIndex < Polygon.Num(); StartIndex++)
	{
		EndIndex = fmod(StartIndex + 1, Polygon.Num());

		AddLineDetails(Polygon[StartIndex], Polygon[EndIndex]);
	}
}

void FGridCell::AddLineDetails(FVector Start, FVector End)
{
	// Calculate Triangle Positions
	LineTriangles.Add(LineVertices.Num() + 0);
	LineTriangles.Add(LineVertices.Num() + 1);
	LineTriangles.Add(LineVertices.Num() + 3);
	LineTriangles.Add(LineVertices.Num() + 0);
	LineTriangles.Add(LineVertices.Num() + 3);
	LineTriangles.Add(LineVertices.Num() + 2);


	// Points are the inner edge of line corners
	auto PointOffset = LineThickness / FMath::Sin(FMath::DegreesToRadians(360 / Polygon.Num()));
	// TODO: For some reason this is now pointng up
	auto StartOffset = PointOffset * ((Start - Tile.ToWorld()) * FVector(1,1,0)).GetSafeNormal();
	auto EndOffset = PointOffset * ((End - Tile.ToWorld()) * FVector(1,1,0)).GetSafeNormal();

	// Raise slightly above tile to prevent clipping
	Start += FVector(0, 0, 2);
	End += FVector(0, 0, 2);

	// Calculate Vertex Positions
	LineVertices.Add(Start);
	LineVertices.Add(Start - StartOffset);
	LineVertices.Add(End);
	LineVertices.Add(End - EndOffset);
}




// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Grid Mesh"));
	ProceduralMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridManager::AddCell(FTile Tile)
{
	auto prev = CellArray.FindByPredicate([=](auto Option) { return Option.IsSet() && Option.GetValue() == Tile.ToIndex(); });
	if (prev != nullptr) return;

	FGridCell Cell = FGridCell(Tile);
	Cell.BuildCell(TileShape, WarBoardLib::GetTileSize(), LineThickness, CellPadding);
	CellArray[GetFirstUnsetID()] = Cell;
	DisplayCell(Cell);
}

void AGridManager::RemoveCell(FTile Tile)
{
	FGridCell Cell = FGridCell(Tile);
	int ID = CellArray.Find(Cell);
	if (ID == INDEX_NONE) return;

	ProceduralMesh->ClearMeshSection(ID);
	CellArray[ID].Reset();

	CleanUpArray();
}

void AGridManager::RebuildCells()
{
	ProceduralMesh->ClearAllMeshSections();

	for (auto &Option : CellArray)
	{
		if (Option.IsSet())
		{
			auto Cell = Option.GetValue();
			Cell.BuildCell(TileShape, WarBoardLib::GetTileSize(), LineThickness, CellPadding);
			DisplayCell(Cell);
		}
	}
}

void AGridManager::DisplayInitialCells()
{
	//	REMEMBER:	Cells are empty until BP construction
	for (auto Index : InitialCells)
	{
		AddCell(Index);
	}

}

int32 AGridManager::GetNumUnset()
{
	int32 count = 0;
	for (auto Option : CellArray)
	{
		if (!Option.IsSet()) ++count;
	}
	return count;
}

int32 AGridManager::GetFirstUnsetID()
{
	// Return Id
	for (int i = 0; i < CellArray.Num(); i++)
	{
		if (!CellArray[i].IsSet()) return i;
	}

	// No unset Optionals
	auto Opt = new TOptional<FGridCell>;
	return CellArray.Add(*Opt);

}

void AGridManager::DisplayCell(FGridCell Cell)
{
	int32 ID = CellArray.Find(Cell);
	if (ID == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ID"));
		return;
	}

	// TODO: if(SelectedCell == Cell) Alter cell vertices in some way

	ProceduralMesh->ClearMeshSection(ID);
	ProceduralMesh->CreateMeshSection_LinearColor(ID, Cell.LineVertices, Cell.LineTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);
	
}

void AGridManager::CleanUpArray()
{
	if (GetNumUnset() < 10) return;

	while (GetNumUnset() > 0)
	{
		CellArray.RemoveAt(GetFirstUnsetID());
	}

	RebuildCells();
}
