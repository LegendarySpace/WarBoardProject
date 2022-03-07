// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <stdlib.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Tiles.generated.h"

struct FCubic;
struct FTile;

/**
 *	Possible shapes for tiles
 */
UENUM(BlueprintType, Category = "WarBoard|Enums")
enum class ETileShape : uint8
{
	Triangle		UMETA(DisplayName = "Triangle"),
	Square			UMETA(DisplayName = "Square"),
	Hex_Hor			UMETA(DisplayName = "Hexagon_Horizontal"),
	Hex_Vert		UMETA(DisplayName = "Hexagon_Vertical"),
	Octogon			UMETA(DisplayName = "Octogon"),
	Dodecagon		UMETA(DisplayName = "Dodecagon"),
	Shape_MAX		UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FOrtho
{
	GENERATED_BODY()

	FOrtho(int32 InRow = 0, int32 InCol = 0)
	{
		Row = InRow;
		Column = InCol;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarBoard|Tiles")
	int32 Row;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarBoard|Tiles")
	int32 Column;


	void operator=(const FOrtho Coord)
	{
		Row = Coord.Row;
		Column = Coord.Column;
	}

	FOrtho operator+(const FOrtho& Coord)
	{
		return FOrtho(this->Row + Coord.Row, this->Column + Coord.Column);
	}

	FOrtho& operator+=(const FOrtho& Coord)
	{
		this->Row += Coord.Row;
		this->Column += Coord.Column;
		return (*this);
	}

	FOrtho operator-(const FOrtho& Coord)
	{
		return FOrtho(this->Row - Coord.Row, this->Column - Coord.Column);
	}

	FOrtho& operator-=(const FOrtho& Coord)
	{
		this->Row -= Coord.Row;
		this->Column -= Coord.Column;
		return (*this);
	}

	bool operator<(const FOrtho& Coord)		// UPGRADE: Need to rework these	EX: (Row -5, Col 1) < (Row -1, Col -2)
	{
		return this->Row < Coord.Row && this->Column < Coord.Column;
	}

	bool operator<<(const FOrtho& Coord)
	{
		return abs(this->Row) <= abs(Coord.Row) && abs(this->Column) <= abs(Coord.Column);
	}

	bool operator>(const FOrtho& Coord)
	{
		return this->Row > Coord.Row&& this->Column > Coord.Column;
	}

	bool operator>>(const FOrtho& Coord)
	{
		return abs(this->Row) > abs(Coord.Row) || abs(this->Column) > abs(Coord.Column);
	}

	bool operator==(const FOrtho& Coord)
	{
		return this->Row == Coord.Row && this->Column == Coord.Column;
	}

	bool operator==(const FOrtho& Coord) const
	{
		return this->Row == Coord.Row && this->Column == Coord.Column;
	}

	bool operator!=(const FOrtho& Coord)
	{
		return this->Row != Coord.Row || this->Column != Coord.Column;
	}

	//	Math with other operands
	void operator=(const FTile Tile);

	void operator=(const int32 Index);

	void operator=(const FCubic Cubic);

	void operator=(const FVector V3);

	FOrtho operator+(const FTile& Tile);

	FOrtho operator+(const int32 Index);

	FOrtho operator+(const FCubic Cubic);

	FOrtho operator+(const FVector V3);

	FOrtho& operator+=(const FTile& Tile);

	FOrtho& operator+=(const int32 Index);

	FOrtho& operator+=(const FCubic Cubic);

	FOrtho& operator+=(const FVector V3);

	FOrtho operator-(const FTile& Tile);

	FOrtho operator-(const int32 Index);

	FOrtho operator-(const FCubic Cubic);

	FOrtho operator-(const FVector V3);

	FOrtho& operator-=(const FTile& Tile);

	FOrtho& operator-=(const int32 Index);

	FOrtho& operator-=(const FCubic Cubic);

	FOrtho& operator-=(const FVector V3);

	bool operator<(const FTile& Tile);

	bool operator<(const int32 Index);

	bool operator<(const FCubic Cubic);

	bool operator<(const FVector V3);

	bool operator<<(const FTile& Tile);

	bool operator<<(const int32 Index);

	bool operator<<(const FCubic Cubic);

	bool operator<<(const FVector V3);
	
	bool operator>(const FTile& Tile);

	bool operator>(const int32 Index);

	bool operator>(const FCubic Cubic);

	bool operator>(const FVector V3);

	bool operator>>(const FTile& Tile);

	bool operator>>(const int32 Index);

	bool operator>>(const FCubic Cubic);

	bool operator>>(const FVector V3);

	bool operator==(const FTile& Tile);

	bool operator==(const FTile& Tile) const;

	bool operator==(const int32 Index);

	bool operator==(const int32 Index) const;

	bool operator==(const FCubic& Cubic);

	bool operator==(const FCubic& Cubic) const;

	bool operator==(const FVector& V3);

	bool operator==(const FVector& V3) const;

	bool operator!=(const FTile& Tile);

	bool operator!=(const int32 Index);

	bool operator!=(const FCubic& Cubic);

	bool operator!=(const FVector& V3);
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarBoard|Tiles")
		int32 A;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarBoard|Tiles")
		int32 B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarBoard|Tiles")
		int32 C;


	void operator=(const FCubic Cubic)
	{
		A = Cubic.A;
		B = Cubic.B;
		C = Cubic.C;
	}
	
	FCubic operator+(const FCubic& Cubic)
	{
		return FCubic(this->A + Cubic.A, this->B + Cubic.B, this->C + Cubic.C);
	}

	FCubic& operator+=(const FCubic& Cubic)
	{
		this->A += Cubic.A;
		this->B += Cubic.B;
		this->B += Cubic.B;
		return (*this);
	}

	FCubic operator-(const FCubic& Cubic)
	{
		return FCubic(this->A - Cubic.A, this->B - Cubic.B, this->C - Cubic.C);
	}

	FCubic& operator-=(const FCubic& Cubic)
	{
		this->A -= Cubic.A;
		this->B -= Cubic.B;
		this->C -= Cubic.C;
		return (*this);
	}

	bool operator<(const FCubic& Cubic)		// closer to lower left
	{
		return this->C > Cubic.C;
	}

	bool operator<<(const FCubic& Cubic)	// closer to center
	{
		return (abs(this->A) + abs(this->B) + abs(this->C)) < (abs(Cubic.A) + abs(Cubic.B) + abs(Cubic.C));
	}

	bool operator>(const FCubic& Cubic)		// closer to upper right
	{
		return this->C < Cubic.C;
	}

	bool operator>>(const FCubic& Cubic)	// farther from center
	{
		return (abs(this->A) + abs(this->B) + abs(this->C)) > (abs(Cubic.A) + abs(Cubic.B) + abs(Cubic.C));
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

	//	Math with other operands
	void operator=(const FTile Tile);

	void operator=(const int32 Index);

	void operator=(const FOrtho Ortho);

	void operator=(const FVector V3);

	FCubic operator+(const FTile& Tile);

	FCubic operator+(const int32 Index);

	FCubic operator+(const FOrtho Ortho);

	FCubic operator+(const FVector V3);

	FCubic& operator+=(const FTile& Tile);

	FCubic& operator+=(const int32 Index);

	FCubic& operator+=(const FOrtho Ortho);

	FCubic& operator+=(const FVector V3);

	FCubic operator-(const FTile& Tile);

	FCubic operator-(const int32 Index);

	FCubic operator-(const FOrtho Ortho);

	FCubic operator-(const FVector V3);

	FCubic& operator-=(const FTile& Tile);

	FCubic& operator-=(const int32 Index);

	FCubic& operator-=(const FOrtho Ortho);

	FCubic& operator-=(const FVector V3);

	bool operator<(const FTile& Tile);

	bool operator<(const int32 Index);

	bool operator<(const FOrtho Ortho);

	bool operator<(const FVector V3);

	bool operator<<(const FTile& Tile);

	bool operator<<(const int32 Index);

	bool operator<<(const FOrtho Ortho);

	bool operator<<(const FVector V3);

	bool operator>(const FTile& Tile);

	bool operator>(const int32 Index);

	bool operator>(const FOrtho Ortho);

	bool operator>(const FVector V3);

	bool operator>>(const FTile& Tile);

	bool operator>>(const int32 Index);

	bool operator>>(const FOrtho Ortho);

	bool operator>>(const FVector V3);

	bool operator==(const FTile& Tile);

	bool operator==(const FTile& Tile) const;

	bool operator==(const int32 Index);

	bool operator==(const int32 Index) const;

	bool operator==(const FOrtho Ortho);

	bool operator==(const FOrtho Ortho) const;

	bool operator==(const FVector& V3);

	bool operator==(const FVector& V3) const;

	bool operator!=(const FTile& Tile);

	bool operator!=(const int32 Index);

	bool operator!=(const FOrtho Ortho);

	bool operator!=(const FVector& V3);
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
	FTile(FOrtho Coord) { OrthoToInternal(Coord); }
	FTile(int32 InRow, int32 InColumn) { OrthoToInternal(FOrtho(InRow, InColumn)); }
	FTile(FCubic Cubic) { CubicToInternal(Cubic); }
	FTile(int32 InA, int32 InB, int32 InC) { CubicToInternal(FCubic(InA, InB, InC)); }
	FTile(FVector InLocation) { WorldToInternal(InLocation); }

private:
	void IndexToInternal(int32 InIndex);
	void OrthoToInternal(FOrtho Coord);
	void CubicToInternal(FCubic Cubic);
	void WorldToInternal(FVector InLocation);
	void PIPtoInternal(FVector2D PIP);
	FVector2D GetPattern();
	static TArray<FVector>& GetVertices(const int32 Sides, const float Size, const float RelativeRotationToFirstVertex = 0.0, const float PolygonRotation = 0.0);

public:
	int32 ToIndex() const;
	FOrtho ToOrtho() const;
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
		OrthoToInternal(Tile.ToOrtho());
	}

	void operator=(const int32 Index)
	{
		IndexToInternal(Index);
	}

	void operator=(const FOrtho& Coord)
	{
		OrthoToInternal(Coord);
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
		return FTile(this->ToOrtho() + InTile.ToOrtho());
	}

	FTile& operator+=(const FTile InTile)
	{
		this->OrthoToInternal(this->ToOrtho() + InTile.ToOrtho());
		return *this;
	}

	FTile operator-(const FTile& InTile)
	{
		return FTile(this->ToOrtho() - InTile.ToOrtho());
	}

	FTile& operator-=(const FTile& InTile)
	{
		this->OrthoToInternal(this->ToOrtho() - InTile.ToOrtho());
		return *this;
	}

	bool operator<(const FTile& Tile)
	{
		return this->ToOrtho() < Tile.ToOrtho();
	}

	bool operator<(const FTile& Tile) const
	{
		return this->ToOrtho() < Tile.ToOrtho();
	}

	bool operator<<(const FTile& Tile)
	{
		return this->ToOrtho() << Tile.ToOrtho();
	}

	bool operator>(const FTile& Tile)
	{
		return this->ToOrtho() > Tile.ToOrtho();
	}

	bool operator>(const FTile& Tile) const
	{
		return this->ToOrtho() > Tile.ToOrtho();
	}

	bool operator>>(const FTile& Tile)
	{
		return this->ToOrtho() >> Tile.ToOrtho();
	}

	bool operator==(const FTile& Tile)
	{
		return this->ToOrtho() == Tile.ToOrtho();
	}

	bool operator==(const FTile& Tile) const
	{
		return this->ToOrtho() == Tile.ToOrtho();
	}

	bool operator!=(const FTile& Tile)
	{
		return this->ToOrtho() != Tile.ToOrtho();
	}

	FTile operator+(const int32 Index)
	{
		return *this + FTile(Index);
	}

	FTile operator+(const FOrtho& Coord)
	{
		return FTile(this->ToOrtho() + Coord);
	}

	FTile operator+(const FCubic& Cubic)
	{
		return FTile(this->ToCubic() + Cubic);
	}

	FTile operator+(const FVector& Location)
	{
		return FTile(this->ToWorld() + Location);
	}

	FTile& operator+=(const int32 Index)
	{
		this->OrthoToInternal(this->ToOrtho() + FTile(Index).ToOrtho());
		return *this;
	}

	FTile& operator+=(const FOrtho& Coord)
	{
		this->OrthoToInternal(this->ToOrtho() + Coord);
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

	FTile operator-(const int32 Index)
	{
		return FTile(this->ToOrtho() - FTile(Index).ToOrtho());
	}

	FTile operator-(const FOrtho& Coord)
	{
		return FTile(this->ToOrtho() - Coord);
	}

	FTile operator-(const FCubic& Cubic)
	{
		return FTile(this->ToCubic() - Cubic);
	}

	FTile operator-(const FVector& Location)
	{
		return FTile(this->ToWorld() - Location);
	}

	FTile& operator-=(const int32 Index)
	{
		this->OrthoToInternal(this->ToOrtho() - FTile(Index).ToOrtho());
		return *this;
	}

	FTile& operator-=(const FOrtho& Coord)
	{
		this->OrthoToInternal(this->ToOrtho() - Coord);
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

	bool operator<(const int32 Index)
	{
		return *this < FTile(Index);
	}

	bool operator<(const int32 Index) const
	{
		return *this < FTile(Index);
	}

	bool operator<(const FOrtho& Coord)
	{
		return *this < FTile(Coord);
	}

	bool operator<(const FOrtho& Coord) const
	{
		return *this < FTile(Coord);
	}

	bool operator<(const FCubic& Cubic)
	{
		return *this < FTile(Cubic);
	}

	bool operator<(const FCubic& Cubic) const
	{
		return *this < FTile(Cubic);
	}

	bool operator<(const FVector& Location)
	{
		return *this < FTile(Location);
	}

	bool operator<(const FVector& Location) const
	{
		return *this < FTile(Location);
	}

	bool operator<<(const int32 Index)
	{
		return *this << FTile(Index);
	}

	bool operator<<(const FOrtho& Coord)
	{
		return *this << FTile(Coord);
	}

	bool operator<<(const FCubic& Cubic)
	{
		return *this << FTile(Cubic);
	}

	bool operator<<(const FVector& Location)
	{
		return *this << FTile(Location);
	}

	bool operator>(const int32 Index)
	{
		return *this > FTile(Index);
	}

	bool operator>(const int32 Index) const
	{
		return *this > FTile(Index);
	}

	bool operator>(const FOrtho& Coord)
	{
		return *this > FTile(Coord);
	}

	bool operator>(const FOrtho& Coord) const
	{
		return *this > FTile(Coord);
	}

	bool operator>(const FCubic& Cubic)
	{
		return *this > FTile(Cubic);
	}

	bool operator>(const FCubic& Cubic) const
	{
		return *this > FTile(Cubic);
	}

	bool operator>(const FVector& Location)
	{
		return *this > FTile(Location);
	}

	bool operator>(const FVector& Location) const
	{
		return *this > FTile(Location);
	}

	bool operator>>(const int32 Index)
	{
		return *this >> FTile(Index);
	}

	bool operator>>(const FOrtho& Coord)
	{
		return *this >> FTile(Coord);
	}

	bool operator>>(const FCubic& Cubic)
	{
		return *this >> FTile(Cubic);
	}

	bool operator>>(const FVector& Location)
	{
		return *this >> FTile(Location);
	}

	bool operator==(const int32 Index)
	{
		return *this == FTile(Index);
	}

	bool operator==(const int32 Index) const
	{
		return *this == FTile(Index);
	}

	bool operator==(const FOrtho& Coord)
	{
		return *this == FTile(Coord);
	}

	bool operator==(const FOrtho& Coord) const
	{
		return *this == FTile(Coord);
	}

	bool operator==(const FCubic& Cubic)
	{
		return *this == FTile(Cubic);
	}

	bool operator==(const FCubic& Cubic) const
	{
		return *this == FTile(Cubic);
	}

	bool operator==(const FVector& Location)
	{
		return *this == FTile(Location);
	}

	bool operator==(const FVector& Location) const
	{
		return *this == FTile(Location);
	}

};


