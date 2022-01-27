// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include <vector>

#include "Tiles.h"
#include "WarBoardLibrary.h"

#include "GridComponent.generated.h"

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
	FTile Tile = FTile();

	UPROPERTY(BlueprintReadWrite)
	float CellSize = WarBoardLib::GetTileSize();

	UPROPERTY(BlueprintReadWrite)
	float LineThickness = 10;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bProjectGrid = false;

	TArray<FVector> LineVertices;
	TArray<int32> LineTriangles;
	TArray<FVector> Polygon;

	void BuildCell(ETileShape Shape, float Size = -1, float Thickness = -1, float Padding = 0);

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

	void operator=(const FGCoord Coord)
	{
		this->Tile = Coord;
		this->BuildPolygonLines();
	}

	void operator=(const FCubic Cubic)
	{
		this->Tile = Cubic;
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

	// UPDATE: Add operators for cubic, coord, and index
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

	bool operator==(const FGCoord& Coord)
	{
		return this->Tile == Coord;
	}

	bool operator==(const FCubic& Cubic)
	{
		return this->Tile == Cubic;
	}

	bool operator==(const FTile& InTile)
	{
		return this->Tile == InTile;
	}

};


/**
*
*/
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WARBOARD_API UGridComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UGridComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void AddCell(FGCoord Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void RemoveCell(FGCoord Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void RebuildCells();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void Populate(TArray<FGCoord> Tiles);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void SetPadding(float InPadding);

protected:
	int32 GetNumUnset();
	int32 GetFirstUnsetID();
	void DisplayCell(FGridCell Cell);
	void CleanUpArray();

public:
	UPROPERTY(EditAnywhere)
	float LineThickness = 10.0;

	UPROPERTY(EditAnywhere)
	float CellPadding = 0.0;

private:
	TArray<TOptional<FGridCell>> CellArray;

	// UPGRADE: Add Cell SelectedCell, has Setter that builds cell to update details at location
};
