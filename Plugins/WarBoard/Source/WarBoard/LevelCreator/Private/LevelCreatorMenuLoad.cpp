// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/LevelCreatorMenuLoad.h"

#include "../Public/LevelCreatorPlayerController.h"


bool ULevelCreatorMenuLoad::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	return true;
}

void ULevelCreatorMenuLoad::Accept_Implementation()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	auto PC = Cast<ALevelCreatorPlayerController>(World->GetFirstPlayerController());
	if (!ensure(PC != nullptr)) return;

	PC->Load(SelectedName.GetValue());
	CloseMenu();
}

void ULevelCreatorMenuLoad::SelectEnv(uint32 Index, FString Name)
{
	Super::SelectEnv(Index, Name);
}
