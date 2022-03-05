// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Public/Tiles.h"
#include "../Public/BiomeStructs.h"

#include "LevelCreatorBase.generated.h"

class UTraceFloor;
class UEnvironmentComponent;
class UBiomeManager;
class UMaterialInterface;

UCLASS()
class WARBOARD_API ALevelCreatorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelCreatorBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UTraceFloor* Floor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UEnvironmentComponent* Environment;

	UBiomeManager* SelectionHighlighter;

public:
	UFUNCTION(BlueprintCallable, Category = "WarBoard|LevelCreator")
	void AddTileHighlight(FGCoord Tile);
	void AddHighlight(FTile Tile);
	void AddHighlight(FGCoord Tile);
	void AddHighlight(FCubic Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|LevelCreator")
	void RemoveTileHighlight(FGCoord Tile);
	void RemoveHighlight(FTile Tile);
	void RemoveHighlight(FGCoord Tile);
	void RemoveHighlight(FCubic Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|LevelCreator")
	void ClearHighlights();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Settings")
	int32 FloorSize = 5;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<FBiomeSetup> Biomes;

	UPROPERTY(EditAnywhere, Category = "Settings")
	UMaterialInterface* HighlighterMaterial;

	TArray<FTile> MultiselectArray = TArray<FTile>();
};
