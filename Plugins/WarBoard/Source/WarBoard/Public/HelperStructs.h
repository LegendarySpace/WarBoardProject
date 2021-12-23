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

public:
	int32 ToIndex();
	FVector2D ToRC();
	FVector ToCubic();
	FVector ToWorld(bool TileCenter = true);
	static void SetTileSize(float Size);

private:
	int32 Row;
	int32 Col;
	static float Radius;
	static float Height;
	static float Width;
	static ETileShape Shape;
};


USTRUCT(BlueprintType)
struct FTileInstance
{
	GENERATED_BODY()

public:
	FTileInstance(int32 InIndex = 0, ETileType InType = ETileType::TT_Normal)
	{
		Index = InIndex;
		Type = InType;
	}

	int32 Index;
	ETileType Type;

	bool operator==(const int32 InIndex)
	{
		return Index == InIndex;
	}

	bool operator==(const ETileType InType)
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
};


