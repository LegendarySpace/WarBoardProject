// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LevelCreatorPawn.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class WARBOARD_API ALevelCreatorPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALevelCreatorPawn();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USceneComponent* ViewTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCapsuleComponent* Collision;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCameraComponent* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "WarBoard|LevelCreator")
	void Zoom(float Value);

protected:
	float DesiredArmLength = 500.f;
	float MinLength = 50.f;
	bool AllowZoom = false;

};
