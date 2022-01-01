// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "TileShape.h"
#include "HelperStructs.h"

#include "PlanarManager.generated.h"

class UHierarchicalInstancedStaticMeshComponent;

/**
*		Highlights selection options
*/
UCLASS()
class WARBOARD_API APlanarManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanarManager();

	// Determines the basic plane used
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Settings")
		void SetPlaneType(ETileShape Shape);

	// Populate the Meshes
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Managers")
	void Populate(TArray<FTile> Choices);
	virtual void Populate_Implementation(TArray<FTile> Choices); // Virtual override

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Managers")
		void Clear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UHierarchicalInstancedStaticMeshComponent* HISM;

};
