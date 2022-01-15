// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackManager.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;

// Sets default values
AAttackManager::AAttackManager()
{
 	// Should never tick
	PrimaryActorTick.bCanEverTick = false;

	PlaneMaterial = ConstructorHelpers::FObjectFinder<UMaterialInstance>(TEXT("UMaterialInstance'/WarBoard/Material/Node_End_MI.Node_End_MI'")).Object;
}

void AAttackManager::Populate_Implementation(TArray<FTile> Choices)
{
	for (auto Tile : Choices)
	{
		auto a = GetActorAtTile(GetWorld(), Tile);
		if (a == nullptr && !bCanTargetEmptyTile) continue;
		if (IsSameTeam(GetHighlightedActor(), a) && !bCanTargetAllies) continue;

		Planes->AddInstance(FTransform(FRotator(0.0), Tile.ToWorld(), Scale));
	}
}

// Called when the game starts or when spawned
void AAttackManager::BeginPlay()
{
	Super::BeginPlay();
	
}
