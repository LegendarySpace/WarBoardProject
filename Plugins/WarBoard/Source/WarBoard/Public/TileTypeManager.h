// Fill out your copyright notice in the Description page of Project Settings.
// Manages a given group of tiles all possessing the same mesh

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ETileType.h"
#include "HelperStructs.h"

#include "TileTypeManager.generated.h"

class UInstancedStaticMeshComponent;
class UStaticMesh;
class UTextRenderComponent;

UCLASS()
class WARBOARD_API ATileTypeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileTypeManager();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* InstancedMeshComp;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetInstanceIndexes)
	TArray<int32> InstanceIndexes;

public:
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Type Manager")		// TODO: Should use TileSetup instead
	void SetupInstance(ETileType TileType, UStaticMesh* Mesh);

	void SetMesh(UStaticMesh* Mesh);

	void SetMat(UMaterial* Mat);

	// Index of tile to add
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Type Manager")
	void AddTile(FTile Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Type Manager")
	void BuildTile(FTile Tile);

	// Index of tile to remove
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Type Manager")
	void RemoveTile(FTile TileToRemove);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Type Manager")
	void DisplayInstanceIndexes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	float MeshSize = 100.f;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<int32> GetInstanceIndexes() { return InstanceIndexes; }

	ETileType Type = ETileType::TT_Normal;

	UPROPERTY(EditAnywhere)
	bool bDebugMode = false;

	TArray<int32> RenderIndexArray;
};
