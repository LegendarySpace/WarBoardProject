// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EGridShape.h"
#include "Tiles.h"

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
	*	Conversion functions for blueprint
	*/
	//
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Index To Tile", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FTile Conv_IndexToTile(int32 Index) { return FTile(Index); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile To Index", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static int32 Conv_TileToIndex(FTile Tile) { return Tile.ToIndex(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "World To Tile", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FTile Conv_VectorToTile(FVector World) { return FTile(World); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile To World", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FVector Conv_TileToVector(FTile Tile) { return Tile.ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord To Tile", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FTile Conv_CoordToTile(FOrtho Coord) { return FTile(Coord); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile To GCoord", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FOrtho Conv_TileToCoord(FTile Tile) { return Tile.ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic To Tile", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FTile Conv_CubicToTile(FCubic Cubic) { return FTile(Cubic); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile To Cubic", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FCubic Conv_TileToCubic(FTile Tile) { return Tile.ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Index To World", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FVector Conv_IndexToVector(int32 Index) { return FTile(Index).ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "World To Index", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static int32 Conv_VectorToIndex(FVector World) { return FTile(World).ToIndex(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Index To GCoord", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FOrtho Conv_IndexToCoord(int32 Index) { return FTile(Index).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord To Index", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static int32 Conv_CoordToIndex(FOrtho Coord) { return FTile(Coord).ToIndex(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Index To Cubic", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FCubic Conv_IndexToCubic(int32 Index) { return FTile(Index).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic To Index", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static int32 Conv_CubicToIndex(FCubic Cubic) { return FTile(Cubic).ToIndex(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "World To GCoord", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FOrtho Conv_VectorToCoord(FVector World) { return FTile(World).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord To World", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FVector Conv_CoordToVector(FOrtho Coord) { return FTile(Coord).ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "World To Cubic", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FCubic Conv_VectorToCubic(FVector World) { return FTile(World).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic To World", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FVector Conv_CubicToVector(FCubic Cubic) { return FTile(Cubic).ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord To Cubic", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FCubic Conv_CoordToCubic(FOrtho Coord) { return FTile(Coord).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic To GCoord", CompactNodeTitle = "->", BlueprintAutocast), Category = "WarBoard|Tile")
	static FOrtho Conv_CubicToCoord(FCubic Cubic) { return FTile(Cubic).ToOrtho(); }

	/**
	*	Tile Math
	*/
	//

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile + Integer", CompactNodeTitle = "+", ScriptMethod = "AddTile", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FTile Add_TileTile(FTile A, FTile B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile + Integer", CompactNodeTitle = "+", ScriptMethod = "AddInteger", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FTile Add_TileIndex(FTile A, int32 B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile + GCoord", CompactNodeTitle = "+", ScriptMethod = "AddGCoord", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FTile Add_TileCoord(FTile A, FOrtho B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile + Cubic", CompactNodeTitle = "+", ScriptMethod = "AddCubic", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FTile Add_TileCubic(FTile A, FCubic B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile + World", CompactNodeTitle = "+", ScriptMethod = "AddVector", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FTile Add_TileVector(FTile A, FVector B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord + Tile", CompactNodeTitle = "+", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FOrtho Add_CoordTile(FOrtho A, FTile B) { return A + B.ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord + Integer", CompactNodeTitle = "+", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FOrtho Add_CoordIndex(FOrtho A, int32 B) { return A + FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord + GCoord", CompactNodeTitle = "+", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FOrtho Add_CoordCoord(FOrtho A, FOrtho B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord + Cubic", CompactNodeTitle = "+", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FOrtho Add_CoordCubic(FOrtho A, FCubic B) { return A + FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord + Vector", CompactNodeTitle = "+", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FOrtho Add_CoordVector(FOrtho A, FVector B) { return A + FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic + Tile", CompactNodeTitle = "+",  ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FCubic Add_CubicTile(FCubic A, FTile B) { return A + B.ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic + Integer", CompactNodeTitle = "+", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FCubic Add_CubicIndex(FCubic A, int32 B) { return A + FTile(B).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic + GCoord", CompactNodeTitle = "+",  ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FCubic Add_CubicCoord(FCubic A, FOrtho B) { return A + FTile(B).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic + Cubic", CompactNodeTitle = "+", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FCubic Add_CubicCubic(FCubic A, FCubic B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic + Vector", CompactNodeTitle = "+",  ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FCubic Add_CubicVector(FCubic A, FVector B) { return A + FTile(B).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Vector + Tile", CompactNodeTitle = "+", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FVector Add_VectorTile(FVector A, FTile B) { return A + B.ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Vector + Integer", CompactNodeTitle = "+", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FVector Add_VectorIndex(FVector A, int32 B) { return A + FTile(B).ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Vector + GCoord", CompactNodeTitle = "+", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FVector Add_VectorCoord(FVector A, FOrtho B) { return A + FTile(B).ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Vector + Cubic", CompactNodeTitle = "+", ScriptOperator = "+;+=", Keywords = "+ add"), Category = "Math|Tile")
	static FVector Add_VectorCubic(FVector A, FCubic B) { return A + FTile(B).ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile - Tile", CompactNodeTitle = "-", ScriptMethod = "SubtractTile", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FTile Subtract_TileTile(FTile A, FTile B) { return A - B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile - Integer", CompactNodeTitle = "-", ScriptMethod = "SubtractInteger", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FTile Subtract_TileIndex(FTile A, int32 B) { return A - B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile - GCoord", CompactNodeTitle = "-", ScriptMethod = "SubtractGCoord", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FTile Subtract_TileCoord(FTile A, FOrtho B) { return A - B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile - Cubic", CompactNodeTitle = "-", ScriptMethod = "SubtractCubic", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FTile Subtract_TileCubic(FTile A, FCubic B) { return A - B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile - World", CompactNodeTitle = "-", ScriptMethod = "SubtractVector", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FTile Subtract_TileVector(FTile A, FVector B) { return A - B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord - Tile", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FOrtho Subtract_CoordTile(FOrtho A, FTile B) { return A - B.ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord - Integer", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FOrtho Subtract_CoordIndex(FOrtho A, int32 B) { return A - FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord - GCoord", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FOrtho Subtract_CoordCoord(FOrtho A, FOrtho B) { return A - B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord - Cubic", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FOrtho Subtract_CoordCubic(FOrtho A, FCubic B) { return A - FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord - Vector", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FOrtho Subtract_CoordVector(FOrtho A, FVector B) { return A - FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic - Tile", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FCubic Subtract_CubicTile(FCubic A, FTile B) { return A - B.ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic - Integer", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FCubic Subtract_CubicIndex(FCubic A, int32 B) { return A - FTile(B).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic - GCoord", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FCubic Subtract_CubicCoord(FCubic A, FOrtho B) { return A - FTile(B).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic - Cubic", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FCubic Subtract_CubicCubic(FCubic A, FCubic B) { return A - B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic - Vector", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FCubic Subtract_CubicVector(FCubic A, FVector B) { return A - FTile(B).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Vector - Tile", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FVector Subtract_VectorTile(FVector A, FTile B) { return A - B.ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Vector - Integer", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FVector Subtract_VectorIndex(FVector A, int32 B) { return A - FTile(B).ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Vector - Coord", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FVector Subtract_VectorCoord(FVector A, FOrtho B) { return A - FTile(B).ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Vector - Cubic", CompactNodeTitle = "-", ScriptOperator = "-;-=", Keywords = "- subtract"), Category = "Math|Tile")
	static FVector Subtract_VectorCubic(FVector A, FCubic B) { return A - FTile(B).ToWorld(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile < Tile", CompactNodeTitle = "<", ScriptMethod = "LessThanTile", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_TileTile(FTile A, FTile B) { return A < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile < Integer", CompactNodeTitle = "<", ScriptMethod = "LessThanInteger", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_TileIndex(FTile A, int32 B) { return A < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile < GCoord", CompactNodeTitle = "<", ScriptMethod = "LessThanGCoord", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_TileCoord(FTile A, FOrtho B) { return A < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile < Cubic", CompactNodeTitle = "<", ScriptMethod = "LessThanCubic", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_TileCubic(FTile A, FCubic B) { return A < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile < World", CompactNodeTitle = "<", ScriptMethod = "LessThanVector", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_TileVector(FTile A, FVector B) { return A < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord < GCoord", CompactNodeTitle = "<", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_CoordCoord(FOrtho A, FOrtho B) { return A < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord < Index", CompactNodeTitle = "<", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_CoordIndex(FOrtho A, int32 B) { return A < FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord < Cubic", CompactNodeTitle = "<", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_CoordCubic(FOrtho A, FCubic B) { return A < FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord < Vector", CompactNodeTitle = "<", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_CoordVector(FOrtho A, FVector B) { return FTile(A) < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic < Cubic", CompactNodeTitle = "<", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_CubicCubic(FCubic A, FCubic B) { return A < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic < Index", CompactNodeTitle = "<", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_CubicIndex(FCubic A, int32 B) { return A < FTile(B).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic < Vector", CompactNodeTitle = "<", ScriptOperator = "<", Keywords = "< lessthan"), Category = "Math|Tile")
	static bool LessThan_CubicVector(FCubic A, FVector B) { return FTile(A) < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile << Tile", CompactNodeTitle = "<<", ScriptMethod = "InsideTile", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_TileTile(FTile A, FTile B) { return A << B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile << Integer", CompactNodeTitle = "<<", ScriptMethod = "InsideInteger", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_TileIndex(FTile A, int32 B) { return A << B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile << GCoord", CompactNodeTitle = "<<", ScriptMethod = "InsideGCoord", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_TileCoord(FTile A, FOrtho B) { return A << B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile << Cubic", CompactNodeTitle = "<<", ScriptMethod = "InsideCubic", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_TileCubic(FTile A, FCubic B) { return A << B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile << World", CompactNodeTitle = "<<", ScriptMethod = "InsideVector", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_TileVector(FTile A, FVector B) { return A << B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord << GCoord", CompactNodeTitle = "<<", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_CoordCoord(FOrtho A, FOrtho B) { return A << B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord << Index", CompactNodeTitle = "<<", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_CoordIndex(FOrtho A, int32 B) { return A << FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord << Cubic", CompactNodeTitle = "<<", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_CoordCubic(FOrtho A, FCubic B) { return A << FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord << Vector", CompactNodeTitle = "<<", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_CoordVector(FOrtho A, FVector B){ return FTile(A) << B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic << Cubic", CompactNodeTitle = "<<", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_CubicCubic(FCubic A, FCubic B) { return A << B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic << Index", CompactNodeTitle = "<<", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_CubicIndex(FCubic A, int32 B) { return A << FTile(B).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic << Vector", CompactNodeTitle = "<<", ScriptOperator = "<<", Keywords = "<< inside"), Category = "Math|Tile")
	static bool Inside_CubicVector(FCubic A, FVector B)	{ return FTile(A) << B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile > Tile", CompactNodeTitle = ">", ScriptMethod = "GreaterThanTile", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_TileTile(FTile A, FTile B) { return A > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile > Integer", CompactNodeTitle = ">", ScriptMethod = "GreaterThanInteger", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_TileIndex(FTile A, int32 B) { return A > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile > GCoord", CompactNodeTitle = ">", ScriptMethod = "GreaterThanGCoord", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_TileCoord(FTile A, FOrtho B) { return A > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile > Cubic", CompactNodeTitle = ">", ScriptMethod = "GreaterThanCubic", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_TileCubic(FTile A, FCubic B) { return A > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile > World", CompactNodeTitle = ">", ScriptMethod = "GreaterThanVector", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_TileVector(FTile A, FVector B) { return A > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord > GCoord", CompactNodeTitle = ">", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_CoordCoord(FOrtho A, FOrtho B) { return A > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord > Index", CompactNodeTitle = ">", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_CoordIndex(FOrtho A, int32 B) { return A > FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord > Cubic", CompactNodeTitle = ">", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_CoordCubic(FOrtho A, FCubic B) { return A > FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord > Vector", CompactNodeTitle = ">", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_CoordVector(FOrtho A, FVector B) { return FTile(A) > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic > Cubic", CompactNodeTitle = ">", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_CubicCubic(FCubic A, FCubic B) { return A > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic > Index", CompactNodeTitle = ">", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_CubicIndex(FCubic A, int32 B) { return A > FTile(B).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic > Vector", CompactNodeTitle = ">", ScriptOperator = ">", Keywords = "> greaterthan"), Category = "Math|Tile")
	static bool GreaterThan_CubicVector(FCubic A, FVector B) { return FTile(A) > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile >> Tile", CompactNodeTitle = ">>", ScriptMethod = "OutsideTile", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_TileTile(FTile A, FTile B) { return A >> B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile >> Integer", CompactNodeTitle = ">>", ScriptMethod = "OutsideInteger", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_TileIndex(FTile A, int32 B) { return A >> B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile >> GCoord", CompactNodeTitle = ">>", ScriptMethod = "OutsideGCoord", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_TileCoord(FTile A, FOrtho B) { return A >> B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile >> Cubic", CompactNodeTitle = ">>", ScriptMethod = "OutsideCubic", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_TileCubic(FTile A, FCubic B) { return A >> B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile >> World", CompactNodeTitle = ">>", ScriptMethod = "OutsideVector", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_TileVector(FTile A, FVector B) { return A >> B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord >> GCoord", CompactNodeTitle = ">>", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_CoordCoord(FOrtho A, FOrtho B) { return A >> B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord >> Index", CompactNodeTitle = ">>", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_CoordIndex(FOrtho A, int32 B) { return A >> FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord >> Cubic", CompactNodeTitle = ">>", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_CoordCubic(FOrtho A, FCubic B) { return A >> FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord >> Vector", CompactNodeTitle = ">>", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_CoordVector(FOrtho A, FVector B) { return FTile(A) >> B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic >> Cubic", CompactNodeTitle = ">>", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_CubicCubic(FCubic A, FCubic B) { return A >> B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic >> Index", CompactNodeTitle = ">>", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_CubicIndex(FCubic A, int32 B) { return A >> FTile(B).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic >> Vector", CompactNodeTitle = ">>", ScriptOperator = ">>", Keywords = ">> outside"), Category = "Math|Tile")
	static bool Outside_CubicVector(FCubic A, FVector B) { return FTile(A) >> B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Tile)", CompactNodeTitle = "==", ScriptMethod = "EqualTile", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_TileTile(FTile A, FVector B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Integer)", CompactNodeTitle = "==", ScriptMethod = "EqualInteger", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_TileIndex(FTile A, int32 B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (GCoord)", CompactNodeTitle = "==", ScriptMethod = "EqualGCoord", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_TileCoord(FTile A, FOrtho B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Cubic)", CompactNodeTitle = "==", ScriptMethod = "EqualCubic", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_TileCubic(FTile A, FCubic B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Vector)", CompactNodeTitle = "==", ScriptMethod = "EqualVector", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_TileVector(FTile A, FVector B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord == GCoord", CompactNodeTitle = "==", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_CoordCoord(FOrtho A, FOrtho B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord == Index", CompactNodeTitle = "==", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_CoordIndex(FOrtho A, int32 B) { return A == FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord == Cubic", CompactNodeTitle = "==", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_CoordCubic(FOrtho A, FCubic B) { return A == FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GCoord == Vector", CompactNodeTitle = "==", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_CoordVector(FOrtho A, FVector B) { return A == FTile(B).ToOrtho(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic == Cubic", CompactNodeTitle = "==", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_CubicCubic(FCubic A, FCubic B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic == Index", CompactNodeTitle = "==", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_CubicIndex(FCubic A, int32 B) { return A == FTile(B).ToCubic(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cubic == Vector", CompactNodeTitle = "==", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Tile")
	static bool EqualEqual_CubicVector(FCubic A, FVector B) { return A == FTile(B).ToCubic(); }

	/**
	*	Utility functions
	*/
	//

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Snap to grid (Vector)"), Category = "Math|Vector")
	static FVector GridSnap_Vector(const FVector Location, const bool Center = false) { return FTile::SnapToGridVector(Location, Center); }

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true", CompactNodeTitle = "ActorAt", Category = "WarBoard|Utility"))
	static AActor* GetActorAtTile(UObject* WorldContextObject, const FTile Tile);

	UFUNCTION(BlueprintCallable, meta = (Category = "WarBoard|Utility"))
	static void SetTileSize(float Size) { FTile::SetTileSize(Size); }

	UFUNCTION(BlueprintCallable, meta = (Category = "WarBoard|Utility"))
	static float GetTileSize() { return FTile::GetTileSize(); }

	UFUNCTION(BlueprintCallable, meta = (Category = "WarBoard|Utility"))
	static void SetTileShape(ETileShape Shape) { return FTile::SetTileShape(Shape); }

	UFUNCTION(BlueprintCallable, meta = (Category = "WarBoard|Utility"))
	static ETileShape GetTileShape() { return FTile::GetTileShape(); }

	/**
	*		DEPRECATED Utility functions				TODO: REMOVE
	*/

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
	inline FVector GridSnap_Vector(const FVector Location, const bool Center = false) { return FTile::SnapToGridVector(Location, Center); }
	inline void SetTileSize(float Size) { FTile::SetTileSize(Size); }
	inline float GetTileSize() { return FTile::GetTileSize(); }
	inline void SetTileShape(ETileShape Shape) { return FTile::SetTileShape(Shape); }
	inline ETileShape GetTileShape() { return FTile::GetTileShape(); }
	inline int32 GetMaxWidth() { return FTile::MAX_WIDTH; }
	inline AActor* GetActorAtTile(UObject* WorldContextObject, const FTile Tile) { return UWarBoardLibrary::GetActorAtTile(WorldContextObject, Tile); }

	// Deprecated
	inline void SetHighlightedActor(AActor* Actor = nullptr) { return UWarBoardLibrary::SetHighlightedActor(Actor); }
	inline AActor* GetHighlightedActor() { return UWarBoardLibrary::GetHighlightedActor(); }
	inline bool IsSameTeam(const AActor* A, const AActor* B) { return UWarBoardLibrary::IsSameTeam(A, B); }
	inline bool IsEnemyTeam(const AActor* A, const AActor* B) { return UWarBoardLibrary::IsEnemyTeam(A, B); }

}