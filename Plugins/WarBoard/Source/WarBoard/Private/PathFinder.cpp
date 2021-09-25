// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinder.h"
#include "WarBoardLibrary.h"
#include "Math/NumericLimits.h"
#include "PathNode.h"

using namespace WarBoardLib;

// Sets default values
APathFinder::APathFinder()
{
 	// Never Tick
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Set default settings
	AllowDiagonals = false;
	PunishDirectionChanges = false;
	HeavyDiagonals = false;
	TieBreaker = false;
	DebugMode = false;
	DisplayAllPaths = false;
	DisplayNodeStats = false;
	NodeStat = 2;
	HeuristicEstimate = 2;
	SearchLimit = 300;
	HeuristicFormula = EHeuFormula::Manhatan;
	Destination = TNumericLimits<int32>::Lowest();

	// Establish Base Array values
	int32 i;
	TileToIndex(1, 0, i); // Front
	CardinalDirections2D.Add(i);
	TileToIndex(0, 1, i); // Right
	CardinalDirections2D.Add(i);
	TileToIndex(-1, 0, i); // Back
	CardinalDirections2D.Add(i);
	TileToIndex(0, -1, i); // Left
	CardinalDirections2D.Add(i);

	TileToIndex(1, 1, i); // Front-Right
	DiagonalDirections2D.Add(i);
	TileToIndex(-1, 1, i); // Back-Right
	DiagonalDirections2D.Add(i);
	TileToIndex(1, 1, i); // Back-Left
	DiagonalDirections2D.Add(i);
	TileToIndex(1, 1, i); // Front-Left
	DiagonalDirections2D.Add(i);

	// Haven't yet implemented 3D movement
	// Declare here once implemented

	ValidDirections.Append(CardinalDirections2D);
	if (AllowDiagonals) ValidDirections.Append(DiagonalDirections2D);
	if (Allow3DMovement)
	{
		ValidDirections.Append(CardinalDirections3D);
		if (AllowDiagonals) ValidDirections.Append(DiagonalDirections3D);
	}

}

void APathFinder::Initialization(TArray<int32> Locations)
{
	// Add a node for each tile
	for (auto i : Locations)
	{
		Add(i);
	}
}

bool APathFinder::Add(int32 Index)
{
	FVector v;
	IndexToWorld(Index, true, v);
	auto node = GetWorld()->SpawnActor<APathNode>(v, FRotator());
	if (!node) return false;
	NodeMap.Add(Index, node);
	return true;
}

bool APathFinder::Remove(int32 Index)
{
	if (!NodeMap.Contains(Index)) return false;
	NodeMap[Index]->Destroy();
	NodeMap.Remove(Index);
	return true;
}

void APathFinder::Discovery_Implementation(int32 Origin, int32 Range, TArray<int32> &PathableTiles)
{
	if (Range < 1) Range = NodeMap.Num();

	// Initialize variables
	bool stopped = false;

	// Clear Nodes from Queue
	ClearQueue();

	// Set start node values and push to queue
	auto pp = NodeMap.Find(Origin);
	if (!pp) return;	// if index not in node map exit
	auto node = *(pp);
	if (!node) return;	// if node not valid exit
	node->SetNodeValues(0, HeuristicEstimate, Origin, 0);
	OpenNodes.Push(node);

	// Start primary loop and open.pop to get index of node, sort open and ensure node valid
	while (OpenNodes.Num() > 0 && !stopped)
	{
		OpenNodes.Sort([](APathNode& A, const APathNode& B) { return A > B; });
		node = OpenNodes.Pop();
		if (!node) continue;
		auto current = node->Index;

		// If node is Destination, Add to closed, set node checked and stopped is true
		if (*node == Destination)
		{
			ClosedNodes.Add(node);
			node->SetAsChecked();
			stopped = true;
		}

		// If search limit < closed.length
		if (SearchLimit < ClosedNodes.Num()) break;

		// If node is Origin or within range and not obstructed check neighbors
		if (*node == Origin || (GetStatusByIndex(node->Index) > ETileStatus::TS_Obstructed && node->Step < Range))
		{
			// Loop through neighbors
			for (auto direct : ValidDirections)
			{
				auto neighbor = node->Index + direct;
				// Skip if pathable tiles and neighbor not amoung them
				if (PathableTiles.Num() > 0 && !PathableTiles.Contains(neighbor)) continue;

				// Skip if status < obstructed
				if (GetStatusByIndex(neighbor) < ETileStatus::TS_Obstructed) continue;

				// Skip if neighbor in open or closed
				auto pred = [=](APathNode* Result) { return *Result == neighbor; };
				if (OpenNodes.ContainsByPredicate(pred) || ClosedNodes.ContainsByPredicate(pred)) continue;

				// If Destination not valid calculate end point
				int32 end;
				if (NodeMap.Contains(Destination)) end = Destination;
				else
				{
					FVector v;
					IndexToWorld(direct, false, v);
					v.Normalize(.0001);
					WorldToIndex(v * (Range + 1) * GetTileSize(), end);
				}

				// Calculate Tie Breaker
				int32 r1, r2, c1, c2;
				IndexToTile(node->Index - end, r1, c1);
				IndexToTile(Origin - end, r2, c2);
				int32 tb = TieBreaker ? FMath::Abs((r1 * c2) - (r2 * c1)) * .001 : 0;

				// Call CheckNeighbor
				CheckNeighbor(node, direct, end, tb);
			}

		}

		// Add to closed and set node checked
		ClosedNodes.Add(node);
		node->SetAsChecked();
	}
}

void APathFinder::CheckNeighbor_Implementation(APathNode *Current, int32 Direction, int32 Goal, int32 BreakTie = 0)
{
	int32 Neighbor = Current->Index + Direction;

	// Return if neighbor is invalid
	if (!NodeMap.Contains(Neighbor)) return;

	// Calculate cost
	int32 cost = Current->Cost + ETileStatus::TS_Status_MAX + 1 - GetStatusByIndex(Neighbor);

	// Neighbor's cost should never be less than parent's
	if (Current->Cost >= cost) return;

	// Weigh based on direction (is direction in diagonal arrays)
	if (HeavyDiagonals)
	{
		if (DiagonalDirections2D.Contains(Direction) || DiagonalDirections3D.Contains(Direction))
		{
			cost = FMath::RoundToInt(cost * 2.41);
		}
	}

	// Weigh cost if punishing direction changes
	if (PunishDirectionChanges)
	{
		int32 r1, r2, c1, c2;
		IndexToTile(Current->Index - Current->ParentIndex, r1, c1);
		IndexToTile(Direction, r2, c2);
		if (r1 == r2) cost += 20;
		if (c1 == c2) cost += 20;
	}

	// Calculate Heuristic
	int32 row, col, Heu;
	IndexToTile(Neighbor - Goal, row, col);
	row = FMath::Abs(row);
	col = FMath::Abs(col);
	Heu = 0;
	switch (HeuristicFormula)
	{
	case EHeuFormula::Manhatan:
		Heu = HeuristicEstimate * (row + col);
		break;
	case EHeuFormula::MaxDXDY:
		Heu = row > col ? row : col;
		break;
	case EHeuFormula::Diagonal:
		Heu = row < col ? row : col;
		Heu = (Heu * 2 * HeuristicEstimate) + (HeuristicEstimate * (row + col - (Heu * 2)));
		break;
	case EHeuFormula::Euclidean:
		Heu = FMath::Sqrt((row ^ 2) + (col ^ 2)) * HeuristicEstimate;
		break;
	case EHeuFormula::EucNoSQRT:
		Heu = ((row ^ 2) + (col ^ 2)) * HeuristicEstimate;
		break;
	case EHeuFormula::Heu_MAX:
	default:
		break;
	}

	// Apply Tie Breaker
	Heu += BreakTie;

	// Add to open, sort open
	auto pp = NodeMap.Find(Neighbor);
	if (!pp) return;	// if index not in node map exit
	auto node = *(pp);
	if (!node) return;	// if node not valid exit
	node->SetNodeValues(cost, Heu, Current->Index, Current->Step + 1);
	OpenNodes.Add(node);
}

bool APathFinder::Route_Implementation(int32 End, TArray<int32> &RouteArray)
{
	if (!NodeMap.Contains(End)) return false;
	auto node = NodeMap[End];
	bool stop = false, found = false;

	node->SetAsEnd();

	while (!found)
	{
		// Stop if parent isn't valid
		if (!NodeMap.Contains(node->ParentIndex)) break;

		// Debug Display
		// TODO::TODO Draw line to parent, possibly set as vertices for spline

		// Parent calling loop
		if (node->ParentIndex != node->Index)
		{
			RouteArray.Add(node->Index);
			node = NodeMap[node->ParentIndex];
			node->SetAsPath();
		}
		else
		{
			node->SetAsStart();
			found = true;
		}
	}

	return found;
}

bool APathFinder::DirectRoute_Implementation(int32 Start, int32 End, TArray<int32> &RouteArray, TArray<int32> &PathableTiles)
{
	Destination = End;
	Discovery(Start, 0, PathableTiles);
	return Route(End, RouteArray);
}

void APathFinder::ResetNodes()
{
	TArray<int32> t;
	NodeMap.GetKeys(t);
	for (auto i : t)
	{
		auto n = NodeMap[i];
		n->Reset();

		if (DebugMode) n->EnableDebugMode();
	}
}

void APathFinder::ClearQueue()
{
	ClosedNodes.Empty();
	OpenNodes.Empty();
	ResetNodes();
	NodeStat = 0;
}

ETileStatus APathFinder::GetStatusByIndex(int32 Index)
{
	if (!NodeMap.Contains(Index)) return ETileStatus::TS_Invalid;
	if (BlockedNodes.Contains(Index)) return ETileStatus::TS_Blocked;
	if (GetActorAtIndex(GetWorld(), Index) != nullptr) return ETileStatus::TS_Obstructed;
	return ETileStatus::TS_Open;
}

void APathFinder::UpdateStatus(int32 Index, ETileStatus Status)
{
	if (Status > ETileStatus::TS_Blocked) BlockedNodes.Remove(Index);
	if (NodeMap.Contains(Index)) BlockedNodes.AddUnique(Index);
}

TArray<int32> APathFinder::GetReachableTiles(bool IncludeObstructed)
{
	TArray<int32> t;
	for (auto n : ClosedNodes)
	{
		if (IncludeObstructed) t.Add(n->Index);
		else if (GetStatusByIndex(n->Index) > ETileStatus::TS_Obstructed) t.Add(n->Index);
	}

	return t;
}

// Called when the game starts or when spawned
void APathFinder::BeginPlay()
{
	Super::BeginPlay();
	
}

