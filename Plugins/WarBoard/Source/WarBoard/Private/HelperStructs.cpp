// Fill out your copyright notice in the Description page of Project Settings.


#include <math.h>

#include "HelperStructs.h"
#include "WarBoardLibrary.h"


float FTile::TileSize = 200.f;
float FTile::Radius = 100.f;
float FTile::Height = 200.f;
float FTile::Width = 200.f;
ETileShape FTile::Shape = ETileShape::Square;


void FTile::IndexToInternal(int32 InIndex)
{
	Row = FMath::RoundToInt(InIndex / MAX_WIDTH);
	Col = ((InIndex + (MAX_WIDTH / 2)) % MAX_WIDTH) - (MAX_WIDTH / 2);
}

void FTile::RCToInternal(int32 InRow, int32 InColumn)
{
	Row = InRow;
	Col = InColumn;
}

void FTile::CubicToInternal(int32 InA, int32 InB, int32 InC)
{
	Row = InA;
	Col = InB - InC;
}

FVector2D FTile::GetPattern()
{
	FVector2D Size = FVector2D();
	switch (UWarBoardLibrary::TileShape)
	{
	default:
		return FVector2D(Height, Width);
	case ETileShape::Hex_Hor:
		return FVector2D(Height, 1.5 * Width);
	case ETileShape::Hex_Vert:
		return FVector2D(1.5 * Height, Width);
	}
}

void FTile::WorldToInternal(FVector InLocation)
{
	FVector2D PIP, Pat = GetPattern();
	InLocation.X += .5 * Pat.X;
	InLocation.Y += .5 * Pat.Y;

	Row = floor(InLocation.X / Pat.X);
	Col = floor(InLocation.Y / Pat.Y);

	PIP.X = FMath::Fmod(InLocation.X, Pat.X);
	if (Row < 0) PIP.X = Pat.X - PIP.X;
	PIP.Y = FMath::Fmod(InLocation.Y, Pat.Y);
	if (Col < 0) PIP.Y = Pat.Y - PIP.Y;

	PIPtoInternal(PIP);
}

//
void FTile::PIPtoInternal(FVector2D PIP)
{
	auto Pat = GetPattern();
	float MaxYInShape;
	// Center PIP
	PIP = PIP - (Pat * .5);
	switch (UWarBoardLibrary::TileShape)
	{
	case ETileShape::Triangle:
		// Twice as many Cols as PIPs
		Col = 2 * Col;
		PIP.X += Pat.X * .5;
		// When odd row flip bottom to top
		if (Row % 2) PIP.X = Pat.X - PIP.X;

		MaxYInShape = Pat.Y - (PIP.X / FMath::Tan(FMath::DegreesToRadians(60)));
		if (abs(PIP.Y) > MaxYInShape) Col += FMath::Sign(PIP.Y);
		return;
	case ETileShape::Hex_Hor:
		// Twice as many Rows and Cols as PIPs
		Row = 2 * Row;
		Col = 2 * Col;
		if (abs(PIP.Y) < (Pat.Y / 6)) return;
		if (abs(PIP.Y) > (Pat.Y / 3) || (abs(PIP.X) >
			FMath::Tan(FMath::DegreesToRadians(60)) * ((Pat.Y / 3) - abs(PIP.Y))))
		{
			Row += FMath::Sign(PIP.X);
			Col += FMath::Sign(PIP.Y);
		}
		return;
	case ETileShape::Hex_Vert:
		// Twice as many Rows and Cols as PIPs
		Row = 2 * Row;
		Col = 2 * Col;
		if (abs(PIP.X) < (Pat.X / 6)) return;
		if (abs(PIP.X) > (Pat.X / 3) || (abs(PIP.Y) >
			FMath::Tan(FMath::DegreesToRadians(60)) * ((Pat.X / 3) - abs(PIP.X))))
		{
			Row += FMath::Sign(PIP.X);
			Col += FMath::Sign(PIP.Y);
		}
		return;
	default:
		return;
	}
}

int32 FTile::ToIndex() const
{
	return Row * UWarBoardLibrary::MaxWidth + Col;
}

FVector2D FTile::ToRC() const
{
	return FVector2D(Row, Col);
}

FVector FTile::ToCubic() const
{
	FVector Cubic;
	Cubic.X = Row;
	Cubic.Y = static_cast<int32>((Row + Col) / 2) - Row;
	Cubic.Z = static_cast<int32>((Row - Col) / 2) - Row;
	return Cubic;
}

FVector FTile::ToWorld(bool TileCenter) const
{
	FVector WorldLocation = FVector();
	float RadialPointOffset;
	bool IsUpsideDown;
	switch (UWarBoardLibrary::TileShape)
	{
	case ETileShape::Triangle:
		// Center is rotational point but needs to be radial point
		// Radial point distance from rotational point is (radius - (height - radius)) / 2
		WorldLocation.X = Row * Height;
		WorldLocation.Y = Col * Width / 2.f;

		RadialPointOffset = (Radius - (Height - Radius)) / 2;
		IsUpsideDown = (Row + Col) % 2;

		if (IsUpsideDown) WorldLocation.X += RadialPointOffset;
		else WorldLocation.X -= RadialPointOffset;
		break;
	case ETileShape::Octogon:
	case ETileShape::Square:
		WorldLocation.X = Row * Height;
		WorldLocation.Y = Col * Width;
		break;


		// TODO: Check accuracy
	case ETileShape::Hex_Hor:
		WorldLocation.X = Row * (Height / 2);
		WorldLocation.Y = Col * Radius * 1.5;
		break;
	case ETileShape::Hex_Vert:
		WorldLocation.X = Row * Radius * 1.5;
		WorldLocation.Y = Col * (Width / 2);
		break;
	default:
		break;
	}

	if (!TileCenter)
	{
		WorldLocation.X -= Height / 2;
		WorldLocation.Y -= Width / 2;
	}
	return WorldLocation + UWarBoardLibrary::GridOffset;
}

void FTile::SetTileSize(float Size)
{
	TileSize = Size;
	switch (Shape)
	{
	case ETileShape::Triangle:
		Radius = .5 * Size / FMath::Cos(FMath::DegreesToRadians(30));
		Height = Size * FMath::Sin(FMath::DegreesToRadians(60));
		Width = Size;
		return;
	case ETileShape::Square:
		Radius = (.5 * Size) / FMath::Sin(FMath::DegreesToRadians(45));
		Height = Size;
		Width = Size;
		break;
	case ETileShape::Hex_Hor:
		Radius = Size / 2;
		Height = 2 * (Radius * FMath::Sin(FMath::DegreesToRadians(60)));
		Width = Size;
		break;
	case ETileShape::Hex_Vert:
		Radius = Size / 2;
		Height = Size;
		Width = 2 * (Radius * FMath::Sin(FMath::DegreesToRadians(60)));
		break;
	case ETileShape::Octogon:
		Radius = (.5 * Size) / FMath::Sin(FMath::DegreesToRadians(22.2));
		Height = Size;
		Width = Size;
		break;
	default:
		Height = Width = Size;
		Radius = .5 * Size;
	}
}


