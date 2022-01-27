// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanarManager.h"
#include "MovementSystemComponent.generated.h"

/**
*		Highlights potential movement options
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WARBOARD_API UMovementSystemComponent : public UPlanarManager
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UMovementSystemComponent();

	// Populate the Meshes
	virtual void Populate_Implementation(TArray<FGCoord> Choices) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Spaces occupied by enemy characters are valid movement options
	bool bCanMoveToEnemy = false;

	// Spaces occupied by allied characters are valid movement options
	bool bCanMoveToAlly = false;

};
