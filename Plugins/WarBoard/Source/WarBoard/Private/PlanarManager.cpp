// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanarManager.h"

#include "Tiles.h"
#include "WarBoardLibrary.h"

// Sets default values
UPlanarManager::UPlanarManager()
{
 	// Never tick
	PrimaryComponentTick.bCanEverTick = false;

	PlaneMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/WarBoard/Material/TileMat.TileMat'")).Object;

}

void UPlanarManager::SetPlaneType(ETileShape Shape)
{
	// Determine Base Tile
	UStaticMesh* mesh;
	switch (Shape)
	{
	case ETileShape::Square:
		mesh = LoadObject<UStaticMesh>(NULL, TEXT("/WarBoard/StaticMesh/SquarePlane.SquarePlane"), NULL, LOAD_None, NULL);
		break;
	case ETileShape::Hex_Hor:
		mesh = LoadObject<UStaticMesh>(NULL, TEXT("/WarBoard/StaticMesh/HexPlane_Hori.HexPlane_Hori"), NULL, LOAD_None, NULL);
		break; 
	case ETileShape::Hex_Vert:
		mesh = LoadObject<UStaticMesh>(NULL, TEXT("/WarBoard/StaticMesh/HexPlane_Vert.HexPlane_Vert"), NULL, LOAD_None, NULL);
		break; 
	case ETileShape::Triangle:
	default:
		mesh = nullptr;
		break;
	}
	this->SetStaticMesh(mesh);
	if (PlaneMaterial) this->SetMaterial(0, PlaneMaterial);
}

void UPlanarManager::Populate(TArray<FGCoord> Choices)
{
	Clear();
	if (bUseDefaultShape) SetPlaneType(WarBoardLib::GetTileShape());

	Populate_Implementation(Choices);
}

void UPlanarManager::Populate_Implementation(TArray<FGCoord> Choices)
{
	for (auto Tile : Choices)
	{
		this->AddInstance(CalculateTransform(Tile));
	}
}

void UPlanarManager::Clear()
{
	this->ClearInstances();
}

// Called when the game starts or when spawned
void UPlanarManager::BeginPlay()
{
	Super::BeginPlay();
	
	SetPlaneType(WarBoardLib::GetTileShape());
}

FTransform UPlanarManager::CalculateTransform(FTile Tile)
{
	float size = WarBoardLib::GetTileSize();
	if (Padding >= 1) size -= Padding;
	else if (Padding > 0 && Padding < 1) size -= size * Padding;
	FVector scale = FVector(size / 100.f);
	return FTransform(FRotator(0), Tile.ToWorld(), scale);
}

