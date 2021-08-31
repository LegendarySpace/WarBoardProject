// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "ETileType.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Category = "WarBoard|Enums")
enum class ETileType : uint8
{
	Normal		UMETA(DisplayName = "Normal"),
	Forest		UMETA(DisplayName = "Forest"),
	Plains		UMETA(DisplayName = "Plains"),
	Mountain	UMETA(DisplayName = "Mountain"),
	Sea			UMETA(DisplayName = "Sea"),
	Type_MAX
};

