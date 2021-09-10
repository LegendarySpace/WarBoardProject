// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileStatus.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Category = "WarBoard|Enums")
enum ETileStatus
{
	TS_Invalid		= -1		UMETA(DisplayName = "Invalid"),
	TS_Blocked		= 0			UMETA(DisplayName = "Blocked"),
	TS_Obstructed	= 1			UMETA(DisplayName = "Obstructed"),
	TS_Open						UMETA(DisplayName = "Open"),
	TS_Status_MAX				UMETA(Hidden)
};
