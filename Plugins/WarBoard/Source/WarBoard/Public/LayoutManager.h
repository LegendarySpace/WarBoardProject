// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ETileType.h"
#include "TileShape.h"
#include "LayoutManager.generated.h"

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

	// Functions used by child, adds support for pathing system
	virtual void AddPathNode(int32 Index);
	virtual void RemovePathNode(int32 Index);

	// Index of bottom left corner of board
	UFUNCTION(BlueprintCallable, Category = "WarBoard|State")
	int32 GetOffset();

	// Full board size as vector
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WarBoard|State")
	FVector GetBoardSize();

	// Change Tile Type
	// If no Tile is present at index, it will make a new Tile of given Type
	UFUNCTION(BlueprintCallable, Category = "WarBoard|State")
	bool ChangeTile(int32 Index, ETileType Type);

	// Remove Tile
	UFUNCTION(BlueprintCallable, Category = "WarBoard|State")
	bool RemoveTile(int32 Index);

	// Assemble the Tile grid
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Initialize")
	void AssembleTiles();

	// Assemble the gridlines
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Initialize")
	void AssembleGrid();

	// Get Tiles
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Utility")
	TArray<int32> GetTiles();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = "Settings")
	float TileSize = 200.f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool CenterBoard = true;

	UPROPERTY(EditAnywhere, Category = "Settings")
	ETileShape TileShape = ETileShape::Square;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool DebugTileIndexes = false;

	UStaticMesh* BaseMesh;


	/**
	*		Maps used for tracking board info
	*/
	TMap<int32, ETileType> TileMap;
	TMap<int32, ETileType> BoardDefault;
	TMap<int32, UTextRenderComponent*> TextMap;
	TMap<ETileType, UStaticMesh*> TileMeshes;

	TArray<ATileTypeManager*> Managers;

};
