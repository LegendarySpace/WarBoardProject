// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingDisplayComponent.h"
#include "MovementTargetingComponent.generated.h"

/**
*		Highlights potential movement options
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WARBOARD_API UMovementTargetingComponent : public UTargetingDisplayComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UMovementTargetingComponent();

	// Populate the Meshes
	void Populate(TArray<FOrtho> Choices);
	void Populate(TArray<FTile> Choices);
	void Populate(TArray<FCubic> Choices);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Spaces occupied by enemy characters are valid movement options
	bool bCanMoveToEnemy = false;

	// Spaces occupied by allied characters are valid movement options
	bool bCanMoveToAlly = false;

};
