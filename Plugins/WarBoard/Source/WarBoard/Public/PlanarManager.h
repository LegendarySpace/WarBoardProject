// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"

#include "Tiles.h"

#include "PlanarManager.generated.h"

/**
*		Highlights selection options
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WARBOARD_API UPlanarManager : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPlanarManager();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	UMaterialInterface* PlaneMaterial;

public:
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Plane")
	void SetPlaneType(ETileShape Shape);

	// Populate the Meshes as a batch, because they are not persistent cannot be individually added or removed
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Plane")
	void Populate(TArray<FGCoord> Choices);
	virtual void Populate_Implementation(TArray<FGCoord> Choices); // Virtual override

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Plane")
	void Clear();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Plane")
	void SetPadding(float InPadding) { this->Padding = InPadding; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTransform CalculateTransform(FTile Tile);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bUseDefaultShape = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float Padding = .1;

};
