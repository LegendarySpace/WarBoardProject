// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ETileType.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Category = "WarBoard|Enums")
enum ETileType
{
	TT_Normal		UMETA(DisplayName = "Normal"),
	TT_Forest		UMETA(DisplayName = "Forest"),
	TT_Plains		UMETA(DisplayName = "Plains"),
	TT_Mountain		UMETA(DisplayName = "Mountain"),
	TT_Sea			UMETA(DisplayName = "Sea"),
	TT_Type_MAX		UMETA(Hidden)
};

