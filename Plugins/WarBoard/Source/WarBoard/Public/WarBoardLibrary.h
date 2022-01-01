// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EGridShape.h"
#include "TileShape.h"
#include "HelperStructs.h"

#include "WarBoardLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WARBOARD_API UWarBoardLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "WarBoard|Utility"))
		static void MakeTile(int32 Row, int32 Column, FTile& Tile) { Tile = FTile(Row, Column); }

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "WarBoard|Utility"))
		static void MakeTileFromCubic(int32 A, int32 B, int32 C, FTile& Tile) { Tile = FTile(A, B, C); }

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "WarBoard|Utility"))
		static void BreakTile(FTile Tile, int32& Row, int32& Column) { Row = (int32)Tile.ToRC().X; Column = (int32)Tile.ToRC().Y; }

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "WarBoard|Utility"))
		static void BreakTileToCubic(FTile Tile, int32& A, int32& B, int32& C) { A = (int32)Tile.ToCubic().X; B = (int32)Tile.ToCubic().Y; C = (int32)Tile.ToCubic().Z; }

	//
	// Tile functions
	//

	// TODO: Add Structs for cubic and rc then add conversions for them
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Index To Tile", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
		static FTile Conv_IndexToTile(int32 Index) { return FTile(Index); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile To Index", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
		static int32 Conv_TileToIndex(FTile Tile) { return Tile.ToIndex(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "World To Tile", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
		static FTile Conv_VectorToTile(FVector World) { return FTile(World); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile To World", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
		static FVector Conv_TileToVector(FTile Tile) { return Tile.ToWorld(); }

	// Add conv for index to world also


	// Operator BP support
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile + Integer", CompactNodeTitle = "+", ScriptMethod = "AddInteger", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
		static FTile Add_TileIndex(FTile A, int32 B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile + Vector", CompactNodeTitle = "+", ScriptMethod = "AddVector", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
		static FTile Add_TileVector(FTile A, FVector B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile + Tile", CompactNodeTitle = "+", ScriptMethod = "AddTile", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
		static FTile Add_TileTile(FTile A, FTile B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile - Integer", CompactNodeTitle = "-", ScriptMethod = "SubtractInteger", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
		static FTile Subtract_TileIndex(FTile A, int32 B) { return A - B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile - Vector", CompactNodeTitle = "-", ScriptMethod = "SubtractVector", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
		static FTile Subtract_TileVector(FTile A, int32 B) { return A - B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile - Tile", CompactNodeTitle = "-", ScriptMethod = "SubtractTile", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
		static FTile Subtract_TileTile(FTile A, int32 B) { return A - B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Integer)", CompactNodeTitle = "==", ScriptMethod = "EqualInteger", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
		static bool EqualEqual_TileIndex(FTile A, int32 B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Vector)", CompactNodeTitle = "==", ScriptMethod = "EqualVector", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
		static bool EqualEqual_TileVector(FTile A, FVector B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Tile)", CompactNodeTitle = "==", ScriptMethod = "EqualTile", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
		static bool EqualEqual_TileTile(FTile A, FVector B) { return A == B; }

	// TODO: Add support for !=, >, <, >=, <=		// Maybe add * / & |



	UFUNCTION(BlueprintPure, meta = (DisplayName = "Snap to grid (Vector)"), Category = "Math|Vector")
		static FVector GridSnap_Vector(const FVector Location, const bool Center = false) { return FTile::SnapToGridVector(Location, Center); }

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true", CompactNodeTitle = "ActorAt", Category = "WarBoard|Utility"))
	static AActor* GetActorAtTile(UObject* WorldContextObject, const FTile Tile);

	/**
	*		Utility functions		DEPRECATED		TODO: REMOVE
	*/


	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "TileArray", Category = "WarBoard|Utility"))
	static TArray<FTile> GetTileArray(const int32 MinRange, const int32 MaxRange, const EGridShape Shape);

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "ValidatedTiles", Category = "WarBoard|Utility"))
	static bool GetValidatedTileArray(const int32 Origin, const int32 MinRange, const int32 MaxRange, const EGridShape Shape, TArray<int32> &ValidatedShape, const TArray<int32> Accessible);

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "Highlight", Category = "WarBoard|Utility"))
	static void SetHighlightedActor(AActor *Actor = nullptr) { Highlighted = Actor; };

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "Highlighted", Category = "WarBoard|Utility"))
	static AActor* GetHighlightedActor() { return Highlighted; }

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Ally", Category = "WarBoard|Utility"))
	static bool IsSameTeam(const AActor* A, const AActor* B);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = "Enemy", Category = "WarBoard|Utility"))
	static bool IsEnemyTeam(const AActor* A, const AActor* B);

public:
	static AActor* Highlighted;
};

namespace WarBoardLib
{
	// Make Static calls through namespace so its footprint can be reduced with 'using namespace'
	inline void SetTileSize(float Size) { FTile::SetTileSize(Size); }
	inline float GetTileSize() { return FTile::GetTileSize(); }
	inline void SetTileShape(ETileShape Shape) { return FTile::SetTileShape(Shape); }
	inline ETileShape GetTileShape() { return FTile::GetTileShape(); }
	inline int32 GetMaxWidth() { return FTile::MAX_WIDTH; }
	inline AActor* GetActorAtTile(UObject* WorldContextObject, const FTile Tile) { return UWarBoardLibrary::GetActorAtTile(WorldContextObject, Tile); }

	// Deprecated
	inline TArray<int32> GetTileArray(const int32 MinRange, const int32 MaxRange, const EGridShape Shape) { return GetTileArray(MinRange, MaxRange, Shape); }
	inline bool GetValidatedTileArray(const int32 Origin, const int32 MinRange, const int32 MaxRange, const EGridShape Shape, TArray<int32> &ValidatedShape, const TArray<int32> Accessible) { return GetValidatedTileArray(Origin, MinRange, MaxRange, Shape, ValidatedShape, Accessible); }
	
	inline void SetHighlightedActor(AActor* Actor = nullptr) { return UWarBoardLibrary::SetHighlightedActor(Actor); }
	inline AActor* GetHighlightedActor() { return UWarBoardLibrary::GetHighlightedActor(); }
	inline bool IsSameTeam(const AActor* A, const AActor* B) { return UWarBoardLibrary::IsSameTeam(A, B); }
	inline bool IsEnemyTeam(const AActor* A, const AActor* B) { return UWarBoardLibrary::IsEnemyTeam(A, B); }

}