// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathNode.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class WARBOARD_API APathNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathNode();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Path")
		void Reset();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Path")
		void SetNodeValues(int32 Goal, int32 Heuristic, int32 Parent);

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Path")
		void SetAsPath();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Path")
		void SetAsStart();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Path")
		void SetAsEnd();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Path")
		void SetAsChecked();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Path")
		void EnableDebugMode();

	UFUNCTION(BlueprintCallable, Category = "WarBoard|Path")
		void DisplayStat(int32 Stat);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintReadOnly)
		USphereComponent* Collision;

	UPROPERTY(BlueprintReadOnly)
		UStaticMeshComponent* Sphere;

	UPROPERTY(BlueprintReadOnly)
		UTextRenderComponent* Display;

	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
		int32 F;

	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
		int32 Cost;

	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
		int32 Heu;

	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
		int32 Index;

	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
		int32 ParentIndex;

	UMaterialInstance* M_Checked;
	UMaterialInstance* M_Unchecked;
	UMaterialInstance* M_Start;
	UMaterialInstance* M_End;
	UMaterialInstance* M_Path;
	UMaterialInstance* M_Ignored;

};
