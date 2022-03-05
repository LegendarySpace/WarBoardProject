// Fill out your copyright notice in the Description page of Project Settings.


#include "WarBoardLibrary.h"

#include <math.h>
#include <stdlib.h>

#include "Tiles.h"

AActor* UWarBoardLibrary::Highlighted = nullptr;

AActor * UWarBoardLibrary::GetActorAtTile(UObject* WorldContextObject, const FTile Tile)
{
	// Check 95 units above and 5 below. This may change in future
	UWorld* World = WorldContextObject->GetWorld();
	FHitResult result;
	FVector start, end;
	start = Tile.ToWorld() + FVector(0, 0, 95);
	end = Tile.ToWorld() - FVector(0, 0, 5);
	// TODO: Need this to ignore Tiles or it will break Pathfinding
	// if (World->LineTraceSingleByChannel(result, start, end, ECollisionChannel::ECC_PhysicsBody)) return result.GetActor();
	return nullptr;
}

bool UWarBoardLibrary::IsSameTeam(const AActor * A, const AActor * B)
{
	if (A == nullptr || B == nullptr) return false;
	FName teamA = FName(TEXT(""));

	// Search A for "Team" tag, return false if not found
	for (auto atag : A->Tags)
	{
		if (atag.ToString().Contains("Team"))
		{
			teamA = atag;
			break;
		}
	}
	if (teamA.GetStringLength() == 0) return false;

	// Return true if B has same "Team" tag
	// Returns false if B has no tag
	return B->ActorHasTag(teamA);
}

bool UWarBoardLibrary::IsEnemyTeam(const AActor * A, const AActor * B)
{
	if (A == nullptr || B == nullptr) return false;
	FName teamA = FName(TEXT(""));
	FName teamB = FName(TEXT(""));

	// Search A for "Team" tag, return false if not found
	for (auto tag : A->Tags)
	{
		if (tag.ToString().Contains("Team"))
		{
			teamA = tag;
			break;
		}
	}
	if (teamA.GetStringLength() == 0) return false;

	// Search B for "Team" tag, return false if not found
	for (auto tag : B->Tags)
	{
		if (tag.ToString().Contains("Team"))
		{
			teamB = tag;
			break;
		}
	}
	if (teamA.GetStringLength() == 0) return false;

	// A and B both have "Team" tags, return true if different
	return !teamB.IsEqual(teamA);

}
