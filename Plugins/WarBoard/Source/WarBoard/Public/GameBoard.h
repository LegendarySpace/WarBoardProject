// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Tiles.h"
#include "BiomeStructs.h"

#include "GameBoard.generated.h"

class UGridComponent;
class UEnvironmentComponent;
class UGridNavigationSystemComponent;
class UMovementTargetingComponent;
class UAttackTargetingComponent;

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
	void RemoveTile(FOrtho Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void Populate(TArray<FTileBiome> Tiles);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void DisplayAttacks(TArray<FOrtho> Tiles);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void ClearAttacks();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void DisplayMovement(TArray<FOrtho> Tiles);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
	void ClearMovement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WarBoard|GameBoard")
	TArray<FOrtho> GetCoords();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WarBoard|GameBoard")
	TArray<FTile> GetTiles();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		void SetGridComponent(TSubclassOf<UGridComponent> InGrid);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		void SetEnvironmentComponent(TSubclassOf<UEnvironmentComponent> InLayout);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		void SetNavigationSystem(TSubclassOf<UGridNavigationSystemComponent> NavigationSys);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		void SetMovementSystem(TSubclassOf<UMovementTargetingComponent> MoveSys);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		void SetAttackSystem(TSubclassOf<UAttackTargetingComponent> AttackSys);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		UGridComponent* GetGridComponent() { return Grid; }

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		UEnvironmentComponent* GetEnvironmentComponent() { return Environment; }

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		UGridNavigationSystemComponent* GetNavigationSystem() { return Navigation; }

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		UMovementTargetingComponent* GetMovementSystem() { return MovementSystem; }

	UFUNCTION(BlueprintCallable, Category = "WarBoard|GameBoard")
		UAttackTargetingComponent* GetAttackSystem() { return AttackSystem; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	float Padding;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Board")
	UGridComponent* Grid;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Board")
	UEnvironmentComponent* Environment;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Board")
	UGridNavigationSystemComponent* Navigation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Board")
	UMovementTargetingComponent* MovementSystem;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Board")
	UAttackTargetingComponent* AttackSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	TArray<FTileBiome> BiomeLocations;
};
