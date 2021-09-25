// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	static void IndexToWorld(const int32 Index, const bool TileCenter, FVector &Location);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "ITT", Category = "WarBoard|Conversion"))
	static void IndexToTile(const int32 Index, int32 &Row, int32 &Col);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTW", Category = "WarBoard|Conversion"))
	static void TileToWorld(const int32 Row, const int32 Col, const bool TileCenter, FVector &Location);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTI", Category = "WarBoard|Conversion"))
	static void TileToIndex(const int32 Row, const int32 Col, int32 &Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTW", Category = "WarBoard|Conversion"))
	static void WorldToIndex(const FVector Location, int32 &Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "TTW", Category = "WarBoard|Conversion"))
	static void WorldToTile(const FVector Location, int32 &Row, int32 &Col);
	

	/**
	*		Utility functions
	*/


	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true", CompactNodeTitle = "ActorAt", Category = "WarBoard|Utility"))
	static AActor* GetActorAtIndex(UObject* WorldContextObject, const int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true", CompactNodeTitle = "ActorAt", Category = "WarBoard|Utility"))
	static AActor* GetActorAtTile(UObject* WorldContextObject, const int32 Row, const int32 Col);

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "Highlight", Category = "WarBoard|Utility"))
	static void SetHighlightedActor(AActor *Actor = nullptr) { Highlighted = Actor; };

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "Highlighted", Category = "WarBoard|Utility"))
	static AActor* GetHighlightedActor() { return Highlighted; }

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "TileArray", Category = "WarBoard|Utility"))
	static TArray<int32> GetTileArray(const int32 MinRange, const int32 MaxRange, const EGridShape Shape);

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "ValidatedTiles", Category = "WarBoard|Utility"))
	static bool GetValidatedTileArray(const int32 Origin, const int32 MinRange, const int32 MaxRange, const EGridShape Shape, TArray<int32> &ValidatedShape, const TArray<int32> Accessible);

	UFUNCTION(BlueprintCallable, meta = (Category = "WarBoard|Utility"))
	static void CreateLine(const FVector Start, const FVector End, float Thickness, TArray<FVector> &Vertices, TArray<int32> &Triangles);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Ally", Category = "WarBoard|Utility"))
	static bool IsSameTeam(const AActor* A, const AActor* B);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Enemy", Category = "WarBoard|Utility"))
	static bool IsEnemyTeam(const AActor* A, const AActor* B);

public:
	static float TileSize;
	static int32 MaxWidth;
	static int32 Offset;
	static bool BoardCentered;
	static FVector BoardLocation;
	static AActor* Highlighted;

	static void initialize();
};

namespace WarBoardLib
{
	// Make Static calls through namespace so its footprint can be reduced with 'using namespace'
	inline void IndexToWorld(const int32 Index, const bool TileCenter, FVector &Location) { return UWarBoardLibrary::IndexToWorld(Index, TileCenter, Location); }
	inline void IndexToTile(const int32 Index, int32 &Row, int32 &Col) { return UWarBoardLibrary::IndexToTile(Index, Row, Col); }
	inline void TileToWorld(const int32 Row, const int32 Col, const bool TileCenter, FVector &Location) { return UWarBoardLibrary::TileToWorld(Row, Col, TileCenter, Location); }
	inline void TileToIndex(const int32 Row, const int32 Col, int32 &Index) { return UWarBoardLibrary::TileToIndex(Row, Col, Index); }
	inline void WorldToIndex(const FVector Location, int32 &Index) { return UWarBoardLibrary::WorldToIndex(Location, Index); }
	inline void WorldToTile(const FVector Location, int32 &Row, int32 &Col) { return UWarBoardLibrary::WorldToTile(Location, Row, Col); }
	inline AActor* GetActorAtIndex(UObject* WorldContextObject, const int32 Index) { return UWarBoardLibrary::GetActorAtIndex(WorldContextObject, Index); }
	inline AActor* GetActorAtTile(UObject* WorldContextObject, const int32 Row, const int32 Col) { return UWarBoardLibrary::GetActorAtTile(WorldContextObject, Row, Col); }
	inline void SetHighlightedActor(AActor* Actor = nullptr) { return UWarBoardLibrary::SetHighlightedActor(Actor); }
	inline AActor* GetHighlightedActor() { return UWarBoardLibrary::GetHighlightedActor(); }
	inline TArray<int32> GetTileArray(const int32 MinRange, const int32 MaxRange, const EGridShape Shape) { return GetTileArray(MinRange, MaxRange, Shape); }
	inline bool GetValidatedTileArray(const int32 Origin, const int32 MinRange, const int32 MaxRange, const EGridShape Shape, TArray<int32> &ValidatedShape, const TArray<int32> Accessible) { return GetValidatedTileArray(Origin, MinRange, MaxRange, Shape, ValidatedShape, Accessible); }
	inline void CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector> &Vertices, TArray<int32> &Triangles) { return CreateLine(Start, End, Thickness, Vertices, Triangles); }
	inline float GetTileSize() { return UWarBoardLibrary::TileSize; }
	inline int32 GetMaxWidth() { return UWarBoardLibrary::MaxWidth; }
	inline int32 GetOffset() { return UWarBoardLibrary::Offset; }
	inline bool GetBoardCentered() { return UWarBoardLibrary::BoardCentered; }
	inline FVector GetBoardLocation() { return UWarBoardLibrary::BoardLocation; }
	inline bool IsSameTeam(const AActor* A, const AActor* B) { return UWarBoardLibrary::IsSameTeam(A, B); }
	inline bool IsEnemyTeam(const AActor* A, const AActor* B) { return UWarBoardLibrary::IsEnemyTeam(A, B); }
}