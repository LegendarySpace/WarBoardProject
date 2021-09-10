// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EGridShape.h"
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
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "ITW", Category = "WarBoard|Conversion"))
	static bool IndexToWorld(const int32 Index, const bool TileCenter, FVector &Location);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "ITT", Category = "WarBoard|Conversion"))
	static void IndexToTile(const int32 Index, int32 &Row, int32 &Col);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTW", Category = "WarBoard|Conversion"))
	static bool TileToWorld(const int32 Row, const int32 Col, const bool TileCenter, FVector &Location);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTI", Category = "WarBoard|Conversion"))
	static void TileToIndex(const int32 Row, const int32 Col, int32 &Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTW", Category = "WarBoard|Conversion"))
	static bool WorldToIndex(const FVector Location, int32 &Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTW", Category = "WarBoard|Conversion"))
	static bool WorldToTile(const FVector Location, int32 &Row, int32 &Col);
	
	/**
	*		Validation functions
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Valid?", Category = "WarBoard|Validation"))
	static bool TileValid(const int32 Row, const int32 Col);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Valid?", Category = "WarBoard|Validation"))
	static bool IndexValid(const int32 Index);

	/**
	*		Utility functions
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Tiles", Category = "WarBoard|Utility"))
	static TArray<int32> GetTileIndexes();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Obstructed", Category = "WarBoard|Utility"))
	static TArray<int32> GetAllObstructedIndexes();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Passable?", Category = "WarBoard|Utility"))
	static bool GetIndexPassable(const int32 Index);

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "TileArray", Category = "WarBoard|Utility"))
	static TArray<int32> GetTileArray(const int32 MinRange, const int32 MaxRange, const EGridShape Shape);

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "ValidatedTiles", Category = "WarBoard|Utility"))
	static bool GetValidatedTileArray(const int32 Origin, const int32 MinRange, const int32 MaxRange, const EGridShape Shape, TArray<int32> &Validated);

	UFUNCTION(BlueprintCallable, meta = (Category = "WarBoard|Utility"))
	static void CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector> &Vertices, TArray<int32> &Triangles);

public:
	static float TileSize;
	static int32 MaxWidth;
	static int32 Offset;
	static bool BoardCentered;
	static FVector BoardLocation;

	static void initialize();
};

namespace WarBoardLib
{
	// Make Static calls through namespace so its footprint can be reduced with 'using namespace'
	inline bool IndexToWorld(const int32 Index, const bool TileCenter, FVector &Location) { return UWarBoardLibrary::IndexToWorld(Index, TileCenter, Location); }
	inline void IndexToTile(const int32 Index, int32 &Row, int32 &Col) { return UWarBoardLibrary::IndexToTile(Index, Row, Col); }
	inline bool TileToWorld(const int32 Row, const int32 Col, const bool TileCenter, FVector &Location) { return UWarBoardLibrary::TileToWorld(Row, Col, TileCenter, Location); }
	inline void TileToIndex(const int32 Row, const int32 Col, int32 &Index) { return UWarBoardLibrary::TileToIndex(Row, Col, Index); }
	inline bool WorldToIndex(const FVector Location, int32 &Index) { return UWarBoardLibrary::WorldToIndex(Location, Index); }
	inline bool WorldToTile(const FVector Location, int32 &Row, int32 &Col) { return UWarBoardLibrary::WorldToTile(Location, Row, Col); }
	static bool TileValid(const int32 Row, const int32 Col) { return UWarBoardLibrary::TileValid(Row, Col); }
	static bool IndexValid(const int32 Index) { return UWarBoardLibrary::IndexValid(Index); }
	static TArray<int32> GetTileIndexes() { return UWarBoardLibrary::GetTileIndexes(); }
	static TArray<int32> GetAllObstructedIndexes() { return UWarBoardLibrary::GetAllObstructedIndexes(); }
	static bool GetIndexPassable(const int32 Index) { return UWarBoardLibrary::GetIndexPassable(Index); }
	static TArray<int32> GetTileArray(const int32 MinRange, const int32 MaxRange, const EGridShape Shape) { return GetTileArray(MinRange, MaxRange, Shape); }
	static bool GetValidatedTileArray(const int32 Origin, const int32 MinRange, const int32 MaxRange, const EGridShape Shape, TArray<int32> &Validated) { return GetValidatedTileArray(Origin, MinRange, MaxRange, Shape, Validated); }
	static void CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector> &Vertices, TArray<int32> &Triangles) { return CreateLine(Start, End, Thickness, Vertices, Triangles); }
	inline float GetTileSize() { return UWarBoardLibrary::TileSize; }
	inline int32 GetMaxWidth() { return UWarBoardLibrary::MaxWidth; }
	inline int32 GetOffset() { return UWarBoardLibrary::Offset; }
	inline bool GetBoardCentered() { return UWarBoardLibrary::BoardCentered; }
	inline FVector GetBoardLocation() { return UWarBoardLibrary::BoardLocation; }
}