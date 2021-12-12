// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileShape.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Category = "WarBoard|Enums")
enum class ETileShape : uint8
{
	Triangle		UMETA(DisplayName = "Triangle"),
	Square			UMETA(DisplayName = "Square"),
	Hex_Hor			UMETA(DisplayName = "Hexagon_Horizontal"),
	Hex_Vert		UMETA(DisplayName = "Hexagon_Vertical"),
	Octogon			UMETA(DisplayName = "Octogon"),
	Dodecagon		UMETA(DisplayName = "Dodecagon"),
	Shape_MAX		UMETA(Hidden)
};
