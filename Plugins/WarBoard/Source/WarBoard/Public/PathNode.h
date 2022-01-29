// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Tiles.h"

#include "PathNode.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

/**
 *  Usage status of node
 */
UENUM(BlueprintType, Category = "WarBoard|Enums")
enum ENodeStatus
{
	NS_Invalid = -1		UMETA(DisplayName = "Invalid"),
	NS_Blocked = 0			UMETA(DisplayName = "Blocked"),
	NS_Obstructed = 1			UMETA(DisplayName = "Obstructed"),
	NS_Open						UMETA(DisplayName = "Open"),
	NS_Status_MAX				UMETA(Hidden)
};

/**
 *	Path status of node
 */
UENUM(BlueprintType, Category = "WarBoard|Enums")
enum class ENavStatus : uint8
{
	Ignored			UMETA(DisplayName = "Ignored"),
	Unchecked		UMETA(DisplayName = "Unchecked"),
	Checked			UMETA(DisplayName = "Checked"),
	Start			UMETA(DisplayName = "Start"),
	End				UMETA(DisplayName = "End"),
	Path			UMETA(DisplayName = "Path"),
	Status_MAX		UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FPathNode
{
	GENERATED_BODY()

	void Reset();
	void SetNodeValues(int32 TravelCost, int32 Heuristic, FTile Parent, int32 Steps);
	void SetAsPath() { NavStatus = ENavStatus::Path; }
	void SetAsStart() { NavStatus = ENavStatus::Start; }
	void SetAsEnd() { NavStatus = ENavStatus::End; }
	void SetAsChecked() { NavStatus = ENavStatus::Checked; }

	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
	ENavStatus NavStatus = ENavStatus::Ignored;

	// Estimated cost to destination
	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
	int32 F = 0;

	// Total cost to reach destination
	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
	int32 Cost = 0;

	// A measure of distance from Origin
	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
	int32 Heu = 0;

	// Index of this node
	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
	FTile Tile;

	// Index of node to parent
	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
	FTile ParentTile;

	// Number of steps from Origin
	UPROPERTY(BlueprintReadWrite, Category = "WarBoard|Path")
	int32 Step = 0;

	// Overload Operators
	FORCEINLINE bool operator== (const FPathNode Node)
	{
		return F == Node.F || Tile == Node.Tile;
	}

	FORCEINLINE bool operator== (const int32 Index)
	{
		return Tile == FTile(Index);
	}

	// Need to make it for pointer
	FORCEINLINE bool operator== (const FTile InTile)
	{
		return Tile == InTile;
	}

	FORCEINLINE bool operator== (const TOptional<FTile> InTile)
	{
		return InTile.IsSet() && Tile == InTile.GetValue();
	}

	FORCEINLINE bool operator!= (const FPathNode Node)
	{
		return F != Node.F;
	}

	FORCEINLINE bool operator> (const FPathNode Node)
	{
		return F > Node.F;
	}

	FORCEINLINE bool operator>= (const FPathNode Node)
	{
		return F >= Node.F;
	}

	FORCEINLINE bool operator< (const FPathNode Node)
	{
		return F < Node.F;
	}

	FORCEINLINE bool operator<= (const FPathNode Node)
	{
		return F <= Node.F;
	}

};

