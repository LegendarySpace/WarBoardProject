// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TileShape.h"
#include "ETileType.h"

#include "HelperStructs.generated.h"

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
	FTile(int32 InRow, int32 InColumn) { RCToInternal(InRow, InColumn); }
	FTile(int32 InA, int32 InB, int32 InC) { CubicToInternal(InA, InB, InC); }
	FTile(FVector InLocation) { WorldToInternal(InLocation); }

private:
	void IndexToInternal(int32 InIndex);
	void RCToInternal(int32 InRow, int32 InColumn);
	void CubicToInternal(int32 InA, int32 InB, int32 InC);
	void WorldToInternal(FVector InLocation);
	void PIPtoInternal(FVector2D PIP);
	FVector2D GetPattern();
	static TArray<FVector>& GetVertices(const int32 Sides, const float Size, const float RelativeRotationToFirstVertex = 0.0, const float PolygonRotation = 0.0);

public:
	int32 ToIndex() const;
	FVector2D ToRC() const;
	FVector ToCubic() const;
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

	// TODO: Add Overloaded operators
public:
	void operator=(const FTile Tile)
	{
		auto rc = Tile.ToRC();
		RCToInternal(rc.X, rc.Y);
	}

	void operator=(const int32 Index)
	{
		IndexToInternal(Index);
	}

	void operator=(const FVector Location)
	{
		WorldToInternal(Location);
	}

	FTile operator+(const FTile InTile)
	{
		FVector2D RC = this->ToRC() + InTile.ToRC();
		return FTile(RC.X, RC.Y);
	}

	FTile operator+(const int32 Index)
	{
		FVector2D RC = this->ToRC() + FTile(Index).ToRC();
		return FTile(RC.X, RC.Y);
	}

	FTile operator+(const FVector Location)
	{
		return FTile(this->ToWorld() + Location);
	}

	FTile operator-(const FTile InTile)
	{
		FVector2D RC = this->ToRC() - InTile.ToRC();
		return FTile(RC.X, RC.Y);
	}

	FTile operator-(const int32 Index)
	{
		FVector2D RC = this->ToRC() - FTile(Index).ToRC();
		return FTile(RC.X, RC.Y);
	}

	FTile operator-(const FVector Location)
	{
		return FTile(this->ToWorld() - Location);
	}

	FTile& operator+=(const FTile InTile)
	{
		FVector2D RC = this->ToRC() + InTile.ToRC();
		this->RCToInternal(RC.X, RC.Y);
		return *this;
	}

	FTile& operator+=(const int32 Index)
	{
		FVector2D RC = this->ToRC() + FTile(Index).ToRC();
		this->RCToInternal(RC.X, RC.Y);
		return *this;
	}

	FTile& operator+=(const FVector Location)
	{
		this->WorldToInternal(this->ToWorld() + Location);
		return *this;
	}

	FTile& operator-=(const FTile InTile)
	{
		FVector2D RC = this->ToRC() - InTile.ToRC();
		this->RCToInternal(RC.X, RC.Y);
		return *this;
	}

	FTile& operator-=(const int32 Index)
	{
		FVector2D RC = this->ToRC() - FTile(Index).ToRC();
		this->RCToInternal(RC.X, RC.Y);
		return *this;
	}

	FTile& operator-=(const FVector Location)
	{
		this->WorldToInternal(this->ToWorld() - Location);
		return *this;
	}

	bool operator==(const FTile& Tile)
	{
		return this->ToIndex() == Tile.ToIndex();
	}

	bool operator==(const FTile& Tile) const
	{
		return this->ToIndex() == Tile.ToIndex();
	}

	bool operator!=(const FTile& Tile)
	{
		return this->ToIndex() != Tile.ToIndex();
	}

	bool operator!=(const FTile& Tile) const
	{
		return this->ToIndex() != Tile.ToIndex();
	}

	bool operator==(const int32 Index)
	{
		return this->ToIndex() == Index;
	}

	bool operator==(const int32 Index) const
	{
		return this->ToIndex() == Index;
	}

	bool operator==(const FVector& Location)
	{
		FTile Tile = FTile(Location);
		return this->ToIndex() == Tile.ToIndex();
	}

	bool operator==(const FVector& Location) const
	{
		FTile Tile = FTile(Location);
		return this->ToIndex() == Tile.ToIndex();
	}

};


USTRUCT(BlueprintType)
struct FTileInstance
{
	GENERATED_BODY()

public:
	FTileInstance(FTile InIndex = 0, ETileType InType = ETileType::TT_Normal)
	{
		Tile = InIndex;
		Type = InType;
	}

	FTile Tile;
	ETileType Type;

	void operator=(ETileType InType)
	{
		Type = InType;
	}

	bool operator==(const FTileInstance& Instance)
	{
		return Tile == Instance.Tile && Type == Instance.Type;
	}

	bool operator==(const FTileInstance& Instance) const
	{
		return Tile == Instance.Tile && Type == Instance.Type;
	}

	bool operator==(const int32 InIndex)
	{
		return Tile == InIndex;
	}

	bool operator==(const int32 InIndex) const
	{
		return Tile == InIndex;
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
	FTileSetup(ETileType InType = ETileType::TT_Normal, UStaticMesh* InMesh = nullptr)
	{ 
		Type = InType;
		Mesh = InMesh;
	}
	ETileType Type;
	UStaticMesh* Mesh;
	// TODO: Add Material

	void operator=(UStaticMesh* StaticMesh)
	{
		Mesh = StaticMesh;
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


