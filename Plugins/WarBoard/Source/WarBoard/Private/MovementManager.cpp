// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "WarBoardLibrary.h"

using namespace WarBoardLib;

// Sets default values
AMovementManager::AMovementManager()
{
 	// Should never tick
	PrimaryActorTick.bCanEverTick = false;

}

void AMovementManager::Populate_Implementation(TArray<int32> Choices)
{
	Clear();
	FRotator rot = FRotator(0.0);
	FVector sca = FVector(WarBoardLib::GetTileSize() / 100.f);
	FVector loc = FVector();
	for (auto i : Choices)
	{
		auto a = GetActorAtIndex(GetWorld(), i);
		if (IsEnemyTeam(GetHighlightedActor(), a) && !bCanMoveToEnemy) continue;
		if (IsSameTeam(GetHighlightedActor(), a) && !bCanMoveToAlly) continue;

		loc = IndexToWorld(i);
		HISM->AddInstance(FTransform(rot, loc, sca));
	}
}

// Called when the game starts or when spawned
void AMovementManager::BeginPlay()
{
	Super::BeginPlay();
	
}

