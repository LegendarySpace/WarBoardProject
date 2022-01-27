// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "HeuristicFormula.h"
#include "Tiles.h"
#include "PathNode.h"

#include "NavSystemComponent.generated.h"

/**
*
*/
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WARBOARD_API UNavSystem : public USceneComponent		// UPGRADE: Make Actor Component after Node is modified to struct since it will have no world location
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UNavSystem();


	/**									**\
	*			Automated calls			  *
	*		These calls should only		  *
	*	   be made by the owning obj	  *
	\**									**/

	void DetermineDirections();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void Populate(TArray<FGCoord> Locations);
	void Populate(TArray<FTile> Locations);

	bool AddNode(FGCoord InTile);

	bool RemoveNode(FGCoord InTile);


	/**										**\
	*				API calls				  *
	*		Handles actual pathfinding		  *
	\**										**/

	// Resets all nodes and determine paths
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing")
	void Discovery(FGCoord Origin, int32 Range, TArray<FGCoord> &PathableTiles);

	// UPDATE: change from UFUNCTION
	// Looks at node in given direction and attempts to set data and add to open
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing")
	void CheckNeighbor(APathNode *Current, FGCoord Direction, FGCoord Goal, int32 BreakTie);

	// List of indexes reached by Discovery
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	TArray<FGCoord> GetReachableTiles(bool IncludeObstructed);

	// Returns the shourtest path to destination
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing", meta =(AutoCreateRefTerm = "RouteArray"))
	bool Route(FGCoord End, TArray<FGCoord> &RouteArray);

	// Returns the shourtest path to destination without utilizing Discovery
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarBoard|Pathing", meta = (AutoCreateRefTerm = "RouteArray, PathableTiles"))
	bool DirectRoute(FGCoord Start, FGCoord End, TArray<FGCoord> &RouteArray, TArray<FGCoord> &PathableTiles);


	/**							**\
	*		Control calls		  *
	\**							**/


	UFUNCTION(BlueprintCallable, Category = "WarBoard|Path")
	APathNode* GetNode(FGCoord Tile);

	// Reset all nodes
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void ResetNodes();

	// Clear node queue
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void ClearQueue();

	// Tile Status of a given Index
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	ENodeStatus GetStatusByTile(FGCoord InTile);

	// Update Status of given Index
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void UpdateStatus(FGCoord InTile, ENodeStatus Status);

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
	TArray<APathNode*> OpenNodes;

	// All nodes that have already been checked
	UPROPERTY(BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<APathNode*> ClosedNodes;

	// All nodes that are blocked by a structure or have impassable terrain
	UPROPERTY(BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FGCoord> BlockedNodes;

	// Map of all created nodes and their indexes. Cannot edit, editing is handled by tile creation/destruction
	UPROPERTY(BlueprintReadOnly, Category = "Pathing|Arrays")
	TArray<APathNode*> NodeMap;

	// All valid single step movement as relative index. Cardinal directions are assumed valid, may change later
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pathing|Arrays")
	TArray<FGCoord> ValidDirections;

	// Cardinal 2D directional movement as index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FGCoord> CardinalDirections2D;

	// Diagonal 2D directional movement as index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FGCoord> DiagonalDirections2D;

	// Cardinal 3D directional movement as index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FGCoord> CardinalDirections3D;

	// Diagonal 3D directional movement as index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing|Arrays")
	TArray<FGCoord> DiagonalDirections3D;


	/**							**\
	*		Conditional use		  *
	\**							**/


	// Used if destination is known as discovery starts, ignored if unknown
	TOptional<FTile> Destination;

};
