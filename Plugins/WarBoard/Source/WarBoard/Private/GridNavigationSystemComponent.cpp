// Fill out your copyright notice in the Description page of Project Settings.


#include "GridNavigationSystemComponent.h"

#include "Math/NumericLimits.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;

// Sets default values
UGridNavigationSystemComponent::UGridNavigationSystemComponent()
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

void UGridNavigationSystemComponent::DetermineDirections()
{
	// UPDATE: GetNeighbors(FTile Origin) Calculate neighbors at runtime based on origin and shape, return list of neighbors, validdirections can be removed
	ValidDirections.Append(CardinalDirections2D);
	if (AllowDiagonals) ValidDirections.Append(DiagonalDirections2D);
	if (Allow3DMovement)
	{
		ValidDirections.Append(CardinalDirections3D);
		if (AllowDiagonals) ValidDirections.Append(DiagonalDirections3D);
	}

}

void UGridNavigationSystemComponent::Populate(TArray<FGCoord> Locations)
{
	TArray<FTile> L;
	for (auto Coord : Locations) { L.Add(FTile(Coord)); }
	Populate(L);
}

void UGridNavigationSystemComponent::Populate(TArray<FTile> Locations)
{
	DetermineDirections();

	for (auto Tile : Locations)
	{
		AddNode(Tile);
	}
}

void UGridNavigationSystemComponent::Populate(TArray<FCubic> Locations)
{
	TArray<FTile> L;
	for (auto Coord : Locations) { L.Add(FTile(Coord)); }
	Populate(L);
}

bool UGridNavigationSystemComponent::AddNode(FTile InTile)
{
	FPathNode Node;
	Node.NavStatus = ENavStatus::Ignored;
	Node.Tile = InTile;
	Node.ParentTile = InTile;
	NodeMap.Add(Node);
	return true;

}

bool UGridNavigationSystemComponent::RemoveNode(FTile InTile)
{
	FPathNode Node;
	if (!GetNode(InTile, Node)) return false;
	NodeMap.Remove(Node);
	return true;
}

void UGridNavigationSystemComponent::Discovery_Implementation(FGCoord Origin, int32 Range, TArray<FGCoord> &PathableTiles)
{
	BeginDiscovery(Origin, Range, PathableTiles);
}

void UGridNavigationSystemComponent::BeginDiscovery(FGCoord Origin, int32 Range, TArray<FGCoord> PathableTiles)
{
	TArray<FGCoord> PT;
	if (PathableTiles.Num() > 0) { for (auto Tile : PathableTiles) PT.Add(Tile); }
	Discovery(Origin, Range, PT);
}

void UGridNavigationSystemComponent::BeginDiscovery(FTile Origin, int32 Range, TArray<FTile> PathableTiles)
{
	if (Range < 1) Range = NodeMap.Num();

	bool stopped = false;

	ClearQueue();

	FPathNode Node;
	if (!GetNode(Origin, Node)) return;
	Node.SetNodeValues(0, HeuristicEstimate, Origin, 0);
	OpenNodes.Push(Node);

	// Start primary loop and open.pop to get index of node, sort open and ensure node valid
	while (OpenNodes.Num() > 0 && !stopped)
	{
		OpenNodes.Sort([](FPathNode A, const FPathNode B) { return A > B; });
		Node = OpenNodes.Pop();

		// If node is Destination, Add to closed, set node checked and stopped is true
		if (Node == Destination)
		{
			ClosedNodes.Add(Node);
			Node.SetAsChecked();
			stopped = true;
		}

		if (SearchLimit < ClosedNodes.Num()) break;

		// If node is Origin or within range and not obstructed check neighbors
		if (Node == Origin || (GetStatusByTile(Node.Tile) > ENodeStatus::NS_Obstructed && Node.Step < Range))
		{
			for (auto Direction : ValidDirections)
			{
				FTile Neighbor = Node.Tile + Direction;
				// Skip if pathable tiles and neighbor not amoung them, rename pathable to pathrestrictions
				if (PathableTiles.Num() > 0 && !PathableTiles.Contains(Neighbor)) continue;

				if (GetStatusByTile(Neighbor) < ENodeStatus::NS_Obstructed) continue;

				auto pred = [=](FPathNode Node) { return Node == Neighbor; };
				if (OpenNodes.FindByPredicate(pred) || ClosedNodes.FindByPredicate(pred)) continue;

				// Debug Display
				// UPGRADE: Draw temp line to parent

				// If Destination not valid calculate end point
				FTile end;
				FPathNode N;
				if (GetNode(Destination, N)) end = Destination.GetValue();
				else end = FTile(Direction).ToWorld().GetSafeNormal() * (Range + 1) * WarBoardLib::GetTileSize();

				// Calculate Tie Breaker here, Origin is not available where tiebreaker is used
				FTile TileA = Node.Tile - end;
				FTile TileB = Origin - end;
				int32 tb = TieBreaker ? FMath::Abs((TileA.ToRC().Row * TileB.ToRC().Column) - (TileB.ToRC().Row * TileA.ToRC().Column)) * .001 : 0;

				// Call CheckNeighbor
				CheckNeighbor(Node, Direction, end.ToRC(), tb);
			}

		}

		// Add to closed and set node checked
		ClosedNodes.Add(Node);
		Node.SetAsChecked();
	}
}

void UGridNavigationSystemComponent::BeginDiscovery(FCubic Origin, int32 Range, TArray<FCubic> PathableTiles)
{
	TArray<FGCoord> PT;
	if (PathableTiles.Num() > 0) { for (auto Tile : PathableTiles) PT.Add(FTile(Tile).ToRC()); }
	Discovery(FTile(Origin).ToRC(), Range, PT);
}

void UGridNavigationSystemComponent::CheckNeighbor_Implementation(FPathNode Current, FGCoord Direction, FGCoord Goal, int32 BreakTie = 0)
{
	CheckNeighborStatus(Current, Direction, Goal, BreakTie);
}

void UGridNavigationSystemComponent::CheckNeighborStatus(FPathNode Current, FGCoord Direction, FGCoord Goal, int32 BreakTie = 0)
{
	CheckNeighborStatus(Current, FTile(Direction), Goal, BreakTie);
}

void UGridNavigationSystemComponent::CheckNeighborStatus(FPathNode Current, FTile Direction, FTile Goal, int32 BreakTie = 0)
{
	FTile Neighbor = Direction + Current.Tile;

	FPathNode N;
	if (!GetNode(Neighbor, N)) return;

	// Calculate cost
	int32 cost = Current.Cost + ENodeStatus::NS_Status_MAX + 1 - GetStatusByTile(Neighbor);

	// Neighbor's cost should never be less than parent's
	if (Current.Cost >= cost) return;

	if (HeavyDiagonals)
	{
		if (DiagonalDirections2D.Contains(Direction.ToRC()) || DiagonalDirections3D.Contains(Direction.ToRC()))
		{
			cost = FMath::RoundToInt(cost * 2.41);
		}
	}

	if (PunishDirectionChanges)
	{
		FTile TileA = Current.Tile - Current.ParentTile;
		if (TileA.ToRC().Row != Direction.ToRC().Row) cost += 20;
		if (TileA.ToRC().Column != Direction.ToRC().Column) cost += 20;
	}

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

	FPathNode Node;
	if (!GetNode(Neighbor, Node)) return;
	Node.SetNodeValues(cost, Heu, Current.Tile, Current.Step + 1);
	OpenNodes.Add(Node);
}

void UGridNavigationSystemComponent::CheckNeighborStatus(FPathNode Current, FCubic Direction, FCubic Goal, int32 BreakTie = 0)
{
	CheckNeighborStatus(Current, FTile(Direction), Goal, BreakTie);
}

bool UGridNavigationSystemComponent::Route_Implementation(FGCoord End, TArray<FGCoord>& RouteArray)
{
	return GetRoute(End, RouteArray);
}

bool UGridNavigationSystemComponent::GetRoute(FGCoord End, TArray<FGCoord>& RouteArray)
{
	TArray<FTile> RA;
	if (!GetRoute(FTile(End), RA)) return false;
	RouteArray.Empty();
	for (auto Tile : RA) RouteArray.Add(Tile.ToRC());
	return true;
}

bool UGridNavigationSystemComponent::GetRoute(FTile End, TArray<FTile> &RouteArray)
{
	RouteArray.Empty();
	FPathNode Node;
	if (!GetNode(End, Node)) return false;
	bool stop = false, found = false;

	Node.SetAsEnd();

	while (!found)
	{
		// Stop if parent isn't valid
		FPathNode tempNode;
		if (!GetNode(Node.ParentTile, tempNode)) break;

		// Parent calling loop
		if (Node.ParentTile != Node.Tile)
		{
			RouteArray.Add(Node.Tile);
			Node = tempNode;
			Node.SetAsPath();
		}
		else
		{
			Node.SetAsStart();
			found = true;
		}
	}

	// TODO: if (bDisplayPath) Set Spline from start to finish with vertices for each tile

	return found;
}

bool UGridNavigationSystemComponent::GetRoute(FCubic End, TArray<FCubic>& RouteArray)
{
	TArray<FTile> RA;
	if (!GetRoute(FTile(End), RA)) return false;
	RouteArray.Empty();
	for (auto Tile : RA) RouteArray.Add(Tile.ToCubic());
	return true;
}

bool UGridNavigationSystemComponent::DirectRoute_Implementation(FGCoord Start, FGCoord End, TArray<FGCoord> &PathableTiles, TArray<FGCoord>& RouteArray)
{
	return GetDirectRoute(Start, End, PathableTiles, RouteArray);
}

bool UGridNavigationSystemComponent::GetDirectRoute(FGCoord Start, FGCoord End, TArray<FGCoord> PathableTiles, TArray<FGCoord>& RouteArray)
{
	Destination = FTile(End);
	Discovery(Start, 0, PathableTiles);
	return Route(End, RouteArray);
}

bool UGridNavigationSystemComponent::GetDirectRoute(FTile Start, FTile End, TArray<FTile> PathableTiles, TArray<FTile>& RouteArray)
{
	TArray<FGCoord> PT, RA;
	if (PathableTiles.Num() > 0) { for (auto& Tile : PathableTiles) PT.Add(Tile.ToRC()); }
	if (!DirectRoute(Start.ToRC(), End.ToRC(), PT, RA)) return false;
	RouteArray.Empty();
	for (auto Tile : RA) RouteArray.Add(FTile(Tile));
	return true;

}

bool UGridNavigationSystemComponent::GetDirectRoute(FCubic Start, FCubic End, TArray<FCubic> PathableTiles, TArray<FCubic>& RouteArray)
{
	TArray<FGCoord> PT, RA;
	if (PathableTiles.Num() > 0) { for (auto &Tile : PathableTiles) PT.Add(FTile(Tile).ToRC()); }
	if (!DirectRoute(FTile(Start).ToRC(), FTile(End).ToRC(), PT, RA)) return false;
	RouteArray.Empty();
	for (auto Tile : RA) RouteArray.Add(FTile(Tile).ToCubic());
	return true;
}

bool UGridNavigationSystemComponent::GetNodeFromCoord(FGCoord Tile, FPathNode &Node)
{
	return GetNode(Tile, Node);
}

bool UGridNavigationSystemComponent::GetNode(FGCoord Tile, FPathNode& Node)
{
	return GetNode(FTile(Tile), Node);
}

bool UGridNavigationSystemComponent::GetNode(TOptional<FTile> Tile, FPathNode& Node)
{
	if (!Tile.IsSet()) return nullptr;
	return GetNode(Tile.GetValue(), Node);
}

bool UGridNavigationSystemComponent::GetNode(FTile Tile, FPathNode& Node)
{
	auto pNode = NodeMap.FindByPredicate([=](FPathNode Node) { return Node == Tile; });
	if (pNode == nullptr) return false;
	Node = *pNode;
	return true;
}

bool UGridNavigationSystemComponent::GetNode(FCubic Tile, FPathNode& Node)
{
	return GetNode(FTile(Tile), Node);
}

void UGridNavigationSystemComponent::ResetNodes()
{
	for (auto Node : NodeMap)
	{
		Node.Reset();

	}
}

void UGridNavigationSystemComponent::ClearQueue()
{
	ClosedNodes.Empty();
	OpenNodes.Empty();
	ResetNodes();
	NodeStat = 0;
}

ENodeStatus UGridNavigationSystemComponent::GetStatusByCoord(FGCoord InTile)
{
	return GetStatusByTile(InTile);
}

ENodeStatus UGridNavigationSystemComponent::GetStatusByTile(FGCoord InTile)
{
	return GetStatusByTile(FTile(InTile));
}

ENodeStatus UGridNavigationSystemComponent::GetStatusByTile(TOptional<FTile> InTile)
{
	return GetStatusByTile(InTile.GetValue());
}

ENodeStatus UGridNavigationSystemComponent::GetStatusByTile(FTile InTile)
{
	FPathNode N;
	if (!GetNode(InTile, N)) return ENodeStatus::NS_Invalid;
	if (BlockedNodes.Contains(InTile.ToRC())) return ENodeStatus::NS_Blocked;
	if (GetActorAtTile(GetWorld(), InTile) != nullptr) return ENodeStatus::NS_Obstructed;
	return ENodeStatus::NS_Open;
}

ENodeStatus UGridNavigationSystemComponent::GetStatusByTile(FCubic InTile)
{
	return GetStatusByTile(FTile(InTile));
}

void UGridNavigationSystemComponent::UpdateStatusByCoord(FGCoord InTile, ENodeStatus Status)
{
	UpdateStatus(InTile, Status);
}

void UGridNavigationSystemComponent::UpdateStatus(FGCoord InTile, ENodeStatus Status)
{
	FPathNode N;
	if (Status > ENodeStatus::NS_Blocked) BlockedNodes.Remove(InTile);
	if (GetNode(InTile, N)) BlockedNodes.AddUnique(InTile);
}

void UGridNavigationSystemComponent::UpdateStatus(FTile InTile, ENodeStatus Status)
{
	UpdateStatus(InTile.ToRC(), Status);
}

void UGridNavigationSystemComponent::UpdateStatus(FCubic InTile, ENodeStatus Status)
{
	UpdateStatus(FTile(InTile).ToRC(), Status);
}

TArray<FGCoord> UGridNavigationSystemComponent::GetReachableTiles(bool IncludeObstructed)
{
	TArray<FGCoord> Tiles;
	for (auto Node : ClosedNodes)
	{
		if (IncludeObstructed) Tiles.Add(Node.Tile.ToRC());
		else if (GetStatusByTile(Node.Tile) > ENodeStatus::NS_Obstructed) Tiles.Add(Node.Tile.ToRC());
	}

	return Tiles;
}

// Called when the game starts or when spawned
void UGridNavigationSystemComponent::BeginPlay()
{
	Super::BeginPlay();

}

