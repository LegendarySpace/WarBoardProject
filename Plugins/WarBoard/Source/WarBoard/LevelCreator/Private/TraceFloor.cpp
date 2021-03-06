// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/TraceFloor.h"


UTraceFloor::UTraceFloor()
{
	PrimaryComponentTick.bCanEverTick = false;

	FBiomeSetup Setup = FBiomeSetup();
	Setup.Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/WarBoard/StaticMesh/SquarePlane.SquarePlane")).Object;
	Setup.Mat = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/WarBoard/Material/Base_Transparent_Mat.Base_Transparent_Mat")).Object;

	SetupInstance(Setup);
	Padding = 2;
	ClearTiles();
}

void UTraceFloor::CreateFloor(int32 InSize, FOrtho InOrigin)
{
	CreateFloor(InSize, FTile(InOrigin));
}

void UTraceFloor::CreateFloor(int32 InSize, FTile InOrigin)
{
	Size = InSize;
	Origin = InOrigin;
	for (int32 Row = -Size; Row <= Size; Row++)
	{
		for (int32 Col = -Size; Col <= Size; Col++)
		{
			FTile floor = FTile(Row, Col) + InOrigin;
			BuildFloor(floor);
		}
	}
}

void UTraceFloor::CreateFloor(int32 InSize, FCubic InOrigin)
{
	CreateFloor(InSize, FTile(InOrigin));
}

FTransform UTraceFloor::CalculateFloorTransform(FTile Tile)
{
	FTransform Form = CalculateTransform(Tile);
	FVector Loc = Form.GetLocation();
	float HeightOffset = Level * LevelHeight - 5;
	Loc.Z = HeightOffset;
	Form.SetLocation(Loc);
	return Form;
}

void UTraceFloor::BuildFloor(FTile Tile)
{
	if (!InstanceIndexes.Contains(Tile.ToOrtho()))
	{
		InstanceIndexes.AddUnique(Tile.ToOrtho());

		this->AddInstanceWorldSpace(CalculateFloorTransform(Tile));
	}
}

void UTraceFloor::RebuildFloor()
{
	for (int32 i = 0; i < InstanceIndexes.Num(); i++)
	{
		this->UpdateInstanceTransform(i, CalculateFloorTransform(InstanceIndexes[i]), true, false, true);
	}
}

void UTraceFloor::ShiftFloor(FOrtho Hover)
{
	ShiftFloor(FTile(Hover));
}

void UTraceFloor::ShiftFloor(FTile Hover)
{
	FOrtho ShiftBy;

	FOrtho HoverRelative = (Hover - Origin).ToOrtho();
	float r = ((float)HoverRelative.Row / Size) * 1.00000001;
	float c = ((float)HoverRelative.Column / Size) * 1.00000001;
	FOrtho Shift = FOrtho(lround(r), lround(c));
	if (Shift == FOrtho(0, 0)) return;
	ShiftBy = FOrtho(Shift.Row * Size, Shift.Column * Size);
	Origin += ShiftBy;

	for (FOrtho& Coord : InstanceIndexes) Coord += ShiftBy;
	RebuildFloor();
}

void UTraceFloor::ShiftFloor(FCubic Hover)
{
	ShiftFloor(FTile(Hover));
}

void UTraceFloor::ShiftLevel(int32 InLevel)
{
	Level += InLevel;
	RebuildFloor();
}


