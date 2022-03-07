// Fill out your copyright notice in the Description page of Project Settings.
// Manages a given group of tiles all possessing the same mesh

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"

#include "BiomeStructs.h"
#include "Tiles.h"

#include "BiomeManager.generated.h"

class UInstancedStaticMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WARBOARD_API UBiomeManager : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UBiomeManager();

public:
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void SetupInstance(FBiomeSetup Setup);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void Populate(TArray<FOrtho> Tiles);
	void Populate(TArray<FTile> Tiles);
	void Populate(TArray<FCubic> Tiles);

	// Index of tile to add
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void AddTile(FOrtho Tile);
	void AddTile(FTile Tile);
	void AddTile(FCubic Tile);

	// Index of tile to remove
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void RemoveTile(FOrtho TileToRemove);
	void RemoveTile(FTile TileToRemove);
	void RemoveTile(FCubic TileToRemove);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void RebuildBiome();

	// Debugging tool displays information about each instance
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void DisplayInstanceIndexes();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WarBoard|Biome Manager")
	TArray<FOrtho> GetInstanceIndexes() { return InstanceIndexes; }

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void SetPadding(float InPadding);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void ClearTiles();

protected:
	FTransform CalculateTransform(FTile Tile);

	void BuildTile(FTile Tile);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float Padding = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float MeshSize = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TEnumAsByte<EBiome> Biome = EBiome::TT_Normal;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bCenterVerticalOffset = true;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float VerticalOffset = 0.f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bDebugMode = false;

protected:
	TArray<FOrtho> InstanceIndexes;

};
