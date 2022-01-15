// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ETileType.h"
#include "TileShape.h"
#include "HelperStructs.h"

#include "LayoutManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTileAddDelegate, FGCoord, Coord);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTileRemoveDelegate, FGCoord, Coord);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTileChangeDelegate, FGCoord, Coord, ETileType, Type);

class ATileTypeManager;
class UStaticMesh;
class UTextRenderComponent;
class APathFinder;

UCLASS()
class WARBOARD_API ALayoutManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALayoutManager();

	// Change Tile Type
	// If no Tile is present at index, it will make a new Tile of given Type
	UFUNCTION(BlueprintCallable, Category = "WarBoard|State")
	bool ChangeTile(FTileInstance Instance);

	// Remove Tile
	UFUNCTION(BlueprintCallable, Category = "WarBoard|State")
	bool RemoveTile(FGCoord Coord);

	// Assemble the Tile grid
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Initialize")
	void AssembleTiles();

	// Get Tiles
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Utility")
	TArray<FGCoord> GetTiles();

private:
	void BuildTileManagerArray();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = "Settings")
	bool CenterBoard = true;

	// Thickness of line inside tile
	UPROPERTY(EditAnywhere, Category = "Settings")
	float GridThickness = 4.f;

	// Grid distance from tile edge
	UPROPERTY(EditAnywhere, Category = "Settings")
	float GridPadding = 0.0;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool DebugTileIndexes = false;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FTileAddDelegate OnTileAdd;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FTileRemoveDelegate OnTileRemove;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FTileChangeDelegate OnTileChange;


	/**
	*		Maps used for tracking board info
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	TArray<FTileInstance> TileMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	TArray<FTileInstance> BoardDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	TArray<FTileSetup> TileMeshes;	// TODO: Add Mat to setup

	TArray<ATileTypeManager*> Managers;
	TMap<int32, UTextRenderComponent*> TextMap;
};
