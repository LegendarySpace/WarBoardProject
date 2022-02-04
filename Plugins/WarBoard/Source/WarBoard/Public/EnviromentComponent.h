// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "BiomeStructs.h"
#include "Tiles.h"

#include "EnviromentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTileAddDelegate, FGCoord, Coord);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTileRemoveDelegate, FGCoord, Coord);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTileChangeDelegate, FGCoord, Coord, EBiome, Biome);

class UBiomeManager;
class UStaticMesh;


/**
*
*/
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WARBOARD_API UEnviromentComponent : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UEnviromentComponent();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Biomes")
	UBiomeManager* BiomeManager0;

	UPROPERTY(BlueprintReadOnly, Category = "Biomes")
	UBiomeManager* BiomeManager1;

	UPROPERTY(BlueprintReadOnly, Category = "Biomes")
	UBiomeManager* BiomeManager2;

	UPROPERTY(BlueprintReadOnly, Category = "Biomes")
	UBiomeManager* BiomeManager3;

	UPROPERTY(BlueprintReadOnly, Category = "Biomes")
	UBiomeManager* BiomeManager4;

	UPROPERTY(BlueprintReadOnly, Category = "Biomes")
	UBiomeManager* BiomeManager5;

	UPROPERTY(BlueprintReadOnly, Category = "Biomes")
	UBiomeManager* BiomeManager6;

	UPROPERTY(BlueprintReadOnly, Category = "Biomes")
	UBiomeManager* BiomeManager7;

	UPROPERTY(BlueprintReadOnly, Category = "Biomes")
	UBiomeManager* BiomeManager8;

	UPROPERTY(BlueprintReadOnly, Category = "Biomes")
	UBiomeManager* BiomeManager9;

public:
	// Biomes must be initialized before use
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Environment")
	void InitializeBiomes();

	// Change Tile Biome, if no Tile is present one will be made
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Environment")
	bool ChangeTile(FTileBiome Instance);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Environment")
	bool RemoveTile(FGCoord Tile);
	bool RemoveTile(FTile Tile);
	bool RemoveTile(FCubic Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Environment")
	void Populate(TArray<FTileBiome> Tiles);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Environment")
	TArray<FGCoord> GetTiles();

	// Get Biome from Coord
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Environment")
	EBiome GetBiome(FGCoord Tile);
	EBiome GetBiome(FTile Tile);
	EBiome GetBiome(FCubic Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Environment")
	bool IsValid(FGCoord Tile);
	bool IsValid(FTile Tile);
	bool IsValid(FCubic Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Environment")
	void SetPadding(float Padding);

private:
	void BuildBiomeManagers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UBiomeManager* GetBiomeManager(EBiome Biome);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")	// UPGRADE: Utilize this
	bool DebugTileIndexes = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float Padding = 0;

	///		Delegates
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FTileAddDelegate OnTileAdd;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FTileRemoveDelegate OnTileRemove;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FTileChangeDelegate OnTileChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<FBiomeSetup> BiomeSettings;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enviroment")
	TArray<FTileBiome> BiomeTiles;

};
