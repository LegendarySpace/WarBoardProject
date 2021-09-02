// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EGridShape.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Category="WarBoard|Enums")
enum class EGridShape : uint8
{
	Square			UMETA(DisplayName="Square"),
	Diamond			UMETA(DisplayName="Diamond"),
	Cross			UMETA(DisplayName="Cross"),
	Diagonal		UMETA(DisplayName="Diagonal"),
	CrossDiagonal	UMETA(DisplayName="Cross + Diagonal"),
	Rhombus			UMETA(DisplayName="Rhombus"),
	RhombusVert		UMETA(DisplayName="Vertical Rhombus"),
	Shape_MAX		UMETA(Hidden)
};
