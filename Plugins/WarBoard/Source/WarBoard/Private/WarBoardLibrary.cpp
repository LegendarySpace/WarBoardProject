// Fill out your copyright notice in the Description page of Project Settings.


#include "WarBoardLibrary.h"
#include <math.h>

float UWarBoardLibrary::TileSize = 200.f;
ETileShape UWarBoardLibrary::TileShape = ETileShape::Square;
int32 UWarBoardLibrary::MaxWidth = 6500;
FVector UWarBoardLibrary::GridOffset = FVector(0.0);
AActor* UWarBoardLibrary::Highlighted = nullptr;

void UWarBoardLibrary::InitializeTiles(float Size, ETileShape Shape, const FVector Offset)
{
	UWarBoardLibrary::TileSize = Size;
	UWarBoardLibrary::TileShape = Shape;
	UWarBoardLibrary::GridOffset = Offset;
}

void UWarBoardLibrary::CalculatePosition(int32 & Row, int32 & Col, FVector & WorldLocation, bool ToWorld) // add bool for offset addition
{
	if (TileSize < 1) return;
	if (ToWorld)
	{
		// Convert to world position
		WorldLocation = FVector(0.0);
		switch (TileShape)
		{
		case ETileShape::Square:
			WorldLocation.X = Row * TileSize;
			WorldLocation.Y = Col * TileSize;
			break;
		case ETileShape::Hex_Hor:
			// Col are tilesize apart, row is SQRT(3) * size/2
			WorldLocation.X = Row * sqrt(3) * (TileSize / 2);
			WorldLocation.Y = Col * TileSize;
			break;
		case ETileShape::Hex_Vert:
			// Rows are tilesize apart, cols are sqrt(3) * size/2
			WorldLocation.X = Row * TileSize;
			WorldLocation.Y = Col * sqrt(3) * (TileSize / 2);
			break;
		case ETileShape::Shape_MAX:
			break;
		default:
			return;
		}
		WorldLocation = WorldLocation + UWarBoardLibrary::GridOffset;
	}
	else
	{
		// Convert from world position
		auto loc = WorldLocation - UWarBoardLibrary::GridOffset;
		switch (TileShape)
		{
		case ETileShape::Square:
			Row = loc.X / TileSize;
			Col = loc.Y / TileSize;
			break;
		case ETileShape::Hex_Hor:
			// Col are tilesize apart, row is SQRT(3) * size/2
			Row = loc.X / (sqrt(3) * TileSize / 2);
			Col = loc.Y / TileSize;
			break;
		case ETileShape::Hex_Vert:
			// Rows are tilesize apart, cols are sqrt(3) * size/2
			Row = loc.X / TileSize;
			Col = loc.Y / (sqrt(3) * TileSize / 2);
			break;
		case ETileShape::Shape_MAX:
		default:
			return;
		}
	}
}

// Used for Hex Spacing
FVector UWarBoardLibrary::IndexToCube(const int32 Row, const int32 Col)
{
	// Note the vector positions does not use world axis
	int32 x = Col;
	int32 z = Row - (x - (x & 1)) / 2;
	int32 y = -x - z;
	return FVector(x, y, z);
}

// Used with Hex Spacing
void UWarBoardLibrary::CubeToIndex(int32 & Row, int32 & Col, const FVector Cube)
{
	// Note the vector positions does not use world axis
	Row = Cube.X;
	Col = Cube.Z + (Cube.X - ((int32)Cube.X & 1)) / 2;
}

FVector UWarBoardLibrary::IndexToWorld(int32 Index, bool TileCenter)
{
	int32 row = 0, col = 0;
	FVector Location;

	IndexToTile(Index, row, col);
	UWarBoardLibrary::CalculatePosition(row, col, Location);
	if (!TileCenter) Location -= FVector(.5, .5, 0.f) * UWarBoardLibrary::TileSize;
	return Location;
}

void UWarBoardLibrary::IndexToTile(int32 Index, int32 & Row, int32 & Col)
{
	// For mod to work correctly, need to offset it in the direction of the sign of the index to retain it's relation to 0
	int32 adj = UWarBoardLibrary::MaxWidth / 2;
	if (Index != 0) adj *= FMath::Sign(Index);
	Row = FMath::RoundToInt((float)Index / (float)UWarBoardLibrary::MaxWidth);
	Col = FMath::Fmod(Index + adj, UWarBoardLibrary::MaxWidth) - adj;
}

FVector UWarBoardLibrary::TileToWorld(int32 Row, int32 Col, bool TileCenter)
{
	FVector Location;
	UWarBoardLibrary::CalculatePosition(Row, Col, Location);
	if (!TileCenter) Location -= FVector(.5, .5, 0.f) * UWarBoardLibrary::TileSize;
	return Location;
}

void UWarBoardLibrary::TileToIndex(int32 Row, int32 Col, int32 & Index)
{
	Index = Row * UWarBoardLibrary::MaxWidth + Col;
}

void UWarBoardLibrary::WorldToIndex(FVector Location, int32 & Index)
{
	int32 row, col;

	UWarBoardLibrary::CalculatePosition(row, col, Location, false);
	TileToIndex(row, col, Index);
}

void UWarBoardLibrary::WorldToTile(FVector Location, int32 & Row, int32 & Col)
{
	UWarBoardLibrary::CalculatePosition(Row, Col, Location, false);
}

AActor * UWarBoardLibrary::GetActorAtIndex(UObject* WorldContextObject, const int32 Index)
{
	// TODO May switch to sphere trace and return all 
	// Perform line trace and retrieve actor if one exists
	// End 150 units above index location
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	FHitResult result;
	FVector start, end;
	IndexToWorld(Index, true);
	end = start + FVector(0.f, 0.f, 150.f);

	if (World->LineTraceSingleByChannel(result, start, end, ECollisionChannel::ECC_PhysicsBody)) return result.GetActor();
	else return nullptr;
}

AActor * UWarBoardLibrary::GetActorAtTile(UObject* WorldContextObject, const int32 Row, const int32 Col)
{
	int32 i;
	TileToIndex(Row, Col, i);
	GetActorAtIndex(WorldContextObject, i);
	return nullptr;
}

TArray<int32> UWarBoardLibrary::GetTileArray(int32 MinRange, int32 MaxRange, EGridShape Shape)
{
	// This is deprecated, instead use template
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
			if (range <= 0)
			{
				TileToIndex(0, 0, i);
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
			if (range <= 0)
			{
				TileToIndex(0, 0, i);
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
			if (range <= 0)
			{
				TileToIndex(0, 0, i);
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
			if (range <= 0)
			{
				TileToIndex(0, 0, a);
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
			if (range <= 0)
			{
				TileToIndex(0, 0, a);
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

// This will be changed to use a template
bool UWarBoardLibrary::GetValidatedTileArray(int32 Origin, int32 MinRange, int32 MaxRange, EGridShape Shape, TArray<int32>& ValidatedShape, const TArray<int32> Accessible)
{
	if (!Accessible.Contains(Origin)) return false;

	auto baseArray = GetTileArray(MinRange, MaxRange, Shape);
	for (auto i : baseArray)
	{
		if (Accessible.Contains(i + Origin)) ValidatedShape.Add(i + Origin);
	}

	return true;
}

bool UWarBoardLibrary::IsSameTeam(const AActor * A, const AActor * B)
{
	if (A == nullptr || B == nullptr) return false;
	FName teamA = FName(TEXT(""));

	// Search A for "Team" tag, return false if not found
	for (auto atag : A->Tags)
	{
		if (atag.ToString().Contains("Team"))
		{
			teamA = atag;
			break;
		}
	}
	if (teamA.GetStringLength() == 0) return false;

	// Return true if B has same "Team" tag
	// Returns false if B has no tag
	return B->ActorHasTag(teamA);
}

bool UWarBoardLibrary::IsEnemyTeam(const AActor * A, const AActor * B)
{
	if (A == nullptr || B == nullptr) return false;
	FName teamA = FName(TEXT(""));
	FName teamB = FName(TEXT(""));

	// Search A for "Team" tag, return false if not found
	for (auto tag : A->Tags)
	{
		if (tag.ToString().Contains("Team"))
		{
			teamA = tag;
			break;
		}
	}
	if (teamA.GetStringLength() == 0) return false;

	// Search B for "Team" tag, return false if not found
	for (auto tag : B->Tags)
	{
		if (tag.ToString().Contains("Team"))
		{
			teamB = tag;
			break;
		}
	}
	if (teamA.GetStringLength() == 0) return false;

	// A and B both have "Team" tags, return true if different
	return !teamB.IsEqual(teamA);

}
