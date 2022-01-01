// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "LayoutManager.h"

#include "GameBoard.generated.h"

class APathFinder;
class AMovementManager;
class AAttackManager;

/**
 * 
 */
UCLASS()
class WARBOARD_API AGameBoard : public ALayoutManager
{
	GENERATED_BODY()
	
public:
	AGameBoard();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void SetPathFinder(TSubclassOf<APathFinder> Router);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void SetMoveManager(TSubclassOf<AMovementManager> MoveSys);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	void SetAttackManager(TSubclassOf<AAttackManager> AttackSys);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Pathing")
	APathFinder* GetRouteManager();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Movement")
	AMovementManager* GetMoveManager();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Attacking")
	AAttackManager* GetAttackManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, Instanced, Category = "Pathing")
	APathFinder* RouteManager;

	UPROPERTY(BlueprintReadOnly, Instanced, Category = "Movement")
	AMovementManager* MoveManager;

	UPROPERTY(BlueprintReadOnly, Instanced, Category = "Attacking")
	AAttackManager* AttackManager;
};
