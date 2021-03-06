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

	UPROPERTY(BlueprintReadWrite, Category = "GridCell")
	FTile Tile = FTile();

	UPROPERTY(BlueprintReadWrite, Category = "GridCell")
	float CellSize = WarBoardLib::GetTileSize();

	UPROPERTY(BlueprintReadWrite, Category = "GridCell")
	float LineThickness = 10;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "GridCell")
	bool bProjectGrid = false;

	TArray<FVector> LineVertices;
	TArray<int32> LineTriangles;
	TArray<FVector> Polygon;
	FRotator CellRotation;

	void BuildCell(ETileShape Shape, float Size = -1, float Thickness = -1, float Padding = 0);

private:
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

	void operator=(const FOrtho Coord)
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

	bool operator==(const FOrtho& Coord)
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
	void AddCell(FOrtho Tile);
	void AddCell(FTile Tile);
	void AddCell(FCubic Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void RemoveCell(FOrtho Tile);
	void RemoveCell(FTile Tile);
	void RemoveCell(FCubic Tile);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void RebuildCells();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void Populate(TArray<FOrtho> Tiles);
	void Populate(TArray<FTile> Tiles);
	void Populate(TArray<FCubic> Tiles);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void SetPadding(float InPadding);

protected:
	int32 GetNumUnset();
	int32 GetFirstUnsetID();
	void DisplayCell(FGridCell Cell);
	void CleanUpArray();

public:
	UPROPERTY(EditAnywhere, Category = "Grid")
	float LineThickness = 10.0;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float CellPadding = 0.0;

private:
	TArray<TOptional<FGridCell>> CellArray;

	// UPGRADE: Add Cell SelectedCell, has Setter that builds cell to update details at location
};
