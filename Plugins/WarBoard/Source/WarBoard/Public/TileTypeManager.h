// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ETileType.h"
#include "TileTypeManager.generated.h"

class UHierarchicalInstancedStaticMeshComponent;
class UStaticMesh;

UCLASS()
class WARBOARD_API ATileTypeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileTypeManager();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Type Manager")
	void SetupInstance(ETileType TileType, UStaticMesh* Mesh,  float TileSize);

	void SetMesh(UStaticMesh* Mesh);

	void SetMat(UMaterial* Mat);

	// Index of tile to add
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Type Manager")
	void Add(int32 Index);

	// Index of tile to remove
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Type Manager")
	void Remove(int32 Index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	float Size = 200.f;
	TArray<int32> IDs;
	ETileType Type = ETileType::TT_Normal;

	UPROPERTY(BlueprintReadOnly, Category = "WarBoard|Type Manager")
	UHierarchicalInstancedStaticMeshComponent* HISM;


};
