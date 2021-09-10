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
		void SetNodeValues(int32 TravelCost, int32 Heuristic, int32 Parent, int32 Steps);

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

	// Overload Operators
	FORCEINLINE bool operator== (const APathNode &Node)
	{
		return F == Node.F;
	}

	FORCEINLINE bool operator== (const int32 i)
	{
		return Index == i;
	}

	FORCEINLINE bool operator!= (const APathNode &Node)
	{
		return F != Node.F;
	}

	FORCEINLINE bool operator> (const APathNode &Node)
	{
		return F > Node.F;
	}

	FORCEINLINE bool operator>= (const APathNode &Node)
	{
		return F >= Node.F;
	}

	FORCEINLINE bool operator< (const APathNode &Node)
	{
		return F < Node.F;
	}

	FORCEINLINE bool operator<= (const APathNode &Node)
	{
		return F <= Node.F;
	}

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

	// Estimated cost to destination
	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
	int32 F;

	// Total cost to reach destination
	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
	int32 Cost;

	// A measure of distance from Origin
	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
	int32 Heu;

	// Index of this node
	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
	int32 Index;

	// Index of node to parent
	UPROPERTY(EditAnywhere, Category = "WarBoard|Path")
	int32 ParentIndex;

	// Number of steps from Origin
	UPROPERTY(BlueprintReadWrite, Category = "WarBoard|Path")
	int32 Step;

	UMaterialInstance* M_Checked;
	UMaterialInstance* M_Unchecked;
	UMaterialInstance* M_Start;
	UMaterialInstance* M_End;
	UMaterialInstance* M_Path;
	UMaterialInstance* M_Ignored;

};
