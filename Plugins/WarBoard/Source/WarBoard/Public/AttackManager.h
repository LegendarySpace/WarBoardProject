// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanarManager.h"
#include "AttackManager.generated.h"

/**
*		Highlights potential targeting options
*/
UCLASS()
class WARBOARD_API AAttackManager : public APlanarManager
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackManager();

	// Populate the Meshes
	virtual void Populate_Implementation(TArray<int32> Choices) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Allows targeting of allies
	bool bCanTargetAllies;

	// Allows targeting of unoccupied tiles
	bool bCanTargetEmptyTile;
};
