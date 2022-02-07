// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Public/Tiles.h"
#include "../Public/BiomeStructs.h"

#include "LevelCreatorBase.generated.h"

class UTraceFloor;
class UEnvironmentComponent;

UCLASS()
class WARBOARD_API ALevelCreatorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelCreatorBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UTraceFloor* Floor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UEnvironmentComponent* Environment;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Settings")
	int32 FloorSize = 25;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<FBiomeSetup> Biomes;
};
