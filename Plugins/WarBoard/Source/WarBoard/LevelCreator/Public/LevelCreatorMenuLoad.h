// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelCreatorMenuBase.h"
#include "LevelCreatorMenuLoad.generated.h"

/**
 * 
 */
UCLASS()
class WARBOARD_API ULevelCreatorMenuLoad : public ULevelCreatorMenuBase
{
	GENERATED_BODY()

protected:
	bool Initialize() override;

public:
	virtual void Accept_Implementation() override;

	virtual void SelectEnv(uint32 Index, FString Name) override;
};
