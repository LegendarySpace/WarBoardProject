// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingDisplayComponent.h"
#include "AttackTargetingComponent.generated.h"


/**
*		Highlights potential targeting options
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WARBOARD_API UAttackTargetingComponent : public UTargetingDisplayComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UAttackTargetingComponent();

	// Populate the Meshes
	void Populate(TArray<FGCoord> Choices);
	void Populate(TArray<FTile> Choices);
	void Populate(TArray<FCubic> Choices);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Allows targeting of allies
	bool bCanTargetAllies = false;

	// Allows targeting of unoccupied tiles
	bool bCanTargetEmptyTile = true;
};
