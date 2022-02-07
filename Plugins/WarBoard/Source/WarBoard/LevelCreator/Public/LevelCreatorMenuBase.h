// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelCreatorMenuBase.generated.h"

class UImage;
class UButton;
class UScrollBox;

/**
 * 
 */
UCLASS()
class WARBOARD_API ULevelCreatorMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
	ULevelCreatorMenuBase(const FObjectInitializer& ObjectInitializer);

protected:
	bool Initialize() override;

public:
	UFUNCTION(BlueprintCallable)
	void Setup();

	void Teardown();

	UFUNCTION(BlueprintCallable)
	void CloseMenu();
	virtual void CloseMenu_Implementation();

	UFUNCTION(BlueprintCallable)
	void Accept();
	virtual void Accept_Implementation();

	UFUNCTION()
	void LoadEnvironmentsList();

	UFUNCTION()
	void SetSavedEnvironmentsList(TArray<FString> EnvironmentList);

	virtual void SelectEnv(uint32 Index, FString Name);

	void UpdateChildren();

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Background;

	UPROPERTY(meta = (BindWidget))
	UImage* MenuBackground;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* SavedEnvScrollBox;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* AcceptButton;

	TSubclassOf<UUserWidget> SavedResultClass;

	TOptional<uint32> SelectedIndex;
	TOptional<FString> SelectedName;
};
