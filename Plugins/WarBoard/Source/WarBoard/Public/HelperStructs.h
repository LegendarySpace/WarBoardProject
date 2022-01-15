// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TileShape.h"
#include "ETileType.h"

#include "HelperStructs.generated.h"


USTRUCT(BlueprintType)
struct FGCoord
{
	GENERATED_BODY()

	FGCoord(int32 InRow = 0, int32 InCol = 0)
	{
		Row = InRow;
		Column = InCol;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Row;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Column;

	FGCoord operator+(const FGCoord& Coord)
	{
		return FGCoord(this->Row + Coord.Row, this->Column + Coord.Column);
	}

	FGCoord operator-(const FGCoord& Coord)
	{
		return FGCoord(this->Row - Coord.Row, this->Column - Coord.Column);
	}

	bool operator==(const FGCoord& Coord)
	{
		return this->Row == Coord.Row && this->Column == Coord.Column;
	}

	bool operator==(const FGCoord& Coord) const
	{
		return this->Row == Coord.Row && this->Column == Coord.Column;
	}

	bool operator!=(const FGCoord& Coord)
	{
		return this->Row != Coord.Row || this->Column != Coord.Column;
	}

};

USTRUCT(BlueprintType)
struct FCubic
{
	GENERATED_BODY()

		FCubic(int32 InA = 0, int32 InB = 0, int32 InC = 0)
	{
		A = InA;
		B = InB;
		C = InC;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 A;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 C;

	FCubic operator+(const FCubic& Cubic)
	{
		return FCubic(this->A + Cubic.A, this->B + Cubic.B, this->C + Cubic.C);
	}

	FCubic operator-(const FCubic& Cubic)
	{
		return FCubic(this->A - Cubic.A, this->B - Cubic.B, this->C - Cubic.C);
	}

	bool operator==(const FCubic& Cubic)
	{
		return this->A == Cubic.A && this->B == Cubic.B && this->C == Cubic.C;
	}

	bool operator==(const FCubic& Cubic) const
	{
		return this->A == Cubic.A && this->B == Cubic.B && this->C == Cubic.C;
	}

	bool operator!=(const FCubic& Cubic)
	{
		return this->A != Cubic.A || this->B != Cubic.B || this->C != Cubic.C;
	}

};


/**
*
*
*/
USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()

public:
	FTile(int32 InIndex = 0) { IndexToInternal(InIndex); }
	FTile(FGCoord Coord) { RCToInternal(Coord); }
	FTile(int32 InRow, int32 InColumn) { RCToInternal(FGCoord(InRow, InColumn)); }
	FTile(FCubic Cubic) { CubicToInternal(Cubic); }
	FTile(int32 InA, int32 InB, int32 InC) { CubicToInternal(FCubic(InA, InB, InC)); }
	FTile(FVector InLocation) { WorldToInternal(InLocation); }

private:
	void IndexToInternal(int32 InIndex);
	void RCToInternal(FGCoord Coord);
	void CubicToInternal(FCubic Cubic);
	void WorldToInternal(FVector InLocation);
	void PIPtoInternal(FVector2D PIP);
	FVector2D GetPattern();
	static TArray<FVector>& GetVertices(const int32 Sides, const float Size, const float RelativeRotationToFirstVertex = 0.0, const float PolygonRotation = 0.0);

public:
	int32 ToIndex() const;
	FGCoord ToRC() const;
	FCubic ToCubic() const;
	FVector ToWorld(bool TileCenter = true) const;
	static void SetTileSize(float Size);
	static float GetTileSize() { return TileSize; }
	static void SetTileShape(ETileShape InShape) { Shape = InShape; }
	static ETileShape GetTileShape() { return Shape; };
	TArray<FVector>& GetPolygon(float Padding = 0);
	static TArray<FVector>& GetPolygonByShape(ETileShape Shape, float Padding = 0, float Size = TileSize);
	static FVector SnapToGridVector(const FVector Location, const bool Center = false);

private:
	int32 Row;
	int32 Col;
	static float TileSize;
	static float Radius;
	static float Height;
	static float Width;
	static ETileShape Shape;

public:
	static const int32 MAX_WIDTH = 6500;

public:
	void operator=(const FTile& Tile)
	{
		RCToInternal(Tile.ToRC());
	}

	void operator=(const int32 Index)
	{
		IndexToInternal(Index);
	}

	void operator=(const FGCoord& Coord)
	{
		RCToInternal(Coord);
	}

	void operator=(const FCubic& Cubic)
	{
		CubicToInternal(Cubic);
	}

	void operator=(const FVector& Location)
	{
		WorldToInternal(Location);
	}

	FTile operator+(const FTile& InTile)
	{ 
		return FTile(this->ToRC() + InTile.ToRC());
	}

	FTile operator+(const int32 Index)
	{
		return FTile(this->ToRC() + FTile(Index).ToRC());
	}

	FTile operator+(const FGCoord& Coord)
	{
		return FTile(this->ToRC() + Coord);
	}

	FTile operator+(const FCubic& Cubic)
	{
		return FTile(this->ToCubic() + Cubic);
	}

	FTile operator+(const FVector& Location)
	{
		return FTile(this->ToWorld() + Location);
	}

	FTile operator-(const FTile& InTile)
	{
		return FTile(this->ToRC() - InTile.ToRC());
	}

	FTile operator-(const int32 Index)
	{
		return FTile(this->ToRC() - FTile(Index).ToRC());
	}

	FTile operator-(const FGCoord& Coord)
	{
		return FTile(this->ToRC() - Coord);
	}

	FTile operator-(const FCubic& Cubic)
	{
		return FTile(this->ToCubic() - Cubic);
	}

	FTile operator-(const FVector& Location)
	{
		return FTile(this->ToWorld() - Location);
	}

	FTile& operator+=(const FTile InTile)
	{
		this->RCToInternal(this->ToRC() + InTile.ToRC());
		return *this;
	}

	FTile& operator+=(const int32 Index)
	{
		this->RCToInternal(this->ToRC() + FTile(Index).ToRC());
		return *this;
	}

	FTile& operator+=(const FGCoord& Coord)
	{
		this->RCToInternal(this->ToRC() + Coord);
		return *this;
	}

	FTile& operator+=(const FCubic& Cubic)
	{
		this->CubicToInternal(this->ToCubic() + Cubic);
		return *this;
	}

	FTile& operator+=(const FVector& Location)
	{
		this->WorldToInternal(this->ToWorld() + Location);
		return *this;
	}

	FTile& operator-=(const FTile& InTile)
	{
		this->RCToInternal(this->ToRC() - InTile.ToRC());
		return *this;
	}

	FTile& operator-=(const int32 Index)
	{
		this->RCToInternal(this->ToRC() - FTile(Index).ToRC());
		return *this;
	}

	FTile& operator-=(const FGCoord& Coord)
	{
		this->RCToInternal(this->ToRC() - Coord);
		return *this;
	}

	FTile& operator-=(const FCubic& Cubic)
	{
		this->CubicToInternal(this->ToCubic() - Cubic);
		return *this;
	}

	FTile& operator-=(const FVector& Location)
	{
		this->WorldToInternal(this->ToWorld() - Location);
		return *this;
	}

	bool operator==(const FTile& Tile)
	{
		return this->ToRC() == Tile.ToRC();
	}

	bool operator==(const FTile& Tile) const
	{
		return this->ToRC() == Tile.ToRC();
	}

	bool operator!=(const FTile& Tile)
	{
		return this->ToRC() != Tile.ToRC();
	}

	bool operator==(const int32 Index)
	{
		return this->ToIndex() == Index;
	}

	bool operator==(const int32 Index) const
	{
		return this->ToIndex() == Index;
	}

	bool operator==(const FGCoord& Coord)
	{
		return this->ToRC() == Coord;
	}

	bool operator==(const FGCoord& Coord) const
	{
		return this->ToRC() == Coord;
	}

	bool operator==(const FCubic& Cubic)
	{
		return this->ToCubic() == Cubic;
	}

	bool operator==(const FCubic& Cubic) const
	{
		return this->ToCubic() == Cubic;
	}

	bool operator==(const FVector& Location)
	{
		return this->ToRC() == FTile(Location).ToRC();
	}

	bool operator==(const FVector& Location) const
	{
		return this->ToRC() == FTile(Location).ToRC();
	}

	// TODO: May add support for <>
};


USTRUCT(BlueprintType)
struct FTileInstance
{
	GENERATED_BODY()

public:
	FTileInstance(FGCoord InCoord = FGCoord(), ETileType InType = ETileType::TT_Normal)
	{
		Coord = InCoord;
		Type = InType;
	}

	// Should also accept FTile in constructor

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGCoord Coord;
	// TODO: This is causing errors
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETileType> Type;

	void operator=(ETileType InType)
	{
		Type = InType;
	}

	bool operator==(const FTileInstance& Instance)
	{
		return Coord == Instance.Coord && Type == Instance.Type;
	}

	bool operator==(const FTileInstance& Instance) const
	{
		return Coord == Instance.Coord && Type == Instance.Type;
	}

	bool operator==(const int32 InIndex)
	{
		return Coord == FTile(InIndex).ToRC();
	}

	bool operator==(const int32 InIndex) const
	{
		return Coord == FTile(InIndex).ToRC();
	}

	bool operator==(const FGCoord InCoord)
	{
		return Coord == InCoord;
	}

	bool operator==(const FGCoord InCoord) const
	{
		return Coord == InCoord;
	}

	bool operator==(const ETileType& InType)
	{
		return Type == InType;
	}

	bool operator==(const ETileType& InType) const
	{
		return Type == InType;
	}

};


USTRUCT(BlueprintType)
struct FTileSetup
{
	GENERATED_BODY()

public:
	FTileSetup(ETileType InType = ETileType::TT_Normal, UStaticMesh* InMesh = nullptr, UMaterial* InMat = nullptr)
	{ 
		Type = InType;
		Mesh = InMesh;
		Mat = InMat;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETileType> Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* Mat;

	void operator=(UStaticMesh* StaticMesh)
	{
		Mesh = StaticMesh;
	}

	void operator=(UMaterial* Material)
	{
		Mat = Material;
	}

	bool operator==(ETileType InType)
	{
		return Type == InType;
	}

	bool operator==(ETileType InType) const
	{
		return Type == InType;
	}

};


