// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "HeuristicFormula.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Category = "WarBoard|Enums")
enum class EHeuFormula : uint8
{
	Manhatan		UMETA(DisplayName="Manhatan"),
	MaxDXDY			UMETA(DisplayName="MaxDXDY"),
	Diagonal		UMETA(DisplayName="DiagonalShortCut"),
	Euclidean		UMETA(DisplayName="Euclidean"),
	EucNoSQRT		UMETA(DisplayName="EuclideanNoSQRT"),
	Heu_MAX			UMETA(Hidden)
};
