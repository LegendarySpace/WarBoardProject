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

void UAttackSystemComponent::Populate_Implementation(TArray<FGCoord> Choices)
{
	for (auto Tile : Choices)
	{
		auto a = GetActorAtTile(GetWorld(), FTile(Tile));
		if (a == nullptr && !bCanTargetEmptyTile) continue;
		if (IsSameTeam(GetHighlightedActor(), a) && !bCanTargetAllies) continue;

		this->AddInstance(CalculateTransform(Tile));
	}
}

// Called when the game starts or when spawned
void UAttackSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
