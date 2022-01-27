// Fill out your copyright notice in the Description page of Project Settings.


#include "WarBoardLibrary.h"

#include <math.h>

#include "Tiles.h"

AActor* UWarBoardLibrary::Highlighted = nullptr;

AActor * UWarBoardLibrary::GetActorAtTile(UObject* WorldContextObject, const FTile Tile)
{
	// Check 100 units above and 50 below. This may change in future
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	FHitResult result;
	FVector start, end;
	start = Tile.ToWorld() + FVector(0, 0, 100);
	end = Tile.ToWorld() - FVector(0, 0, 50);
	// TODO: Need this to ignore Tiles or it will break Pathfinding
	// if (World->LineTraceSingleByChannel(result, start, end, ECollisionChannel::ECC_PhysicsBody)) return result.GetActor();
	return nullptr;
}

// This will be changed to use a template
TArray<FTile> UWarBoardLibrary::GetTileArray(int32 MinRange, int32 MaxRange, EGridShape Shape)
{
	// This is deprecated, instead use template
	// MinRange is region to start making tiles
	TArray<FTile> Tiles;
	FTile a, b, c, d;
	switch (Shape)
	{
	case EGridShape::Square:
		for (int32 Row = 0; Row <= MaxRange; Row++)
		{
			for (int32 Col = 0; Col <= MaxRange; Col++)
			{
				// Skip if row and col under min
				if (Row >= MinRange || Col >= MinRange)
				{
					Tiles.Add(FTile(Row, Col));
					if (Col > 0)
					{
						// Duplicate across X
						Tiles.Add(FTile(Row, -Col));
					}
					if (Row > 0)
					{
						// Duplicate across Y
						Tiles.Add(FTile(-Row, Col));
					}
					if (Row > 0 && Col > 0)
					{
						// Duplicate across both Axis
						Tiles.Add(FTile(-Row, -Col));
					}
				}
			}
		}
		break;
	case EGridShape::Diamond:
		for (int32 Row = 0; Row <= MaxRange; Row++)
		{
			// Col + Row is the maximum distance
			// therefore loop limit is Max - row
			for (int32 Col = 0; Col <= (MaxRange - Row); Col++)
			{
				// Skip if row + col under min
				if ((Row + Col) >= MinRange)
				{
					Tiles.Add(FTile(Row ,Col));
					if (Col > 0)
					{
						// Duplicate across X
						Tiles.Add(FTile(Row, -Col));
					}
					if (Row > 0)
					{
						// Duplicate across Y
						Tiles.Add(FTile(-Row, Col));
					}
					if (Row > 0 && Col > 0)
					{
						// Duplicate across both Axis
						Tiles.Add(FTile(-Row, -Col));
					}
				}
			}
		}
		break;
	case EGridShape::Cross:
		for (int32 Range = MinRange; Range <= MaxRange; Range++)
		{
			// Cross only increases row or column
			if (Range <= 0)
			{
				Tiles.Add(FTile());
			}
			else
			{
				Tiles.Add(FTile(Range, 0));
				Tiles.Add(FTile(-Range, 0));
				Tiles.Add(FTile(0, Range));
				Tiles.Add(FTile(0, -Range));
			}
		}
		break;
	case EGridShape::Diagonal:
		for (int32 Range = MinRange; Range <= MaxRange; Range++)
		{
			// Diagonal only increases as (row, column)
			if (Range <= 0)
			{
				Tiles.Add(FTile());
			}
			else
			{
				Tiles.Add(FTile(Range, Range));
				Tiles.Add(FTile(-Range, Range));
				Tiles.Add(FTile(-Range, -Range));
				Tiles.Add(FTile(Range, -Range));
			}
		}
		break;
	case EGridShape::CrossDiagonal:
		for (int32 Range = MinRange; Range <= MaxRange; Range++)
		{
			// First make cross then diagonal for each range
			if (Range <= 0)
			{
				Tiles.Add(FTile());
			}
			else
			{
				// add cross
				Tiles.Add(FTile(Range, 0));
				Tiles.Add(FTile(-Range, 0));
				Tiles.Add(FTile(0, Range));
				Tiles.Add(FTile(0, -Range));
				
				// then add diagonal
				Tiles.Add(FTile(Range, Range));
				Tiles.Add(FTile(-Range, Range));
				Tiles.Add(FTile(-Range, -Range));
				Tiles.Add(FTile(Range, -Range));
			}
		}
		break;
	case EGridShape::Rhombus:
		for (int32 Range = MinRange; Range <= MaxRange; Range++)
		{
			// Rhombus horizontal calculations
			if (Range <= 0)
			{
				Tiles.Add(FTile());
			}
			else
			{
				// Calculate points to build from
				a = FTile(Range, 0);
				Tiles.Add(a);
				b = FTile(Range, 2 * Range);
				Tiles.Add(b);
				c = FTile(-Range, 0);
				Tiles.Add(c);
				d = FTile(-Range, -2 * Range);
				Tiles.Add(d);

				for (int32 Circ = 1; Circ <= (2 * Range - 1); Circ++)
				{
					// Build outward from starting point
					Tiles.Add(a + FTile(0, Circ));
					Tiles.Add(b + FTile(-Circ, -Circ));
					Tiles.Add(c + FTile(0, -Circ));
					Tiles.Add(d + FTile(Circ, Circ));
				}
			}
		}
		break;
	case EGridShape::RhombusVert:
		for (int32 Range = MinRange; Range <= MaxRange; Range++)
		{
			// Rhombus vertical calculations
			if (Range <= 0)
			{
				Tiles.Add(FTile());
			}
			else
			{
				// Calculate points to build from
				a = FTile(0, Range);
				Tiles.Add(a);
				b = FTile(-2 * Range, Range);
				Tiles.Add(b);
				c = FTile(0, -Range);
				Tiles.Add(c);
				d = FTile(2 * Range, -Range);
				Tiles.Add(d);

				for (int32 Circ = 1; Circ <= (2 * Range - 1); Circ++)
				{
					// Build outward from starting point
					Tiles.Add(a + FTile(-Circ, 0));
					Tiles.Add(b + FTile(Circ, -Circ));
					Tiles.Add(c + FTile(Circ, 0));
					Tiles.Add(d + FTile(-Circ, Circ));
				}
			}
		}
		break;
	default:
		break;
	}


	return Tiles;
}

bool UWarBoardLibrary::GetValidatedTileArray(int32 Origin, int32 MinRange, int32 MaxRange, EGridShape Shape, TArray<int32>& ValidatedShape, const TArray<int32> Accessible)
{
	if (!Accessible.Contains(Origin)) return false;

	auto baseArray = GetTileArray(MinRange, MaxRange, Shape);
	for (auto i : baseArray)
	{
		if (Accessible.Contains(i.ToIndex() + Origin)) ValidatedShape.Add(i.ToIndex() + Origin);
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
