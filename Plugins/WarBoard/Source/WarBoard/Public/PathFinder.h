// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeuristicFormula.h"
#include "TileStatus.h"
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
	void Initialization(TArray<int32> Locations);

	// Add Node
	bool Add(int32 Index);

	// Remove Node
	bool Remove(int32 Index);


	/**										**\
	*				API calls				  *
	*		Handles actual pathfinding		  *
	\**										**/


	// Resets all nodes and determine paths
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing", meta = (AutoCreateRefTerm = "PathableTiles"))
	void Discovery(int32 Origin, int32 Range, TArray<int32> &PathableTiles);

	// Looks at node in given direction and attempts to set data and add to open
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing")
	void CheckNeighbor(APathNode *Current, int32 Direction, int32 Goal, int32 BreakTie);

	// Returns the shourtest path to destination
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing", meta =(AutoCreateRefTerm = "RouteArray"))
	bool Route(int32 End, TArray<int32> &RouteArray);

	// Returns the shourtest path to destination
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing", meta = (AutoCreateRefTerm = "RouteArray, PathableTiles"))
	bool DirectRoute(int32 Start, int32 End, TArray<int32> &RouteArray, TArray<int32> &PathableTiles);


	/**							**\
	*		Control calls		  *
	\**							**/

	
	// Reset all nodes
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void ResetNodes();

	// Clear node queue
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void ClearQueue();

	// Tile Status of a given Index
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	ETileStatus GetStatusByIndex(int32 Index);

	// Update Status of given Index
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void UpdateStatus(int32 Index, ETileStatus Status);

	// List of indexes reached by Discovery
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	TArray<int32> GetReachableTiles(bool IncludeObstructed);

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
	TArray<int32> BlockedNodes;

	// Map of all created nodes and their indexes. Cannot edit, editing is handled by tile creation/destruction
	UPROPERTY(BlueprintReadOnly, Category = "Pathing|Arrays")
	TMap<int32, APathNode*> NodeMap;

	// All valid single step movement as relative index. Cardinal directions are assumed valid, may change later
	UPROPERTY(BlueprintReadOnly, Category = "Pathing|Arrays")
	TArray<int32> ValidDirections;

	// Cardinal 2D directional movement as index
	UPROPERTY(EditAnywhere, Category = "Pathing|Arrays")
	TArray<int32> CardinalDirections2D;

	// Diagonal 2D directional movement as index
	UPROPERTY(EditAnywhere, Category = "Pathing|Arrays")
	TArray<int32> DiagonalDirections2D;

	// Cardinal 3D directional movement as index
	UPROPERTY(EditAnywhere, Category = "Pathing|Arrays")
	TArray<int32> CardinalDirections3D;

	// Diagonal 3D directional movement as index
	UPROPERTY(EditAnywhere, Category = "Pathing|Arrays")
	TArray<int32> DiagonalDirections3D;


	/**							**\
	*		Conditional use		  *
	\**							**/


	// Used if destination is known as discovery starts, ignored if unknown
	int32 Destination;

};
