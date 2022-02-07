// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SavedEnvironments.generated.h"

/**
 * 
 */
UCLASS()
class WARBOARD_API USavedEnvironments : public USaveGame
{
	GENERATED_BODY()
	
public:
	USavedEnvironments();

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	TArray<FString> EnvironmentNames;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString SaveSlotName = TEXT("SE_SavedEnvironmentNames");

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	uint32 UserIndex;
};


