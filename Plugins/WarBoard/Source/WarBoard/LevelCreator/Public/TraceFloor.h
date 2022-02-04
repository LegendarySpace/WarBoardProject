// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Public/BiomeManager.h"
#include "../Public/Tiles.h"

#include "TraceFloor.generated.h"

/**
 * 
 */
UCLASS()
class WARBOARD_API UTraceFloor : public UBiomeManager
{
	GENERATED_BODY()
	
public:
	UTraceFloor();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|LevelCreator")
	void CreateFloor(int32 InSize, FGCoord InOrigin);
	void CreateFloor(int32 InSize, FTile InOrigin = FTile());
	void CreateFloor(int32 InSize, FCubic InOrigin);

	FTransform CalculateFloorTransform(FTile Tile);

	void BuildFloor(FTile Tile);

	void RebuildFloor();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|LevelCreator")
	void ShiftFloor(FGCoord Hover);
	void ShiftFloor(FTile Hover);
	void ShiftFloor(FCubic Hover);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|LevelCreator")
	void ShiftLevel(int32 InLevel);

public:
	int32 Size = 0;
	FTile Origin = FTile();
	int32 Level = 0;
	float LevelHeight = 100;		// Placeholder value since tile doesn't have Z dimension component currently
};
