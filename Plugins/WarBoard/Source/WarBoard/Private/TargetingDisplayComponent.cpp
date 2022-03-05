// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingDisplayComponent.h"

#include "Tiles.h"
#include "WarBoardLibrary.h"

// Sets default values
UTargetingDisplayComponent::UTargetingDisplayComponent()
{
 	// Never tick
	PrimaryComponentTick.bCanEverTick = false;

	PlaneMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("Material'/WarBoard/Material/TileMat.TileMat'")).Object;

}

void UTargetingDisplayComponent::SetPlaneType(ETileShape Shape)
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

void UTargetingDisplayComponent::Populate(TArray<FGCoord> Choices)
{
	TArray<FTile> Tiles;
	for (auto Selection : Choices) Tiles.Add(FTile(Selection));
	Populate(Tiles);
}

void UTargetingDisplayComponent::Populate(TArray<FTile> Choices)
{
	Clear();
	if (bUseDefaultShape) SetPlaneType(WarBoardLib::GetTileShape());

	for (auto Tile : Choices)
	{
		this->AddInstance(CalculateTransform(Tile));
	}
}

void UTargetingDisplayComponent::Populate(TArray<FCubic> Choices)
{
	TArray<FTile> Tiles;
	for (auto Selection : Choices) Tiles.Add(FTile(Selection));
	Populate(Tiles);
}

void UTargetingDisplayComponent::Clear()
{
	this->ClearInstances();
}

// Called when the game starts or when spawned
void UTargetingDisplayComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetPlaneType(WarBoardLib::GetTileShape());
}

FTransform UTargetingDisplayComponent::CalculateTransform(FTile Tile)
{
	float size = WarBoardLib::GetTileSize();
	if (Padding >= 1) size -= Padding;
	else if (Padding > 0 && Padding < 1) size -= size * Padding;
	FVector scale = FVector(size / 100.f);
	return FTransform(FRotator(0), Tile.ToWorld(), scale);
}

