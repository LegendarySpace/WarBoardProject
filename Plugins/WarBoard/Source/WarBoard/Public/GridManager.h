// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>

#include "EGridShape.h"
#include "TileShape.h"
#include "HelperStructs.h"

#include "GridManager.generated.h"

class UProceduralMeshComponent;

/**
*	Cells Contain all information necessary for display
**/
USTRUCT(BlueprintType)
struct FGridCell
{
	GENERATED_BODY()

public:
	FGridCell(int32 Index = 0) { this->Tile = Index; }

	FGridCell(FTile InTile) { this->Tile = InTile; }

	UPROPERTY(BlueprintReadWrite)
	FTile Tile = FTile(0);

	UPROPERTY(BlueprintReadWrite)
	float CellSize = 200;

	UPROPERTY(BlueprintReadWrite)
	float LineThickness = 10;


	TArray<FVector> LineVertices;
	TArray<int32> LineTriangles;
	TArray<FVector> Polygon;

	void BuildCell(ETileShape Shape, float Size = 200, float Thickness = 10, float Padding = 0);

private:
	TArray<FVector>& GetCellVertices(const int32 Sides, const float RelativeRotationToFirstVertex = 0.0, const float PolygonRotation = 0.0);

	void ProjectVerticesOntoSurface(float Height);

public:
	void BuildPolygonLines();

	void AddLineDetails(const FVector Start, const FVector End);

public:

	void operator=(const int32 Index)
	{
		this->Tile = Index;
		this->BuildPolygonLines();
	}

	void operator=(const FTile InTile)
	{
		this->Tile = InTile;
		this->BuildPolygonLines();
	}

	void operator=(const FGridCell Cell)
	{
		this->Tile = Cell.Tile;
		this->BuildPolygonLines();
	}

	FGridCell operator+(const FTile InTile)
	{
		FGridCell Cell = FGridCell(InTile);
		Cell = *this;
		Cell.Tile = this->Tile + InTile;
		Cell.BuildPolygonLines();
		return Cell;
	}

	FGridCell& operator+=(const FTile InTile)
	{
		this->Tile += InTile;
		BuildPolygonLines();
		return *this;
	}

	FGridCell operator-(const FTile InTile)
	{
		FGridCell Cell = FGridCell(InTile);
		Cell = *this;
		Cell.Tile = this->Tile - InTile;
		Cell.BuildPolygonLines();
		return Cell;
	}

	FGridCell& operator-=(const FTile InTile)
	{
		this->Tile -= InTile;
		BuildPolygonLines();
		return *this;
	}

	bool operator==(const FGridCell& Cell)
	{
		return this->Tile == Cell.Tile;
	}

	bool operator==(const int32& Index)
	{
		return this->Tile == Index;
	}

	bool operator==(const FTile& InTile)
	{
		return this->Tile == InTile;
	}

};


UCLASS()
class WARBOARD_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* ProceduralMesh;

	// TODO: Change to FGCoord
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> InitialCells;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void AddCell(FTile Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void RemoveCell(FTile Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void RebuildCells();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void DisplayInitialCells();

protected:
	int32 GetNumUnset();

	int32 GetFirstUnsetID();

	void DisplayCell(FGridCell Cell);

	void CleanUpArray();

	// TODO: Add Initializer for these so they match from FTile
public:
	UPROPERTY(EditAnywhere)
	ETileShape TileShape = ETileShape::Square;

	UPROPERTY(EditAnywhere)
	float LineThickness = 20.0;

	UPROPERTY(EditAnywhere)
	float CellPadding = 0.0;

private:
	TArray<TOptional<FGridCell>> CellArray;

	// TODO: Add Cell SelectedCell, has Setter that builds cell to update details at location
};
