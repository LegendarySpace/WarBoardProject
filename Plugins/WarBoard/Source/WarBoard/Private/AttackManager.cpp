// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackManager.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;

// Sets default values
AAttackManager::AAttackManager()
{
 	// Should never tick
	PrimaryActorTick.bCanEverTick = false;

	FTile::FTile();
}

void AAttackManager::Populate_Implementation(TArray<FTile> Choices)
{
	Clear();
	FVector sca = FVector(WarBoardLib::GetTileSize() / 100.f);
	for (auto Tile : Choices)
	{
		auto a = GetActorAtTile(GetWorld(), Tile);
		if (a == nullptr && !bCanTargetEmptyTile) continue;
		if (IsSameTeam(GetHighlightedActor(), a) && !bCanTargetAllies) continue;

		HISM->AddInstance(FTransform(FRotator(0.0), Tile.ToWorld(), sca));
	}
}

// Called when the game starts or when spawned
void AAttackManager::BeginPlay()
{
	Super::BeginPlay();
	
}
