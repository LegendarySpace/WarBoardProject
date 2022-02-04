// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "../Public/Tiles.h"

#include "LevelCreatorPlayerController.generated.h"

class ALevelCreatorPawn;
class ALevelCreatorBase;

/**
 * 
 */
UCLASS()
class WARBOARD_API ALevelCreatorPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALevelCreatorPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void IncreaseBiome();
	void DecreaseBiome();
	void Multiselect();
	void ClearMultiselect();
	void Load();
	void Save();
	void SaveAs();


protected:
	ALevelCreatorPawn* PlayerPawn = nullptr;
	ALevelCreatorBase* Creator = nullptr;
	FTile HoverTile = FTile();

private:
	bool bRotationEnabled = false;
	float ClickTimer = 0.0;
	FVector Forward;
	FVector Right;
	bool bMoveForward = false;
	bool bMoveRight = false;
	FTile* StartMultiselect = nullptr;
	FTile* EndMultiselect = nullptr;
	TArray<FTile> MultiselectArray = TArray<FTile>();
};
