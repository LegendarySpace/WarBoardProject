// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"

#include "ProceduralMeshComponent.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;



void FGridCell::BuildCell(ETileShape Shape, int Index, float Size, float Thickness, float Padding)
{
	LineThickness = Thickness;
	CellSize = Size - Padding;
	CellIndex = Index;

	switch (Shape)
	{
	case ETileShape::Triangle:
		Polygon = GetCellVertices(3);
		break;
	default:
	case ETileShape::Square:
		Polygon = GetCellVertices(4, 45);
		break;
	case ETileShape::Hex_Hor:
		Polygon = GetCellVertices(6);
		break;
	case ETileShape::Hex_Vert:
		Polygon = GetCellVertices(6, 0, 90);
		break;
		// Add Octogon Cell Vertices (8, 22.5)
	}

	ProjectVerticesOntoSurface(100/*ProjectionHeight*/);
	BuildPolygonLines();
}

TArray<FVector>& FGridCell::GetCellVertices(const int32 Sides, const float RelativeRotationToFirstVertex, const float PolygonRotation)
{
	auto Vertices = new TArray<FVector>;
	if (Sides < 3) return *Vertices;

	auto VertexDistance = (CellSize / 2) / FMath::Cos(FMath::DegreesToRadians(RelativeRotationToFirstVertex));

	float angle = 360.f / Sides;
	for (int32 i = 0; i < Sides; i++)
	{
		float Rotation = (angle * i) + RelativeRotationToFirstVertex + PolygonRotation + 90;
		FVector Vertex = FRotator(0.0, Rotation, 0.0).Vector() * VertexDistance;
		Vertex += IndexToWorld(CellIndex);
		Vertices->Add(Vertex);
	}

	return *Vertices;
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
	LineTriangles.Add(LineVertices.Num() + 2);
	LineTriangles.Add(LineVertices.Num() + 1);
	LineTriangles.Add(LineVertices.Num() + 0);
	LineTriangles.Add(LineVertices.Num() + 2);
	LineTriangles.Add(LineVertices.Num() + 3);
	LineTriangles.Add(LineVertices.Num() + 1);

	// Line Corners distance from point
	auto PointOffset = (LineThickness / 2) / FMath::Sin(FMath::DegreesToRadians(360 / Polygon.Num()));
	auto StartOffset = PointOffset * Start.GetSafeNormal();
	auto EndOffset = PointOffset * End.GetSafeNormal();

	Start += FVector(0, 0, 2);
	End += FVector(0, 0, 2);
	Start -= StartOffset;
	End -= EndOffset;

	// Calculate Vertex Positions
	LineVertices.Add(Start + StartOffset);
	LineVertices.Add(End + EndOffset);
	LineVertices.Add(Start - StartOffset);
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

	AddCell(0);
	AddCell(2);
	AddCell(-2);
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridManager::AddCell(int32 Index)
{
	//if (CellArray.Contains(Index)) return;

	UE_LOG(LogTemp, Warning, TEXT("Adding Cell at %d"), Index);
	FGridCell Cell = FGridCell();
	Cell.BuildCell(TileShape, Index, TileSize, LineThickness, CellPadding);
	CellArray[GetFirstUnsetID()] = Cell;
	DisplayCell(Cell);
}

void AGridManager::RemoveCell(FGridCell Cell)
{
	int32 Index = CellArray.Find(Cell);
	UE_LOG(LogTemp, Warning, TEXT("Removing Cell at %d"), Index);
	ProceduralMesh->ClearMeshSection(Index);
	CellArray[Index].Reset();

	CleanUpArray();
}

void AGridManager::RebuildCells()
{
	ProceduralMesh->ClearAllMeshSections();

	UE_LOG(LogTemp, Warning, TEXT("Rebuilding %d Cells"), CellArray.Num());

	for (auto &Option : CellArray)
	{
		if (Option.IsSet())
		{
			auto Cell = Option.GetValue();
			Cell.BuildCell(TileShape, Cell.CellIndex, TileSize, LineThickness, CellPadding);
			DisplayCell(Cell);
		}
	}
}

void AGridManager::DisplayInitialCells()
{
	//			Cells are empty until BP construction
	UE_LOG(LogTemp, Warning, TEXT("Building %d Initial Cells"), InitialCells.Num());
	for (auto Index : InitialCells)
	{
		UE_LOG(LogTemp, Warning, TEXT("Calling Add Cell"));
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
	int32 id = CellArray.Add(*Opt);
	UE_LOG(LogTemp, Warning, TEXT("First Unset index in Cell Array is %d"), id);
	return id;
}

void AGridManager::DisplayCell(FGridCell Cell)
{
	int32 ID = CellArray.Find(Cell);
	if (ID == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ID"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Displaying Cell at %d"), Cell.CellIndex);
	ProceduralMesh->ClearMeshSection(ID);
	ProceduralMesh->CreateMeshSection_LinearColor(ID, Cell.LineVertices, Cell.LineTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);
	
}

void AGridManager::CleanUpArray()
{
	if (GetNumUnset() < 10) return;

	UE_LOG(LogTemp, Warning, TEXT("Cleaning Base Array"));

	while (GetNumUnset() > 0)
	{
		CellArray.RemoveAt(GetFirstUnsetID());
	}

	RebuildCells();
}
