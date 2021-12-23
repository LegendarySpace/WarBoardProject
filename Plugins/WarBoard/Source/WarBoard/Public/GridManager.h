// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>

#include "EGridShape.h"
#include "TileShape.h"

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
	FGridCell(int32 Index = 0) { CellIndex = Index; }

	UPROPERTY(BlueprintReadWrite)
	int32 CellIndex;

	UPROPERTY(BlueprintReadWrite)
	float CellSize;

	UPROPERTY(BlueprintReadWrite)
	float LineThickness;


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
		this->CellIndex = Index;
		this->BuildPolygonLines();
	}

	void operator=(const FGridCell Cell)
	{
		this->CellIndex = Cell.CellIndex;
		this->BuildPolygonLines();
	}

	FGridCell operator+(const int32 Index)
	{
		FGridCell Cell = FGridCell();
		Cell = *this;
		Cell.CellIndex = this->CellIndex + Index;
		Cell.BuildPolygonLines();
		return Cell;
	}

	FGridCell& operator+=(const int32 Index)
	{
		CellIndex += Index;
		BuildPolygonLines();
		return *this;
	}

	FGridCell operator-(const int32 Index)
	{
		FGridCell Cell = FGridCell();
		Cell = *this;
		Cell.CellIndex = this->CellIndex - Index;
		Cell.BuildPolygonLines();
		return Cell;
	}

	FGridCell& operator-=(const int32 Index)
	{
		CellIndex -= Index;
		BuildPolygonLines();
		return *this;
	}

	bool operator==(const FGridCell& Cell) const
	{
		return this->CellIndex == Cell.CellIndex;
	}

	bool operator==(const int32& Index) const
	{
		return this->CellIndex == Index;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> InitialCells;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void AddCell(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void RemoveCell(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void RebuildCells();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Grid")
	void DisplayInitialCells();

protected:
	int32 GetNumUnset();

	int32 GetFirstUnsetID();

	void DisplayCell(FGridCell Cell);

	void CleanUpArray();

public:
	UPROPERTY(EditAnywhere)
	float TileSize = 200.0;

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
