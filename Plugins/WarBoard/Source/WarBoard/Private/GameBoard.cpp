// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard.h"

#include "UObject/ConstructorHelpers.h"

#include "Tiles.h"
#include "GridComponent.h"
#include "EnvironmentComponent.h"
#include "GridNavigationSystemComponent.h"
#include "MovementTargetingComponent.h"
#include "AttackTargetingComponent.h"
#include "WarBoardLibrary.h"

AGameBoard::AGameBoard()
{
	// Should never tick
	PrimaryActorTick.bCanEverTick = false;

	auto Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Grid = CreateDefaultSubobject<UGridComponent>("Grid");
	Grid->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Environment = CreateDefaultSubobject<UEnvironmentComponent>("Environment");
	Environment->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Navigation = CreateDefaultSubobject<UGridNavigationSystemComponent>("NavSystem");
	Navigation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MovementSystem = CreateDefaultSubobject<UMovementTargetingComponent>("MovementSystem");
	MovementSystem->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	AttackSystem = CreateDefaultSubobject<UAttackTargetingComponent>("AttackSystem");
	AttackSystem->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

void AGameBoard::ChangeTile(FTileBiome Tile)
{
	BiomeLocations.AddUnique(Tile);
	Environment->ChangeTile(Tile);
	Grid->AddCell(Tile.Coord);
	Navigation->AddNode(Tile.Coord);
}

void AGameBoard::RemoveTile(FOrtho Tile)
{
	int i = 0;
	if (BiomeLocations.Find(Tile, i))
	{
		BiomeLocations.RemoveAt(i);
		Environment->RemoveTile(Tile);
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

void AGameBoard::DisplayAttacks(TArray<FOrtho> Tiles)
{
	AttackSystem->Populate(Tiles);
}

void AGameBoard::ClearAttacks()
{
	AttackSystem->Clear();
}

void AGameBoard::DisplayMovement(TArray<FOrtho> Tiles)
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

void AGameBoard::SetEnvironmentComponent(TSubclassOf<UEnvironmentComponent> InLayout)
{
	delete Environment;
	Environment = NewObject<UEnvironmentComponent>(this, InLayout);
	Environment->Populate(BiomeLocations);
}

void AGameBoard::SetNavigationSystem(TSubclassOf<UGridNavigationSystemComponent> Router)
{
	// Clear any current route manager first
	delete Navigation;
	Navigation = NewObject<UGridNavigationSystemComponent>(this, Router);
	Navigation->Populate(GetTiles());
}

void AGameBoard::SetMovementSystem(TSubclassOf<UMovementTargetingComponent> MoveSys)
{
	delete MovementSystem;
	MovementSystem = NewObject<UMovementTargetingComponent>(this, MoveSys);
}

void AGameBoard::SetAttackSystem(TSubclassOf<UAttackTargetingComponent> AttackSys)
{
	delete AttackSystem;
	AttackSystem = NewObject<UAttackTargetingComponent>(this, AttackSys);
}

void AGameBoard::BeginPlay()
{
	Super::BeginPlay();

}

TArray<FOrtho> AGameBoard::GetCoords()
{
	TArray<FOrtho> Coords = TArray<FOrtho>();
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

