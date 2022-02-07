// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/LevelCreatorMenuSave.h"

#include "Components/EditableTextBox.h"

#include "../Public/LevelCreatorPlayerController.h"


bool ULevelCreatorMenuSave::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	return true;
}

void ULevelCreatorMenuSave::Accept_Implementation()
{
	if (NameInput->GetText().IsEmpty()) return;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	auto PC = Cast<ALevelCreatorPlayerController>(World->GetFirstPlayerController());
	if (!ensure(PC != nullptr)) return;

	PC->SaveAs(NameInput->GetText().ToString());
	CloseMenu();
}

void ULevelCreatorMenuSave::SelectEnv(uint32 Index, FString Name)
{
	Super::SelectEnv(Index, Name);
	NameInput->SetText(FText::FromString(Name));
}
