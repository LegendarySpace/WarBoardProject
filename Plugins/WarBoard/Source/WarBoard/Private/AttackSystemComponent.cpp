// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSystemComponent.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "WarBoardLibrary.h"

using namespace WarBoardLib;

// Sets default values
UAttackSystemComponent::UAttackSystemComponent()
{
 	// Should never tick
	PrimaryComponentTick.bCanEverTick = false;

	PlaneMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("UMaterialInstance'/WarBoard/Material/Node_End_MI.Node_End_MI'")).Object;
}

void UAttackSystemComponent::Populate(TArray<FGCoord> Choices)
{
	for (auto Tile : Choices)
	{
		auto a = GetActorAtTile(GetWorld(), FTile(Tile));
		if (a == nullptr && !bCanTargetEmptyTile) continue;
		if (IsSameTeam(GetHighlightedActor(), a) && !bCanTargetAllies) continue;
		
		this->AddInstance(CalculateTransform(Tile));
	}
}

void UAttackSystemComponent::Populate(TArray<FTile> Choices)
{
	TArray<FGCoord> Population;
	for (auto Selection : Choices) Population.Add(Selection.ToRC());
	Populate(Population);
}

void UAttackSystemComponent::Populate(TArray<FCubic> Choices)
{
	TArray<FGCoord> Population;
	for (auto Selection : Choices) Population.Add(FTile(Selection).ToRC());
	Populate(Population);
}

// Called when the game starts or when spawned
void UAttackSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
