// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementTargetingComponent.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;

// Sets default values
UMovementTargetingComponent::UMovementTargetingComponent()
{
 	// Should never tick
	PrimaryComponentTick.bCanEverTick = false;

	PlaneMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("UMaterialInstance'/WarBoard/Material/Node_Start_MI.Node_Start_MI'")).Object;
}

void UMovementTargetingComponent::Populate(TArray<FGCoord> Choices)
{
	for (auto Tile : Choices)
	{
		auto a = GetActorAtTile(GetWorld(), FTile(Tile));
		if (IsEnemyTeam(GetHighlightedActor(), a) && !bCanMoveToEnemy) continue;
		if (IsSameTeam(GetHighlightedActor(), a) && !bCanMoveToAlly) continue;

		this->AddInstance(CalculateTransform(Tile));
	}
}

void UMovementTargetingComponent::Populate(TArray<FTile> Choices)
{
	TArray<FGCoord> Population;
	for (auto Selection : Choices) Population.Add(Selection.ToRC());
	Populate(Population);
}

void UMovementTargetingComponent::Populate(TArray<FCubic> Choices)
{
	TArray<FGCoord> Population;
	for (auto Selection : Choices) Population.Add(FTile(Selection).ToRC());
	Populate(Population);
}

// Called when the game starts or when spawned
void UMovementTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

