// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "HeuristicFormula.h"
#include "Tiles.h"
#include "PathNode.h"

#include "GridNavigationSystemComponent.generated.h"

/**
*
*/
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WARBOARD_API UGridNavigationSystemComponent : public USceneComponent		// UPGRADE: Make Actor Component instead of scene after Node is modified to struct since it will have no world location
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UGridNavigationSystemComponent();


	/**									**\
	*			Automated calls			  *
	*		These calls should only		  *
	*	   be made by the owning obj	  *
	\**									**/

	void DetermineDirections();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void Populate(TArray<FOrtho> Locations);
	void Populate(TArray<FTile> Locations);
	void Populate(TArray<FCubic> Locations);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	bool AddNode(FOrtho InTile) { return AddNode(FTile(InTile)); }
	bool AddNode(FTile InTile);
	bool AddNode(FCubic InTile) { return AddNode(FTile(InTile)); }

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	bool RemoveNode(FOrtho InTile) { return RemoveNode(FTile(InTile)); }
	bool RemoveNode(FTile InTile);
	bool RemoveNode(FCubic InTile) { return RemoveNode(FTile(InTile)); }


	/**										**\
	*				API calls				  *
	*		Handles actual pathfinding		  *
	\**										**/

	// Resets all nodes and determine paths
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing")
	void Discovery(FOrtho Origin, int32 Range, TArray<FOrtho>& PathableTiles);
	virtual void BeginDiscovery(FOrtho Origin, int32 Range, TArray<FOrtho> PathableTiles);
	virtual void BeginDiscovery(FTile Origin, int32 Range, TArray<FTile> PathableTiles);
	virtual void BeginDiscovery(FCubic Origin, int32 Range, TArray<FCubic> PathableTiles);

	// UPDATE: change from UFUNCTION
	// Looks at node in given direction and attempts to set data and add to open
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing")
	void CheckNeighbor(FPathNode Current, FOrtho Direction, FOrtho Goal, int32 BreakTie);
	virtual void CheckNeighborStatus(FPathNode Current, FOrtho Direction, FOrtho Goal, int32 BreakTie);
	virtual void CheckNeighborStatus(FPathNode Current, FTile Direction, FTile Goal, int32 BreakTie);
	virtual void CheckNeighborStatus(FPathNode Current, FCubic Direction, FCubic Goal, int32 BreakTie);

	// List of indexes reached by Discovery
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	TArray<FOrtho> GetReachableTiles(bool IncludeObstructed);

	// Returns the shourtest path to destination
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing", meta =(AutoCreateRefTerm = "RouteArray"))
	bool Route(FOrtho End, TArray<FOrtho> &RouteArray);
	virtual bool GetRoute(FOrtho End, TArray<FOrtho>& RouteArray);
	virtual bool GetRoute(FTile End, TArray<FTile>& RouteArray);
	virtual bool GetRoute(FCubic End, TArray<FCubic>& RouteArray);

	// Returns the shourtest path to destination without utilizing Discovery
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing", meta = (AutoCreateRefTerm = "RouteArray, PathableTiles"))
	bool DirectRoute(FOrtho Start, FOrtho End, TArray<FOrtho> &PathableTiles, TArray<FOrtho> &RouteArray);
	virtual bool GetDirectRoute(FOrtho Start, FOrtho End, TArray<FOrtho> PathableTiles, TArray<FOrtho>& RouteArray);
	virtual bool GetDirectRoute(FTile Start, FTile End, TArray<FTile> PathableTiles, TArray<FTile>& RouteArray);
	virtual bool GetDirectRoute(FCubic Start, FCubic End, TArray<FCubic> PathableTiles, TArray<FCubic>& RouteArray);


	/**							**\
	*		Control calls		  *
	\**							**/


	UFUNCTION(BlueprintCallable, Category = "WarBoard|Path")
	bool GetNodeFromCoord(FOrtho Tile, FPathNode &Node);
	virtual bool GetNode(FOrtho Tile, FPathNode &Node);
	virtual bool GetNode(FTile Tile, FPathNode &Node);
	virtual bool GetNode(TOptional<FTile> Tile, FPathNode &Node);
	virtual bool GetNode(FCubic Tile, FPathNode &Node);

	// Reset all nodes
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void ResetNodes();

	// Clear node queue
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void ClearQueue();

	// Tile Status of a given Index
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	ENodeStatus GetStatusByCoord(FOrtho InTile);
	virtual ENodeStatus GetStatusByTile(FOrtho InTile);
	virtual ENodeStatus GetStatusByTile(FTile InTile);
	virtual ENodeStatus GetStatusByTile(TOptional<FTile> InTile);
	virtual ENodeStatus GetStatusByTile(FCubic InTile);

	// Update Status of given Index
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void UpdateStatusByCoord(FOrtho InTile, ENodeStatus Status);
	virtual void UpdateStatus(FOrtho InTile, ENodeStatus Status);
	virtual void UpdateStatus(FTile InTile, ENodeStatus Status);
	virtual void UpdateStatus(FCubic InTile, ENodeStatus Status);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/**							**\
	*		Path Settings		  *
	\**							**/
	
	
	// Can paths move diagonally?
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool AllowDiagonals = false;

	// Can paths move vertically
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool Allow3DMovement = false;

	// Should route minimize directional change?
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool PunishDirectionChanges = false;

	// Discourage yet still allow diagonal movement
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool HeavyDiagonals = false;

	// Add additional calculations to reduce the likelihood of ties
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool TieBreaker = false;

	// Enter Debug Mode
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool DebugMode = false;

	// Display all potential paths, used for debugging
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool DisplayAllPaths = false;

	// Display stats, used for debugging
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool DisplayNodeStats = false;

	// Current stat being displayed
	UPROPERTY(BlueprintReadWrite, Category = "Pathing|Settings")
	int32 NodeStat = 2;

	// Base Heuristic Value
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	int32 HeuristicEstimate = 2;

	// Maximum nodes to search
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	int32 SearchLimit = 300;

	// Formula to use for calculation
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	EHeuFormula HeuristicFormula = EHeuFormula::Manhatan;


protected:
	/**							**\
	*		Internal Lists		  *
	\**							**/


	// All nodes that still need to be checked
	UPROPERTY(BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FPathNode> OpenNodes;

	// All nodes that have already been checked
	UPROPERTY(BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FPathNode> ClosedNodes;

	// All nodes that are blocked by a structure or have impassable terrain
	UPROPERTY(BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FOrtho> BlockedNodes;

	// Map of all created nodes and their indexes. Cannot edit, editing is handled by tile creation/destruction
	UPROPERTY(BlueprintReadOnly, Category = "Pathing|Arrays")
	TArray<FPathNode> NodeMap;

	// All valid single step movement as relative index. Cardinal directions are assumed valid, may change later
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pathing|Arrays")
	TArray<FOrtho> ValidDirections;

	// Cardinal 2D directional movement as index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FOrtho> CardinalDirections2D;

	// Diagonal 2D directional movement as index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FOrtho> DiagonalDirections2D;

	// Cardinal 3D directional movement as index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FOrtho> CardinalDirections3D;

	// Diagonal 3D directional movement as index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FOrtho> DiagonalDirections3D;


	/**							**\
	*		Conditional use		  *
	\**							**/


	// Used if destination is known as discovery starts, ignored if unknown
	TOptional<FTile> Destination;

};
