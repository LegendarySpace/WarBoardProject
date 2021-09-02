// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "ETileType.h"
#include "LayoutManager.generated.h"

class ATileTypeManager;

UCLASS()
class WARBOARD_API ALayoutManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALayoutManager();

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
	UPROPERTY(EditAnywhere, Category = "WarBoard|Settings")
	float TileSize = 200.f;

	UPROPERTY(EditAnywhere, Category = "WarBoard|Settings")
	bool CenterBoard = true;

	UPROPERTY(EditAnywhere, Category = "WarBoard|Settings")
	bool HexTiles = false;

	// When using HexTiles should the orientation be horizontal or vertical
	UPROPERTY(EditAnywhere, Category = "WarBoard|Settings")
	bool HexVert = false;

	UPROPERTY(EditAnywhere, Category = "WarBoard|Debug")
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
