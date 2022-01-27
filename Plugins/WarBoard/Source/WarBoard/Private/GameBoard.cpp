// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard.h"

#include "UObject/ConstructorHelpers.h"

#include "Tiles.h"
#include "GridComponent.h"
#include "EnviromentComponent.h"
#include "NavSystemComponent.h"
#include "MovementSystemComponent.h"
#include "AttackSystemComponent.h"
#include "WarBoardLibrary.h"

AGameBoard::AGameBoard()
{
	// Should never tick
	PrimaryActorTick.bCanEverTick = false;

	auto Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Grid = CreateDefaultSubobject<UGridComponent>("Grid");
	Grid->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Enviroment = CreateDefaultSubobject<UEnviromentComponent>("Enviroment");
	Enviroment->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Navigation = CreateDefaultSubobject<UNavSystem>("NavSystem");
	Navigation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MovementSystem = CreateDefaultSubobject<UMovementSystemComponent>("MovementSystem");
	MovementSystem->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	AttackSystem = CreateDefaultSubobject<UAttackSystemComponent>("AttackSystem");
	AttackSystem->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

void AGameBoard::ChangeTile(FTileBiome Tile)
{
	BiomeLocations.AddUnique(Tile);
	Enviroment->ChangeTile(Tile);
	Grid->AddCell(Tile.Coord);
	Navigation->AddNode(Tile.Coord);
}

void AGameBoard::RemoveTile(FGCoord Tile)
{
	int i = 0;
	if (BiomeLocations.Find(Tile, i))
	{
		BiomeLocations.RemoveAt(i);
		Enviroment->RemoveTile(Tile);
		Grid->RemoveCell(Tile);
		Navigation->RemoveNode(Tile);
	}
}

void AGameBoard::Populate(TArray<FTileBiome> Tiles)
{
	for (auto Tile : Tiles)
	{
		ChangeTile(Tile);
	}
}

void AGameBoard::DisplayAttacks(TArray<FGCoord> Tiles)
{
	AttackSystem->Populate(Tiles);
}

void AGameBoard::ClearAttacks()
{
	AttackSystem->Clear();
}

void AGameBoard::DisplayMovement(TArray<FGCoord> Tiles)
{
	MovementSystem->Populate(Tiles);
}

void AGameBoard::ClearMovement()
{
	MovementSystem->Clear();
}

void AGameBoard::SetGridComponent(TSubclassOf<UGridComponent> InGrid)
{
	delete Grid;
	Grid = NewObject<UGridComponent>(this, InGrid);
	Grid->Populate(GetCoords());
}

void AGameBoard::SetEnviromentComponent(TSubclassOf<UEnviromentComponent> InLayout)
{
	delete Enviroment;
	Enviroment = NewObject<UEnviromentComponent>(this, InLayout);
	Enviroment->Populate(BiomeLocations);
}

void AGameBoard::SetNavigationSystem(TSubclassOf<UNavSystem> Router)
{
	// Clear any current route manager first
	delete Navigation;
	Navigation = NewObject<UNavSystem>(this, Router);
	Navigation->Populate(GetTiles());
}

void AGameBoard::SetMovementSystem(TSubclassOf<UMovementSystemComponent> MoveSys)
{
	delete MovementSystem;
	MovementSystem = NewObject<UMovementSystemComponent>(this, MoveSys);
}

void AGameBoard::SetAttackSystem(TSubclassOf<UAttackSystemComponent> AttackSys)
{
	delete AttackSystem;
	AttackSystem = NewObject<UAttackSystemComponent>(this, AttackSys);
}

void AGameBoard::BeginPlay()
{
	Super::BeginPlay();

}

TArray<FGCoord> AGameBoard::GetCoords()
{
	TArray<FGCoord> Coords = TArray<FGCoord>();
	for (auto TB : BiomeLocations)
	{
		Coords.AddUnique(TB.Coord);
	}

	return Coords;
}

TArray<FTile> AGameBoard::GetTiles()
{
	TArray<FTile> Tiles = TArray<FTile>();
	for (auto TB : BiomeLocations)
	{
		Tiles.AddUnique(FTile(TB.Coord));
	}

	return Tiles;
}

