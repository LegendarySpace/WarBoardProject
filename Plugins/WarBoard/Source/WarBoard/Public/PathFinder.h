// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "HeuristicFormula.h"
#include "TileStatus.h"
#include "HelperStructs.h"

#include "PathFinder.generated.h"

class APathNode;

UCLASS()
class WARBOARD_API APathFinder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathFinder();


	/**									**\
	*			Automated calls			  *
	*		These calls should only		  *
	*	   be made by the owning obj	  *
	\**									**/


	// Make a node for each tile
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void Initialization(TArray<FTile> Locations);

	// Add Node
	bool Add(FTile InTile);

	// Remove Node
	bool Remove(FTile InTile);


	/**										**\
	*				API calls				  *
	*		Handles actual pathfinding		  *
	\**										**/


	// Resets all nodes and determine paths
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing", meta = (AutoCreateRefTerm = "PathableTiles"))
	void Discovery(FTile Origin, int32 Range, TArray<FTile> &PathableTiles);

	// Looks at node in given direction and attempts to set data and add to open
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing")
	void CheckNeighbor(APathNode *Current, FTile Direction, FTile Goal, int32 BreakTie);

	// Returns the shourtest path to destination
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing", meta =(AutoCreateRefTerm = "RouteArray"))
	bool Route(FTile End, TArray<FTile> &RouteArray);

	// Returns the shourtest path to destination
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing", meta = (AutoCreateRefTerm = "RouteArray, PathableTiles"))
	bool DirectRoute(FTile Start, FTile End, TArray<FTile> &RouteArray, TArray<FTile> &PathableTiles);


	/**							**\
	*		Control calls		  *
	\**							**/


	UFUNCTION(BlueprintCallable, Category = "WarBoard|Path")
	APathNode* GetNode(FTile Tile);

	// Reset all nodes
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void ResetNodes();

	// Clear node queue
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void ClearQueue();

	// Tile Status of a given Index
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	ETileStatus GetStatusByTile(FTile InTile);

	// Update Status of given Index
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void UpdateStatus(FTile InTile, ETileStatus Status);

	// List of indexes reached by Discovery
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	TArray<FTile> GetReachableTiles(bool IncludeObstructed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/**							**\
	*		Path Settings		  *
	\**							**/
	
	
	// Can paths move diagonally?
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool AllowDiagonals;

	// Can paths move vertically
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool Allow3DMovement;

	// Should route minimize directional change?
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool PunishDirectionChanges;

	// Discourage yet still allow diagonal movement
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool HeavyDiagonals;

	// Add additional calculations to reduce the likelihood of ties
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool TieBreaker;

	// Enter Debug Mode
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool DebugMode;

	// Display all potential paths, used for debugging
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool DisplayAllPaths;

	// Display stats, used for debugging
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	bool DisplayNodeStats;

	// Current stat being displayed
	UPROPERTY(BlueprintReadWrite, Category = "Pathing|Settings")
	int32 NodeStat;

	// Base Heuristic Value
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	int32 HeuristicEstimate;

	// Maximum nodes to search
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	int32 SearchLimit;

	// Formula to use for calculation
	UPROPERTY(EditAnywhere, Category = "Pathing|Settings")
	EHeuFormula HeuristicFormula;


protected:
	/**							**\
	*		Internal Lists		  *
	\**							**/


	// All nodes that still need to be checked
	UPROPERTY(BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<APathNode*> OpenNodes;

	// All nodes that have already been checked
	UPROPERTY(BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<APathNode*> ClosedNodes;

	// All nodes that are blocked by a structure or have impassable terrain
	UPROPERTY(BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FTile> BlockedNodes;

	// Map of all created nodes and their indexes. Cannot edit, editing is handled by tile creation/destruction
	UPROPERTY(BlueprintReadOnly, Category = "Pathing|Arrays")
	TArray<APathNode*> NodeMap;

	// All valid single step movement as relative index. Cardinal directions are assumed valid, may change later
	UPROPERTY(BlueprintReadOnly, Category = "Pathing|Arrays")
	TArray<FTile> ValidDirections;

	// Cardinal 2D directional movement as index
	UPROPERTY(EditAnywhere, Category = "Pathing|Arrays")
	TArray<FTile> CardinalDirections2D;

	// Diagonal 2D directional movement as index
	UPROPERTY(EditAnywhere, Category = "Pathing|Arrays")
	TArray<FTile> DiagonalDirections2D;

	// Cardinal 3D directional movement as index
	UPROPERTY(EditAnywhere, Category = "Pathing|Arrays")
	TArray<FTile> CardinalDirections3D;

	// Diagonal 3D directional movement as index
	UPROPERTY(EditAnywhere, Category = "Pathing|Arrays")
	TArray<FTile> DiagonalDirections3D;


	/**							**\
	*		Conditional use		  *
	\**							**/


	// Used if destination is known as discovery starts, ignored if unknown
	TOptional<FTile> Destination;

};
