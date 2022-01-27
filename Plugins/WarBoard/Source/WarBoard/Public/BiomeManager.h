// Fill out your copyright notice in the Description page of Project Settings.
// Manages a given group of tiles all possessing the same mesh

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"

#include "BiomeStructs.h"
#include "Tiles.h"

#include "BiomeManager.generated.h"

class UInstancedStaticMeshComponent;
class UStaticMesh;

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
	void Populate(TArray<FGCoord> Tiles);

	// Index of tile to add
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void AddTile(FGCoord Tile);

	// Index of tile to remove
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void RemoveTile(FGCoord TileToRemove);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void RebuildBiome();

	// Debugging tool displays information about each instance
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
	void DisplayInstanceIndexes();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FGCoord> GetInstanceIndexes() { return InstanceIndexes; }

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Biome Manager")
		void SetPadding(float InPadding);

protected:
	FTransform CalculateTransform(FTile Tile);

	void BuildTile(FTile Tile);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Padding = 0;

	UPROPERTY(EditDefaultsOnly)
	float MeshSize = 100;

	EBiome Biome = EBiome::TT_Normal;

	UPROPERTY(EditAnywhere)
	bool bDebugMode = false;

private:
	TArray<FGCoord> InstanceIndexes;

};
