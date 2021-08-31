// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LayoutManager.generated.h"

UCLASS()
class WARBOARD_API ALayoutManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALayoutManager();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

};
