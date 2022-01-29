  // Fill out your copyright notice in the Description page of Project Settings.


#include "PathNode.h"
#include "Math/NumericLimits.h"

#include "WarBoardLibrary.h"



void FPathNode::Reset()
{
	F = 0;
	Cost = 0;
	Heu = 0;
	ParentTile = Tile;
	NavStatus = ENavStatus::Ignored;
}

void FPathNode::SetNodeValues(int32 TravelCost, int32 Heuristic, FTile Parent, int32 Steps)
{
	Cost = TravelCost;
	Heu = Heuristic;
	F = Cost + Heuristic;
	ParentTile = Parent;
	NavStatus = ENavStatus::Unchecked;
	Step = Steps;
}


