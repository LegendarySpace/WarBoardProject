// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "../Public/Tiles.h"
#include "../Public/BiomeStructs.h"

#include "EnvironmentLayout.generated.h"

/**
 * 
 */
UCLASS()
class WARBOARD_API UEnvironmentLayout : public USaveGame
{
	GENERATED_BODY()
	
public:
	UEnvironmentLayout();

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	TArray<FTileBiome> Layout;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	TArray<FBiomeSetup> Biomes;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	uint32 UserIndex;
};
