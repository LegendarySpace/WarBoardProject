// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanarManager.h"
#include "MovementManager.generated.h"

/**
*		Highlights potential movement options
*/
UCLASS()
class WARBOARD_API AMovementManager : public APlanarManager
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovementManager();

	// Populate the Meshes
	virtual void Populate_Implementation(TArray<int32> Choices) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Spaces occupied by enemy characters are valid movement options
	bool bCanMoveToEnemy = false;

	// Spaces occupied by allied characters are valid movement options
	bool bCanMoveToAlly = false;

};
