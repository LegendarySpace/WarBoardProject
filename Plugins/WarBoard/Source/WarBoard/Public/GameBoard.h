// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Tiles.h"
#include "BiomeStructs.h"

#include "GameBoard.generated.h"

class UGridComponent;
class UEnviromentComponent;
class UNavSystem;
class UMovementSystemComponent;
class UAttackSystemComponent;

/**
 * 
 */
UCLASS()
class WARBOARD_API AGameBoard : public AActor
{
	GENERATED_BODY()
	
public:
	AGameBoard();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void ChangeTile(FTileBiome Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void RemoveTile(FGCoord Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void Populate(TArray<FTileBiome> Tiles);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void DisplayAttacks(TArray<FGCoord> Tiles);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void ClearAttacks();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void DisplayMovement(TArray<FGCoord> Tiles);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void ClearMovement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WarBoard|GameBoard")
	TArray<FGCoord> GetCoords();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WarBoard|GameBoard")
	TArray<FTile> GetTiles();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		void SetGridComponent(TSubclassOf<UGridComponent> InGrid);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		void SetEnviromentComponent(TSubclassOf<UEnviromentComponent> InLayout);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		void SetNavigationSystem(TSubclassOf<UNavSystem> NavigationSys);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		void SetMovementSystem(TSubclassOf<UMovementSystemComponent> MoveSys);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		void SetAttackSystem(TSubclassOf<UAttackSystemComponent> AttackSys);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		UGridComponent* GetGridComponent() { return Grid; }

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		UEnviromentComponent* GetEnviromentComponent() { return Enviroment; }

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		UNavSystem* GetNavigationSystem() { return Navigation; }

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		UMovementSystemComponent* GetMovementSystem() { return MovementSystem; }

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		UAttackSystemComponent* GetAttackSystem() { return AttackSystem; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	float Padding;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Board")
	UGridComponent* Grid;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Board")
	UEnviromentComponent* Enviroment;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Board")
	UNavSystem* Navigation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Board")
	UMovementSystemComponent* MovementSystem;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Board")
	UAttackSystemComponent* AttackSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	TArray<FTileBiome> BiomeLocations;
};
