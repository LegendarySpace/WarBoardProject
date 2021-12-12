// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"

#include "ProceduralMeshComponent.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;



void FGridCell::BuildCell(ETileShape Shape, int Index, float Size, float Thickness, float Padding)
{
	LineThickness = Thickness;
	CellSize = Size - Padding*2;
	CellIndex = Index;

	switch (Shape)
	{
	case ETileShape::Triangle:
		Polygon = GetCellVertices(3, 0, 90);
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
	case ETileShape::Octogon:
		Polygon = GetCellVertices(8, 22.5);
		break;
	case ETileShape::Dodecagon:
		Polygon = GetCellVertices(12, 15);
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
		float Rotation = (angle * i) - RelativeRotationToFirstVertex - PolygonRotation + 90;
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
	LineTriangles.Add(LineVertices.Num() + 0);
	LineTriangles.Add(LineVertices.Num() + 1);
	LineTriangles.Add(LineVertices.Num() + 3);
	LineTriangles.Add(LineVertices.Num() + 0);
	LineTriangles.Add(LineVertices.Num() + 3);
	LineTriangles.Add(LineVertices.Num() + 2);


	// Points are the outer edge of line corners
	auto PointOffset = LineThickness / FMath::Sin(FMath::DegreesToRadians(360 / Polygon.Num()));

	auto StartOffset = PointOffset * (Start - IndexToWorld(CellIndex)).GetSafeNormal();
	auto EndOffset = PointOffset * (End - IndexToWorld(CellIndex)).GetSafeNormal();

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

void AGridManager::AddCell(int32 Index)
{
	auto prev = CellArray.FindByPredicate([=](auto Option) { return Option.IsSet() && Option.GetValue() == Index; });
	if (prev != nullptr) return;

	FGridCell Cell = FGridCell();
	Cell.BuildCell(TileShape, Index, TileSize, LineThickness, CellPadding);
	CellArray[GetFirstUnsetID()] = Cell;
	DisplayCell(Cell);
}

void AGridManager::RemoveCell(int32 Index)
{
	FGridCell Cell;
	Cell.CellIndex = Index;
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
			Cell.BuildCell(TileShape, Cell.CellIndex, TileSize, LineThickness, CellPadding);
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
