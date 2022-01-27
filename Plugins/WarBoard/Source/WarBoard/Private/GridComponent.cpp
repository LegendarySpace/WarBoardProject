// Fill out your copyright notice in the Description page of Project Settings.


#include "GridComponent.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;



void FGridCell::BuildCell(ETileShape Shape, float Size, float Thickness, float Padding)
{
	if (Size <= 0) Size = GetTileSize();
	if (Thickness >= 0) LineThickness = Thickness;
	float pad = 0;
	if (Padding >= 1 && Padding < (Size / 2)) pad = Padding;
	if (Padding > 0 && Padding < 1) pad = Size / 2 * Padding;
	CellSize = Size - pad;
	Polygon = Tile.GetPolygon(pad);
	BuildPolygonLines();
}

void FGridCell::ProjectVerticesOntoSurface(float Height)
{
	if (!bProjectGrid) return;
	for (auto &Vertex : Polygon)
	{
		// UPGRADE: LineTrace from height to find surface to project onto
		// for each vert in poly vert.z = hit location.z + 1
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
	LineTriangles.Add(LineVertices.Num() + 0);
	LineTriangles.Add(LineVertices.Num() + 1);
	LineTriangles.Add(LineVertices.Num() + 3);
	LineTriangles.Add(LineVertices.Num() + 0);
	LineTriangles.Add(LineVertices.Num() + 3);
	LineTriangles.Add(LineVertices.Num() + 2);

	// PointOffset is the inner edge of line corners
	auto PointOffset = LineThickness / FMath::Sin(FMath::DegreesToRadians(360 / Polygon.Num()));
	auto StartOffset = PointOffset * ((Start - Tile.ToWorld()) * FVector(1,1,0)).GetSafeNormal();
	auto EndOffset = PointOffset * ((End - Tile.ToWorld()) * FVector(1,1,0)).GetSafeNormal();

	// Raise slightly above tile to prevent clipping
	Start += FVector(0, 0, .5);
	End += FVector(0, 0, .5);

	LineVertices.Add(Start);
	LineVertices.Add(Start - StartOffset);
	LineVertices.Add(End);
	LineVertices.Add(End - EndOffset);
}




// Sets default values
UGridComponent::UGridComponent(const FObjectInitializer& ObjectInitializer) : UProceduralMeshComponent(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void UGridComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGridComponent::AddCell(FGCoord Tile)
{
	if (CellArray.Find(FGridCell(Tile)) != INDEX_NONE) return; // Cell already exists

	FGridCell Cell = FGridCell(Tile);
	Cell.BuildCell(GetTileShape(), GetTileSize(), LineThickness, CellPadding);
	CellArray[GetFirstUnsetID()] = Cell;
	DisplayCell(Cell);
}

void UGridComponent::RemoveCell(FGCoord Tile)
{
	FGridCell Cell = FGridCell(Tile);
	int32 ID = CellArray.Find(Cell);
	if (ID == INDEX_NONE) return;

	this->ClearMeshSection(ID);
	CellArray[ID].Reset();

	CleanUpArray();
}

void UGridComponent::RebuildCells()
{
	for (auto &Option : CellArray)
	{
		if (Option.IsSet())
		{
			auto Cell = Option.GetValue();
			Cell.BuildCell(GetTileShape(), GetTileSize(), LineThickness, CellPadding);
			DisplayCell(Cell);
		}
	}
}

void UGridComponent::Populate(TArray<FGCoord> Tiles)
{
	for (auto Tile : Tiles)
	{
		AddCell(Tile);
	}

}

void UGridComponent::SetPadding(float InPadding)
{
	CellPadding = InPadding;
	RebuildCells();
}

int32 UGridComponent::GetNumUnset()
{
	int32 count = 0;
	for (auto Option : CellArray)
	{
		if (!Option.IsSet()) ++count;
	}
	return count;
}

int32 UGridComponent::GetFirstUnsetID()
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

void UGridComponent::DisplayCell(FGridCell Cell)
{
	int32 ID = CellArray.Find(Cell);
	if (ID == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ID"));
		return;
	}

	// UPGRADE: if(SelectedCell == Cell) Alter cell vertices in some way

	this->ClearMeshSection(ID);
	this->CreateMeshSection_LinearColor(ID, Cell.LineVertices, Cell.LineTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);
	
}

void UGridComponent::CleanUpArray()
{
	if (GetNumUnset() < 10) return;

	while (GetNumUnset() > 0)
	{
		CellArray.RemoveAt(GetFirstUnsetID());
	}

	RebuildCells();
}


