// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinder.h"

#include "Math/NumericLimits.h"

#include "WarBoardLibrary.h"
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
	Destination.Reset();

	// Establish Base Array values
	// TODO: Establish The Cardinal and Diagonal Direction based on shape

	CardinalDirections2D.Add(FGCoord(1,0)); // Front
	CardinalDirections2D.Add(FGCoord(0,1)); // Right
	CardinalDirections2D.Add(FGCoord(-1,0)); // Back
	CardinalDirections2D.Add(FGCoord(0,-1)); // Left

	DiagonalDirections2D.Add(FGCoord(1,1)); // Front-Right
	DiagonalDirections2D.Add(FGCoord(-1,1)); // Back-Right
	DiagonalDirections2D.Add(FGCoord(-1,-1)); // Back-Left
	DiagonalDirections2D.Add(FGCoord(1,-1)); // Front-Left

	// Haven't yet implemented 3D movement
	// Declare here once implemented

}

void APathFinder::DetermineDirections()
{
	ValidDirections.Append(CardinalDirections2D);
	if (AllowDiagonals) ValidDirections.Append(DiagonalDirections2D);
	if (Allow3DMovement)
	{
		ValidDirections.Append(CardinalDirections3D);
		if (AllowDiagonals) ValidDirections.Append(DiagonalDirections3D);
	}

}

void APathFinder::Initialization(TArray<FTile> Locations)
{
	DetermineDirections();

	// Add a node for each tile
	for (auto Tile : Locations)
	{
		Add(Tile);
	}
}

void APathFinder::InitializeFromCoords(TArray<FGCoord> Locations)
{
	DetermineDirections();

	// Add a node for each tile
	for (auto Coord : Locations)
	{
		Add(Coord);
	}
}

bool APathFinder::Add(FTile InTile)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto Node = GetWorld()->SpawnActor<APathNode>(InTile.ToWorld(), FRotator(), Params);
	if (!Node) return false; 
	if (!Node->GetActorLocation().Equals(InTile.ToWorld())) Node->SetActorLocation(InTile.ToWorld());
	NodeMap.Add(Node);
	return true;
}

bool APathFinder::Remove(FTile InTile)
{
	auto Node = GetNode(InTile);
	Node->Destroy();
	NodeMap.Remove(Node);
	return true;
}

void APathFinder::Discovery_Implementation(FTile Origin, int32 Range, TArray<FTile> &PathableTiles)
{
	if (Range < 1) Range = NodeMap.Num();

	// Initialize variables
	bool stopped = false;

	// Clear Nodes from Queue
	ClearQueue();

	// Set start node values and push to queue
	auto Node = GetNode(Origin);
	Node->SetNodeValues(0, HeuristicEstimate, Origin, 0);
	OpenNodes.Push(Node);

	// Start primary loop and open.pop to get index of node, sort open and ensure node valid
	while (OpenNodes.Num() > 0 && !stopped)
	{
		OpenNodes.Sort([](APathNode& A, const APathNode& B) { return A > B; });
		Node = OpenNodes.Pop();
		if (!Node) continue;

		// If node is Destination, Add to closed, set node checked and stopped is true
		if (Destination.IsSet() && *Node == Destination.GetValue())
		{
			ClosedNodes.Add(Node);
			Node->SetAsChecked();
			stopped = true;
		}

		// If search limit < closed.length
		if (SearchLimit < ClosedNodes.Num()) break;

		// If node is Origin or within range and not obstructed check neighbors
		if (*Node == Origin || (GetStatusByTile(Node->Tile) > ETileStatus::TS_Obstructed && Node->Step < Range))
		{
			// Loop through neighbors
			for (auto Direction : ValidDirections)
			{
				FTile Neighbor = Node->Tile + Direction;
				// Skip if pathable tiles and neighbor not amoung them
				if (PathableTiles.Num() > 0 && !PathableTiles.Contains(Neighbor)) continue;

				// Skip if status < obstructed
				if (GetStatusByTile(Neighbor) < ETileStatus::TS_Obstructed) continue;

				// Skip if neighbor in open or closed
				auto pred = [=](APathNode* Node) { return *Node == Neighbor; };
				if (OpenNodes.FindByPredicate(pred) || ClosedNodes.FindByPredicate(pred)) continue;

				// If Destination not valid calculate end point
				FTile end;
				if (Destination.IsSet() && GetNode(Destination.GetValue())) end = Destination.GetValue();
				else end = FTile(Direction).ToWorld().GetSafeNormal() * (Range + 1) * WarBoardLib::GetTileSize();

				// Calculate Tie Breaker here, Origin is not available where tiebreaker is used
				FTile TileA = Node->Tile - end;
				FTile TileB = Origin - end;
				int32 tb = TieBreaker ? FMath::Abs((TileA.ToRC().Row * TileB.ToRC().Column) - (TileB.ToRC().Row * TileA.ToRC().Column)) * .001 : 0;

				// Call CheckNeighbor
				CheckNeighbor(Node, Direction, end, tb);
			}

		}

		// Add to closed and set node checked
		ClosedNodes.Add(Node);
		Node->SetAsChecked();
	}
}

void APathFinder::CheckNeighbor_Implementation(APathNode *Current, FTile Direction, FTile Goal, int32 BreakTie = 0)
{
	FTile Neighbor = Direction + Current->Tile;

	// Return if neighbor is invalid
	if (!GetNode(Neighbor)) return;

	// Calculate cost
	int32 cost = Current->Cost + ETileStatus::TS_Status_MAX + 1 - GetStatusByTile(Neighbor);

	// Neighbor's cost should never be less than parent's
	if (Current->Cost >= cost) return;

	// Weigh based on direction (is direction in diagonal arrays)
	if (HeavyDiagonals)
	{
		if (DiagonalDirections2D.Contains(Direction.ToRC()) || DiagonalDirections3D.Contains(Direction.ToRC()))
		{
			cost = FMath::RoundToInt(cost * 2.41);
		}
	}

	// Weigh cost if punishing direction changes
	if (PunishDirectionChanges)
	{
		FTile TileA = Current->Tile - Current->ParentTile;
		if (TileA.ToRC().Row != Direction.ToRC().Row) cost += 20;
		if (TileA.ToRC().Column != Direction.ToRC().Column) cost += 20;
	}

	// Calculate Heuristic
	int32 Heu;
	FGCoord RC = (Neighbor - Goal).ToRC();
	RC.Row = abs(RC.Row);
	RC.Column = abs(RC.Column);
	Heu = 0;
	switch (HeuristicFormula)
	{
	case EHeuFormula::Manhatan:
		Heu = HeuristicEstimate * (RC.Row + RC.Column);
		break;
	case EHeuFormula::MaxDXDY:
		Heu = RC.Row > RC.Column ? RC.Row : RC.Column;
		break;
	case EHeuFormula::Diagonal:
		Heu = RC.Row < RC.Column ? RC.Row : RC.Column;
		Heu = (Heu * 2 * HeuristicEstimate) + (HeuristicEstimate * (RC.Row + RC.Column - (Heu * 2)));
		break;
	case EHeuFormula::Euclidean:
		Heu = FMath::Sqrt((RC.Row ^ 2) + (RC.Column ^ 2)) * HeuristicEstimate;
		break;
	case EHeuFormula::EucNoSQRT:
		Heu = ((RC.Row ^ 2) + (RC.Column ^ 2)) * HeuristicEstimate;
		break;
	case EHeuFormula::Heu_MAX:
	default:
		break;
	}

	// Apply Tie Breaker
	Heu += BreakTie;

	// Add to open, sort open
	auto Node = GetNode(Neighbor);
	Node->SetNodeValues(cost, Heu, Current->Tile, Current->Step + 1);
	OpenNodes.Add(Node);
}

bool APathFinder::Route_Implementation(FTile End, TArray<FTile> &RouteArray)
{
	RouteArray.Empty();
	RouteArray.Reserve(50);

	auto Node = GetNode(End);
	bool stop = false, found = false;

	Node->SetAsEnd();

	while (!found && RouteArray.Num() < 50)
	{
		// Stop if parent isn't valid
		if (!GetNode(Node->ParentTile)) break;

		// Debug Display
		// TODO::TODO Draw line to parent, possibly set as vertices for spline

		// Parent calling loop
		if (Node->ParentTile != Node->Tile)
		{
			RouteArray.Add(Node->Tile);
			Node = GetNode(Node->ParentTile);
			Node->SetAsPath();
		}
		else
		{
			Node->SetAsStart();
			found = true;
		}
	}

	return found;
}

bool APathFinder::DirectRoute_Implementation(FTile Start, FTile End, TArray<FTile> &RouteArray, TArray<FTile> &PathableTiles)
{
	Destination = End;
	Discovery(Start, 0, PathableTiles);
	return Route(End, RouteArray);
}

APathNode* APathFinder::GetNode(FTile Tile)
{
	auto pp = NodeMap.FindByPredicate([=](APathNode* Node) { return *Node == Tile; });
	if (!pp || !(*pp)) return nullptr;
	return *pp;
}

void APathFinder::ResetNodes()
{
	for (auto Node : NodeMap)
	{
		Node->Reset();

		if (DebugMode) Node->EnableDebugMode();
	}
}

void APathFinder::ClearQueue()
{
	ClosedNodes.Empty();
	OpenNodes.Empty();
	ResetNodes();
	NodeStat = 0;
}

ETileStatus APathFinder::GetStatusByTile(FTile InTile)
{
	if (!GetNode(InTile)) return ETileStatus::TS_Invalid;
	if (BlockedNodes.Contains(InTile.ToRC())) return ETileStatus::TS_Blocked;
	if (GetActorAtTile(GetWorld(), InTile) != nullptr) return ETileStatus::TS_Obstructed;
	return ETileStatus::TS_Open;
}

void APathFinder::UpdateStatus(FTile InTile, ETileStatus Status)
{
	if (Status > ETileStatus::TS_Blocked) BlockedNodes.Remove(InTile.ToRC());		// TODO: I Think there's an error here
	if (GetNode(InTile)) BlockedNodes.AddUnique(InTile.ToRC());
}

TArray<FTile> APathFinder::GetReachableTiles(bool IncludeObstructed)
{
	TArray<FTile> Tiles;
	for (auto Node : ClosedNodes)
	{
		if (IncludeObstructed) Tiles.Add(Node->Tile);
		else if (GetStatusByTile(Node->Tile) > ETileStatus::TS_Obstructed) Tiles.Add(Node->Tile);
	}

	return Tiles;
}

// Called when the game starts or when spawned
void APathFinder::BeginPlay()
{
	Super::BeginPlay();

}

