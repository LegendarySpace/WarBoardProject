// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelCreatorMenuBase.h"
#include "LevelCreatorMenuSave.generated.h"

class UEditableTextBox;

/**
 * 
 */
UCLASS()
class WARBOARD_API ULevelCreatorMenuSave : public ULevelCreatorMenuBase
{
	GENERATED_BODY()
	
protected:
	bool Initialize() override;

public:
	virtual void Accept_Implementation() override;

	virtual void SelectEnv(uint32 Index, FString Name) override;

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* NameInput;

};
