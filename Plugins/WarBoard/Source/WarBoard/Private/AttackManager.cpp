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

}

void AAttackManager::Populate_Implementation(TArray<int32> Choices)
{
	Clear();
	FRotator rot = FRotator(0.0);
	FVector sca = FVector(WarBoardLib::GetTileSize() / 100.f);
	FVector loc = FVector();
	for (auto i : Choices)
	{
		auto a = GetActorAtIndex(GetWorld(), i);
		if (a == nullptr && !bCanTargetEmptyTile) continue;
		if (IsSameTeam(GetHighlightedActor(), a) && !bCanTargetAllies) continue;

		loc = IndexToWorld(i);
		HISM->AddInstance(FTransform(rot, loc, sca));
	}
}

// Called when the game starts or when spawned
void AAttackManager::BeginPlay()
{
	Super::BeginPlay();
	
}
