// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard.h"
#include "PathFinder.h"
#include "MovementManager.h"
#include "AttackManager.h"

AGameBoard::AGameBoard()
{
	// Should never tick
	PrimaryActorTick.bCanEverTick = false;
}

void AGameBoard::AddPathNode(int32 Index)
{
	RouteManager->Add(Index);
}

void AGameBoard::RemovePathNode(int32 Index)
{
	RouteManager->Remove(Index);
}

void AGameBoard::SetPathFinder(TSubclassOf<APathFinder> Router)
{
	// Clear any current route manager first
	delete RouteManager;
	RouteManager = NewObject<APathFinder>(this, Router);
	RouteManager->Initialization(TArray<int32>());
}

void AGameBoard::SetMoveManager(TSubclassOf<AMovementManager> MoveSys)
{
	delete RouteManager;
	MoveManager = NewObject<AMovementManager>(this, MoveSys);
}

void AGameBoard::SetAttackManager(TSubclassOf<AAttackManager> AttackSys)
{
	delete RouteManager;
	AttackManager = NewObject<AAttackManager>(this, AttackSys);
}

APathFinder * AGameBoard::GetRouteManager()
{
	return RouteManager;
}

AMovementManager * AGameBoard::GetMoveManager()
{
	return MoveManager;
}

AAttackManager * AGameBoard::GetAttackManager()
{
	return AttackManager;
}

void AGameBoard::BeginPlay()
{
	Super::BeginPlay();

	SetPathFinder(APathFinder::StaticClass());
	SetMoveManager(AMovementManager::StaticClass());
	SetAttackManager(AAttackManager::StaticClass());
}
