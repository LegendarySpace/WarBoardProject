// Fill out your copyright notice in the Description page of Project Settings.


#include "NavSystemComponent.h"

#include "Math/NumericLimits.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;

// Sets default values
UNavSystem::UNavSystem()
{
 	// Never Tick
	PrimaryComponentTick.bCanEverTick = false;

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
	// UPDATE: GetNeighbors(FTile Origin) Calculate neighbors at runtime based on origin and shape, return list of neighbors, validdirections can be removed

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

void UNavSystem::DetermineDirections()
{
	ValidDirections.Append(CardinalDirections2D);
	if (AllowDiagonals) ValidDirections.Append(DiagonalDirections2D);
	if (Allow3DMovement)
	{
		ValidDirections.Append(CardinalDirections3D);
		if (AllowDiagonals) ValidDirections.Append(DiagonalDirections3D);
	}

}

void UNavSystem::Populate(TArray<FGCoord> Locations)
{
	DetermineDirections();

	for (auto Coord : Locations)
	{
		AddNode(Coord);
	}
}

void UNavSystem::Populate(TArray<FTile> Locations)
{
	DetermineDirections();

	for (auto Tile : Locations)
	{
		AddNode(Tile.ToRC());
	}
}

bool UNavSystem::AddNode(FGCoord InTile)
{
	FTile T = InTile;
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto Node = GetWorld()->SpawnActor<APathNode>(T.ToWorld(), FRotator(), Params);
	if (!Node) return false; 
	if (!Node->GetActorLocation().Equals(T.ToWorld())) Node->SetActorLocation(T.ToWorld());
	NodeMap.Add(Node);
	return true;
}

bool UNavSystem::RemoveNode(FGCoord InTile)
{
	auto Node = GetNode(InTile);
	Node->Destroy();
	NodeMap.Remove(Node);
	return true;
}

void UNavSystem::Discovery_Implementation(FGCoord Origin, int32 Range, TArray<FGCoord> &PathableTiles)
{
	if (Range < 1) Range = NodeMap.Num();

	bool stopped = false;

	ClearQueue();

	auto Node = GetNode(Origin);
	if (!Node) return;
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

		if (SearchLimit < ClosedNodes.Num()) break;

		// If node is Origin or within range and not obstructed check neighbors
		if (*Node == Origin || (GetStatusByTile(Node->Tile.ToRC()) > ENodeStatus::NS_Obstructed && Node->Step < Range))
		{
			for (auto Direction : ValidDirections)
			{
				FTile Neighbor = Node->Tile + Direction;
				// Skip if pathable tiles and neighbor not amoung them, rename pathable to pathrestrictions
				if (PathableTiles.Num() > 0 && !PathableTiles.Contains(Neighbor.ToRC())) continue;

				if (GetStatusByTile(Neighbor.ToRC()) < ENodeStatus::NS_Obstructed) continue;

				auto pred = [=](APathNode* Node) { return *Node == Neighbor; };
				if (OpenNodes.FindByPredicate(pred) || ClosedNodes.FindByPredicate(pred)) continue;

				// If Destination not valid calculate end point
				FTile end;
				if (Destination.IsSet() && GetNode(Destination.GetValue().ToRC())) end = Destination.GetValue();
				else end = FTile(Direction).ToWorld().GetSafeNormal() * (Range + 1) * WarBoardLib::GetTileSize();

				// Calculate Tie Breaker here, Origin is not available where tiebreaker is used
				FTile TileA = Node->Tile - end;
				FTile TileB = FTile(Origin) - end;
				int32 tb = TieBreaker ? FMath::Abs((TileA.ToRC().Row * TileB.ToRC().Column) - (TileB.ToRC().Row * TileA.ToRC().Column)) * .001 : 0;

				// Call CheckNeighbor
				CheckNeighbor(Node, Direction, end.ToRC(), tb);
			}

		}

		// Add to closed and set node checked
		ClosedNodes.Add(Node);
		Node->SetAsChecked();
	}
}

void UNavSystem::CheckNeighbor_Implementation(APathNode *Current, FGCoord Direction, FGCoord Goal, int32 BreakTie = 0)
{
	FGCoord Neighbor = Direction + Current->Tile.ToRC();

	if (!GetNode(Neighbor)) return;

	// Calculate cost
	int32 cost = Current->Cost + ENodeStatus::NS_Status_MAX + 1 - GetStatusByTile(Neighbor);

	// Neighbor's cost should never be less than parent's
	if (Current->Cost >= cost) return;

	if (HeavyDiagonals)
	{
		if (DiagonalDirections2D.Contains(Direction) || DiagonalDirections3D.Contains(Direction))
		{
			cost = FMath::RoundToInt(cost * 2.41);
		}
	}

	if (PunishDirectionChanges)
	{
		FTile TileA = Current->Tile - Current->ParentTile;
		if (TileA.ToRC().Row != Direction.Row) cost += 20;
		if (TileA.ToRC().Column != Direction.Column) cost += 20;
	}

	int32 Heu;
	FGCoord RC = (Neighbor - Goal);
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

	auto Node = GetNode(Neighbor);
	Node->SetNodeValues(cost, Heu, Current->Tile, Current->Step + 1);
	OpenNodes.Add(Node);
}

bool UNavSystem::Route_Implementation(FGCoord End, TArray<FGCoord> &RouteArray)
{
	RouteArray.Empty();
	RouteArray.Reserve(50);

	auto Node = GetNode(End);
	bool stop = false, found = false;

	Node->SetAsEnd();

	while (!found && RouteArray.Num() < 50)
	{
		// Stop if parent isn't valid
		if (!GetNode(Node->ParentTile.ToRC())) break;

		// Debug Display
		// UPGRADE: Draw line to parent, possibly set as vertices for spline

		// Parent calling loop
		if (Node->ParentTile != Node->Tile)
		{
			RouteArray.Add(Node->Tile.ToRC());
			Node = GetNode(Node->ParentTile.ToRC());
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

bool UNavSystem::DirectRoute_Implementation(FGCoord Start, FGCoord End, TArray<FGCoord> &RouteArray, TArray<FGCoord>& PathableTiles)
{
	Destination = End;
	Discovery(Start, 0, PathableTiles);
	return Route(End, RouteArray);
}

APathNode* UNavSystem::GetNode(FGCoord Tile)
{
	FTile T = Tile;
	auto pp = NodeMap.FindByPredicate([=](APathNode* Node) { return *Node == T; });
	if (!pp || !(*pp)) return nullptr;
	return *pp;
}

void UNavSystem::ResetNodes()
{
	for (auto Node : NodeMap)
	{
		Node->Reset();

		if (DebugMode) Node->EnableDebugMode();
	}
}

void UNavSystem::ClearQueue()
{
	ClosedNodes.Empty();
	OpenNodes.Empty();
	ResetNodes();
	NodeStat = 0;
}

ENodeStatus UNavSystem::GetStatusByTile(FGCoord InTile)
{
	if (!GetNode(InTile)) return ENodeStatus::NS_Invalid;
	if (BlockedNodes.Contains(InTile)) return ENodeStatus::NS_Blocked;
	if (GetActorAtTile(GetWorld(), InTile) != nullptr) return ENodeStatus::NS_Obstructed;
	return ENodeStatus::NS_Open;
}

void UNavSystem::UpdateStatus(FGCoord InTile, ENodeStatus Status)
{
	if (Status > ENodeStatus::NS_Blocked) BlockedNodes.Remove(InTile);
	if (GetNode(InTile)) BlockedNodes.AddUnique(InTile);
}

TArray<FGCoord> UNavSystem::GetReachableTiles(bool IncludeObstructed)
{
	TArray<FGCoord> Tiles;
	for (auto Node : ClosedNodes)
	{
		if (IncludeObstructed) Tiles.Add(Node->Tile.ToRC());
		else if (GetStatusByTile(Node->Tile.ToRC()) > ENodeStatus::NS_Obstructed) Tiles.Add(Node->Tile.ToRC());
	}

	return Tiles;
}

// Called when the game starts or when spawned
void UNavSystem::BeginPlay()
{
	Super::BeginPlay();

}

