// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementManager.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;

// Sets default values
AMovementManager::AMovementManager()
{
 	// Should never tick
	PrimaryActorTick.bCanEverTick = false;

	PlaneMaterial = ConstructorHelpers::FObjectFinder<UMaterialInstance>(TEXT("UMaterialInstance'/WarBoard/Material/Node_Start_MI.Node_Start_MI'")).Object;
}

void AMovementManager::Populate_Implementation(TArray<FTile> Choices)
{
	for (auto Tile : Choices)
	{
		auto a = GetActorAtTile(GetWorld(), Tile);
		if (IsEnemyTeam(GetHighlightedActor(), a) && !bCanMoveToEnemy) continue;
		if (IsSameTeam(GetHighlightedActor(), a) && !bCanMoveToAlly) continue;

		Planes->AddInstance(FTransform(FRotator(0.0), Tile.ToWorld(), Scale));
	}
}

// Called when the game starts or when spawned
void AMovementManager::BeginPlay()
{
	Super::BeginPlay();
	
}

