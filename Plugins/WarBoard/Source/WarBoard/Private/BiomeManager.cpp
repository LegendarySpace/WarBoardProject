// Fill out your copyright notice in the Description page of Project Settings.


#include "BiomeManager.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;

// Sets default values
UBiomeManager::UBiomeManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	FBiomeSetup Setup = FBiomeSetup();
	Setup.Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/WarBoard/StaticMesh/SquareTile.SquareTile'")).Object;
	Setup.Mat = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("StaticMesh'/WarBoard/Material/TileMat.TileMat'")).Object;

	SetupInstance(Setup);
}

void UBiomeManager::SetupInstance(FBiomeSetup Setup) // UPDATE: On tileshape change call setupInstance 
{
	Biome = Setup.Biome;
	if (Setup.Mesh == nullptr)
	{
		// Determine Base Tile (Used when no mesh is provided)
		switch (GetTileShape())
		{
		case ETileShape::Square:
			Setup.Mesh = LoadObject<UStaticMesh>(NULL, TEXT("/WarBoard/StaticMesh/SquareTile.SquareTile"), NULL, LOAD_None, NULL);
			break;
		case ETileShape::Hex_Hor:
			Setup.Mesh = LoadObject<UStaticMesh>(NULL, TEXT("/WarBoard/StaticMesh/HexTile_Hori.HexTile_Hori"), NULL, LOAD_None, NULL);
			break;
		case ETileShape::Hex_Vert:
			Setup.Mesh = LoadObject<UStaticMesh>(NULL, TEXT("/WarBoard/StaticMesh/HexTile_Vert.HexTile_Vert"), NULL, LOAD_None, NULL);
			break;
		}
	}
	if (Setup.Mesh != nullptr) this->SetStaticMesh(Setup.Mesh);
	if (Setup.Mat != nullptr) this->SetMaterial(0, Setup.Mat);
}

void UBiomeManager::Populate(TArray<FGCoord> Tiles)
{
	for (auto Tile : Tiles)
	{
		AddTile(Tile);
	}
}

void UBiomeManager::AddTile(FGCoord Tile)
{
	if (!InstanceIndexes.Contains(Tile))
	{
		InstanceIndexes.AddUnique(Tile);

		BuildTile(Tile);
	}

}

FTransform UBiomeManager::CalculateTransform(FTile Tile)
{
	float Pad = Padding;
	if (Padding > 0 && Padding < 1) Pad = GetTileSize() * (Padding / 2);
	if (Padding < 0 || Padding > MeshSize) Pad = 0;
	FVector scale = FVector((GetTileSize() - (2 * Pad)) / MeshSize);
	FVector loc = Tile.ToWorld();
	// Tiles are assumed to have a thickness 1/10th its TileSize
	loc += FVector(0, 0, (-0.05 * GetTileSize()) + (Pad * 0.1));
	return FTransform(FRotator(0), loc, scale);
}

void UBiomeManager::BuildTile(FTile Tile)
{
	this->AddInstanceWorldSpace(CalculateTransform(Tile));
}

void UBiomeManager::RemoveTile(FGCoord TileToRemove)
{
	int32 InstanceIndex = 0;
	if (InstanceIndexes.Find(TileToRemove, InstanceIndex))
	{
		if (this->RemoveInstance(InstanceIndex))
		{
			InstanceIndexes.RemoveAt(InstanceIndex, 1);
			InstanceIndexes.Shrink();
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("Remove tile failed: Tile not in InstanceIndexes"));
}

void UBiomeManager::RebuildBiome()
{
	for (int32 i = 0; i < InstanceIndexes.Num(); i++)
	{
		this->UpdateInstanceTransform(i, CalculateTransform(InstanceIndexes[i]), true);
	}
}

void UBiomeManager::DisplayInstanceIndexes()
{
	UE_LOG(LogTemp, Warning, TEXT("     Instance Indexes     "));
	auto &insta = this->InstanceBodies;

	FTransform InstanceTransform;
	FTile InstanceTile;
	for (int InstanceBodiesIndex = 0; InstanceBodiesIndex < insta.Num(); InstanceBodiesIndex++)
	{
		if (this->GetInstanceTransform(InstanceBodiesIndex, InstanceTransform, true))
		{
			InstanceTile = InstanceTransform.GetLocation();
			UE_LOG(LogTemp, Warning, TEXT("Instance %d is at index %d"), InstanceBodiesIndex, InstanceTile.ToIndex());
		}
	}
}

void UBiomeManager::SetPadding(float InPadding)
{
	Padding = InPadding;
	RebuildBiome();
}

// UPGRADE: On Tick if debug mode call show on screen debug and display Index

