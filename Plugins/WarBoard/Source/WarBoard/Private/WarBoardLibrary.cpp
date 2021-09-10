// Fill out your copyright notice in the Description page of Project Settings.


#include "WarBoardLibrary.h"
#include <math.h>

float UWarBoardLibrary::TileSize = 200.f;
int32 UWarBoardLibrary::MaxWidth = 6500;
int32 UWarBoardLibrary::Offset = 0;
bool UWarBoardLibrary::BoardCentered = true;
FVector UWarBoardLibrary::BoardLocation = FVector(0.f);

bool UWarBoardLibrary::IndexToWorld(int32 Index, bool TileCenter, FVector &Location)
{
	// initialize static variables on each use to ensure accuracy
	int32 i = Index, row = 0, col = 0;
	initialize();

	if (!UWarBoardLibrary::BoardCentered) i += UWarBoardLibrary::Offset;
	IndexToTile(i, row, col);
	Location = (FVector(row, col, 0.f) * UWarBoardLibrary::TileSize) + UWarBoardLibrary::BoardLocation;
	// Default is lower left corner, add half tile offset if centered
	if (TileCenter) Location += (FVector(.5, .5, 0.f) * UWarBoardLibrary::TileSize);
	return IndexValid(Index);
}

void UWarBoardLibrary::IndexToTile(int32 Index, int32 & Row, int32 & Col)
{
	// For mod to work correctly, need to offset it in the direction of the sign of the index to retain it's relation to 0
	int32 adj = (Index / FMath::Abs(Index)) * (UWarBoardLibrary::MaxWidth / 2);
	Row = FMath::RoundToInt((float)Index / (float)UWarBoardLibrary::MaxWidth);
	Col = FMath::Fmod(Index + adj, UWarBoardLibrary::MaxWidth) - adj;
}

bool UWarBoardLibrary::TileToWorld(int32 Row, int32 Col, bool TileCenter, FVector & Location)
{
	// initialize static variables on each use to ensure accuracy
	int32 i, row, col;
	initialize();

	TileToIndex(Row, Col, i);
	if (!UWarBoardLibrary::BoardCentered) i += UWarBoardLibrary::Offset;
	IndexToTile(i, row, col);
	Location = (FVector(row, col, 0.f) * UWarBoardLibrary::TileSize) + UWarBoardLibrary::BoardLocation;
	// Default is lower left corner, add half tile offset if centered
	if (TileCenter) Location += (FVector(.5, .5, 0.f) * UWarBoardLibrary::TileSize);
	return TileValid(Row, Col);
}

void UWarBoardLibrary::TileToIndex(int32 Row, int32 Col, int32 & Index)
{
	Index = Row * UWarBoardLibrary::MaxWidth + Col;
}

bool UWarBoardLibrary::WorldToIndex(FVector Location, int32 & Index)
{
	// Determine Location relative to board
	FVector relative = Location - UWarBoardLibrary::BoardLocation;

	TileToIndex((relative / UWarBoardLibrary::TileSize).X, (relative / UWarBoardLibrary::TileSize).Y, Index);
	return IndexValid(Index);
}

bool UWarBoardLibrary::WorldToTile(FVector Location, int32 & Row, int32 & Col)
{
	// Determine Location relative to board
	FVector relative = Location - UWarBoardLibrary::BoardLocation;

	Row = (relative / UWarBoardLibrary::TileSize).X;
	Col = (relative / UWarBoardLibrary::TileSize).Y;
	return TileValid(Row, Col);
}

bool UWarBoardLibrary::TileValid(int32 Row, int32 Col)
{
	int32 i;
	TileToIndex(Row, Col, i);
	return GetTileIndexes().Contains(i);
}

bool UWarBoardLibrary::IndexValid(int32 Index)
{
	return GetTileIndexes().Contains(Index);
}

TArray<int32> UWarBoardLibrary::GetTileIndexes()
{
	// TODO::TODO::TODO
	// Get Actor of Class Board->GetTiles
	// If Board doesn't exist return empty array
	return TArray<int32>();
}

TArray<int32> UWarBoardLibrary::GetAllObstructedIndexes()
{
	// TODO::TODO::TODO
	// Get All actors with obstruction tag
	// for each add WorldToIndex(Actor.Location) to array
	return TArray<int32>();
}

bool UWarBoardLibrary::GetIndexPassable(int32 Index)
{
	return !(GetAllObstructedIndexes().Contains(Index));
}

TArray<int32> UWarBoardLibrary::GetTileArray(int32 MinRange, int32 MaxRange, EGridShape Shape)
{
	// MinRange is region to start making tiles
	TArray<int32> tiles;
	int32 a, b, c, d, i, j;
	switch (Shape)
	{
	case EGridShape::Square:
		for (int32 row = 0; row <= MaxRange; row++)
		{
			for (int32 col = 0; col <= MaxRange; col++)
			{
				// Skip if row and col under min
				if (row >= MinRange || col >= MinRange)
				{
					TileToIndex(row, col, i);
					tiles.Add(i);
					if (col > 0)
					{
						// Duplicate across X
						TileToIndex(row, -col, i);
						tiles.Add(i);
					}
					if (row > 0)
					{
						// Duplicate across Y
						TileToIndex(-row, col, i);
						tiles.Add(i);
					}
					if (row > 0 && col > 0)
					{
						// Duplicate across both Axis
						TileToIndex(-row, -col, i);
						tiles.Add(i);
					}
				}
			}
		}
		break;
	case EGridShape::Diamond:
		for (int32 row = 0; row <= MaxRange; row++)
		{
			// Col + Row is the maximum distance
			// therefore loop limit is Max - row
			for (int32 col = 0; col <= (MaxRange - row); col++)
			{
				// Skip if row + col under min
				if ((row + col) >= MinRange)
				{
					TileToIndex(row, col, i);
					tiles.Add(i);
					if (col > 0)
					{
						// Duplicate across X
						TileToIndex(row, -col, i);
						tiles.Add(i);
					}
					if (row > 0)
					{
						// Duplicate across Y
						TileToIndex(-row, col, i);
						tiles.Add(i);
					}
					if (row > 0 && col > 0)
					{
						// Duplicate across both Axis
						TileToIndex(-row, -col, i);
						tiles.Add(i);
					}
				}
			}
		}
		break;
	case EGridShape::Cross:
		for (int32 range = MinRange; range <= MaxRange; range++)
		{
			// Cross only increases row or column
			if (i <= 0)
			{
				TileToIndex(range, range, i);
				tiles.Add(i);
			}
			else
			{
				TileToIndex(range, 0, i);
				tiles.Add(i);
				TileToIndex(-range, 0, i);
				tiles.Add(i);
				TileToIndex(0, range, i);
				tiles.Add(i);
				TileToIndex(0, -range, i);
				tiles.Add(i);
			}
		}
		break;
	case EGridShape::Diagonal:
		for (int32 range = MinRange; range <= MaxRange; range++)
		{
			// Diagonal only increases as (row, column)
			if (i <= 0)
			{
				TileToIndex(range, range, i);
				tiles.Add(i);
			}
			else
			{
				TileToIndex(range, range, i);
				tiles.Add(i);
				TileToIndex(-range, range, i);
				tiles.Add(i);
				TileToIndex(-range, -range, i);
				tiles.Add(i);
				TileToIndex(range, -range, i);
				tiles.Add(i);
			}
		}
		break;
	case EGridShape::CrossDiagonal:
		for (int32 range = MinRange; range <= MaxRange; range++)
		{
			// First make cross then diagonal for each range
			if (i <= 0)
			{
				TileToIndex(range, range, i);
				tiles.Add(i);
			}
			else
			{
				// add cross
				TileToIndex(range, 0, i);
				tiles.Add(i);
				TileToIndex(-range, 0, i);
				tiles.Add(i);
				TileToIndex(0, range, i);
				tiles.Add(i);
				TileToIndex(0, -range, i);
				tiles.Add(i);
				
				// then add diagonal
				TileToIndex(range, range, i);
				tiles.Add(i);
				TileToIndex(-range, range, i);
				tiles.Add(i);
				TileToIndex(-range, -range, i);
				tiles.Add(i);
				TileToIndex(range, -range, i);
				tiles.Add(i);
			}
		}
		break;
	case EGridShape::Rhombus:
		for (int32 range = MinRange; range <= MaxRange; range++)
		{
			// Rhombus horizontal calculations
			if (i <= 0)
			{
				TileToIndex(range, range, a);
				tiles.Add(a);
			}
			else
			{
				// Calculate points to build from
				TileToIndex(range, 0, a);
				tiles.Add(a);
				TileToIndex(range, 2 * range, b);
				tiles.Add(b);
				TileToIndex(-range, 0, c);
				tiles.Add(c);
				TileToIndex(-range, -2 * range, d);
				tiles.Add(d);

				for (int32 circ = 1; circ <= (2*range-1); circ++)
				{
					// Build outward from starting point
					TileToIndex(0, circ, j);
					tiles.Add(a + j);
					TileToIndex(-circ, -circ, j);
					tiles.Add(b + j);
					TileToIndex(0, -circ, j);
					tiles.Add(c + j);
					TileToIndex(circ, circ, j);
					tiles.Add(d + j);
				}
			}
		}
		break;
	case EGridShape::RhombusVert:
		for (int32 range = MinRange; range <= MaxRange; range++)
		{
			// Rhombus vertical calculations
			if (i <= 0)
			{
				TileToIndex(range, range, a);
				tiles.Add(a);
			}
			else
			{
				// Calculate points to build from
				TileToIndex(0, range, a);
				tiles.Add(a);
				TileToIndex(-2 * range, range, b);
				tiles.Add(b);
				TileToIndex(0, -range, c);
				tiles.Add(c);
				TileToIndex(2 * range, -range, d);
				tiles.Add(d);

				for (int32 circ = 1; circ <= (2 * range - 1); circ++)
				{
					// Build outward from starting point
					TileToIndex(-circ, 0, j);
					tiles.Add(a + j);
					TileToIndex(circ, -circ, j);
					tiles.Add(b + j);
					TileToIndex(circ, 0, j);
					tiles.Add(c + j);
					TileToIndex(-circ, circ, j);
					tiles.Add(d + j);
				}
			}
		}
		break;
	default:
		break;
	}


	return tiles;
}

bool UWarBoardLibrary::GetValidatedTileArray(int32 Origin, int32 MinRange, int32 MaxRange, EGridShape Shape, TArray<int32>& Validated)
{
	if (!IndexValid(Origin)) return false;

	auto baseArray = GetTileArray(MinRange, MaxRange, Shape);
	for (auto i : baseArray)
	{
		if (GetTileIndexes().Contains(i + Origin)) Validated.Add(i + Origin);
	}

	return true;
}

void UWarBoardLibrary::CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
	// Calculate Triangle Positions
	Triangles.Add(Vertices.Num() + 2);
	Triangles.Add(Vertices.Num() + 1);
	Triangles.Add(Vertices.Num() + 0);
	Triangles.Add(Vertices.Num() + 2);
	Triangles.Add(Vertices.Num() + 1);
	Triangles.Add(Vertices.Num() + 0);
	
	// Setup pointOffset
	auto v = (End - Start);
	v.Normalize(.0001);
	auto pointOffset = FVector::CrossProduct(v, FVector(0.f, 0.f, 1.f)) * (Thickness / 2);

	// Calculate Vertex Positions
	Vertices.Add(Start + pointOffset);
	Vertices.Add(End + pointOffset);
	Vertices.Add(Start - pointOffset);
	Vertices.Add(End - pointOffset);
}

void UWarBoardLibrary::initialize()
{
	// TODO::TODO::TODO
	// Get board from Get all actors of class and use to initialize variables
}
