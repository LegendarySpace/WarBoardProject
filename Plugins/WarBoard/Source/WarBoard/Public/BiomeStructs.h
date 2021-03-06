// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Tiles.h"

#include "BiomeStructs.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Category = "WarBoard|Enums")
enum EBiome
{
	TT_Normal		UMETA(DisplayName = "Normal"),
	TT_Forest		UMETA(DisplayName = "Forest"),
	TT_Plains		UMETA(DisplayName = "Plains"),
	TT_Mountain		UMETA(DisplayName = "Mountain"),
	TT_Sea			UMETA(DisplayName = "Sea"),
	TT_Type_MAX		UMETA(Hidden)
};


USTRUCT(BlueprintType)
struct FTileBiome
{
	GENERATED_BODY()

public:
	FTileBiome(FOrtho InCoord = FOrtho(), EBiome InType = EBiome::TT_Normal)
	{
		Coord = InCoord;
		Biome = InType;
	}

	FTileBiome(FTile InTile, EBiome InType = EBiome::TT_Normal)
	{
		Coord = InTile.ToOrtho();
		Biome = InType;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarBoard|Biome")
		FOrtho Coord;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarBoard|Biome")
		TEnumAsByte<EBiome> Biome;

	void operator=(EBiome InType)
	{
		Biome = InType;
	}

	bool operator==(const FTileBiome& Instance)
	{
		return Coord == Instance.Coord;
	}

	bool operator==(const FTileBiome& Instance) const
	{
		return Coord == Instance.Coord;
	}

	bool operator==(const int32 InIndex)
	{
		return Coord == FTile(InIndex).ToOrtho();
	}

	bool operator==(const int32 InIndex) const
	{
		return Coord == FTile(InIndex).ToOrtho();
	}

	bool operator==(const FOrtho InCoord)
	{
		return Coord == InCoord;
	}

	bool operator==(const FOrtho InCoord) const
	{
		return Coord == InCoord;
	}

	bool operator==(const EBiome& InType)
	{
		return Biome == InType;
	}

	bool operator==(const EBiome& InType) const
	{
		return Biome == InType;
	}

};

USTRUCT(BlueprintType)
struct FBiomeSetup
{
	GENERATED_BODY()

public:
	FBiomeSetup(EBiome InBiome = EBiome::TT_Normal, UStaticMesh* InMesh = nullptr, UMaterialInterface* InMat = nullptr)
	{
		Biome = InBiome;
		Mesh = InMesh;
		Mat = InMat;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarBoard|Biome")
		TEnumAsByte<EBiome> Biome;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarBoard|Biome")
		UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarBoard|Biome")
		UMaterialInterface* Mat;

	void operator=(UStaticMesh* StaticMesh)
	{
		Mesh = StaticMesh;
	}

	void operator=(UMaterialInterface* Material)
	{
		Mat = Material;
	}

	bool operator==(FBiomeSetup Setup)
	{
		return Biome == Setup.Biome;
	}

	bool operator==(FBiomeSetup Setup) const
	{
		return Biome == Setup.Biome;
	}

	bool operator==(EBiome InType)
	{
		return Biome == InType;
	}

	bool operator==(EBiome InType) const
	{
		return Biome == InType;
	}

};


