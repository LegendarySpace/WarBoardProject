// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarBoardLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WARBOARD_API UWarBoardLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	*		Conversion functions
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "ITW", Category = "Conversion"))
	bool IndexToWorld(int32 Index, bool TileCenter, FVector &Location);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "ITT", Category = "Conversion"))
	void IndexToTile(int32 Index, int32 &Row, int32 &Col);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTW", Category = "Conversion"))
	bool TileToWorld(int32 Row, int32 Col, bool TileCenter, FVector &Location);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTI", Category = "Conversion"))
	void TileToIndex(int32 Row, int32 Col, int32 &Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTW", Category = "Conversion"))
	bool WorldToIndex(FVector Location, int32 &Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTW", Category = "Conversion"))
	bool WorldToTile(FVector Location, int32 &Row, int32 &Col);
	
	/**
	*		Validation functions
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Valid?", Category = "Validation"))
	bool TileValid(int32 Row, int32 Col);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Valid?", Category = "Validation"))
	bool IndexValid(int32 Index);

	/**
	*		Utility functions
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Tiles", Category = "Utility"))
	TArray<int32> GetTileIndexes();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Obstructed", Category = "Utility"))
	TArray<int32> GetAllObstructedIndexes();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Passable?", Category = "Utility"))
	bool GetIndexPassable(int32 Index);

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "TileArray", Category = "Utility"))
	TArray<int32> GetTileArray(int32 MinRange, int32 MaxRange, int Shape);

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "ValidatedTiles", Category = "Utility"))
	bool GetValidatedTileArray(int32 Origin, int32 MinRange, int32 MaxRange, int Shape, TArray<int32> &Validated);

	UFUNCTION(BlueprintCallable, meta = (Category = "Utility"))
	void CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector> &Vertices, TArray<int32> &Triangles);

protected:
	float TileSize = 200;
	int32 MaxWidth = 6500;
	int32 Offset = 0;
	bool BoardCentered = true;
	FVector BoardLocation = FVector(0.f);

	void initialize();
};
