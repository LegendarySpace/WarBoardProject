// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SEResult.generated.h"

/**
 * 
 */
UCLASS()
class WARBOARD_API USEResult : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetName(FString Name);

	void Setup(class ULevelCreatorMenuBase* Parent, uint32 Index);

	UFUNCTION()
	void Clicked();

	UFUNCTION(BlueprintCallable)
	void UpdateSelection();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EnvName;

	UPROPERTY()
	class ULevelCreatorMenuBase* ParentWidget;
	int32 WidgetIndex;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bIsSelected;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bPrevious;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bIsHeader;
};
