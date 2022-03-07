// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackTargetingComponent.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;

// Sets default values
UAttackTargetingComponent::UAttackTargetingComponent()
{
 	// Should never tick
	PrimaryComponentTick.bCanEverTick = false;

	PlaneMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("UMaterialInstance'/WarBoard/Material/Node_End_MI.Node_End_MI'")).Object;
}

void UAttackTargetingComponent::Populate(TArray<FOrtho> Choices)
{
	for (auto Tile : Choices)
	{
		auto a = GetActorAtTile(GetWorld(), FTile(Tile));
		if (a == nullptr && !bCanTargetEmptyTile) continue;
		if (IsSameTeam(GetHighlightedActor(), a) && !bCanTargetAllies) continue;
		
		this->AddInstance(CalculateTransform(Tile));
	}
}

void UAttackTargetingComponent::Populate(TArray<FTile> Choices)
{
	TArray<FOrtho> Population;
	for (auto Selection : Choices) Population.Add(Selection.ToOrtho());
	Populate(Population);
}

void UAttackTargetingComponent::Populate(TArray<FCubic> Choices)
{
	TArray<FOrtho> Population;
	for (auto Selection : Choices) Population.Add(FTile(Selection).ToOrtho());
	Populate(Population);
}

// Called when the game starts or when spawned
void UAttackTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
