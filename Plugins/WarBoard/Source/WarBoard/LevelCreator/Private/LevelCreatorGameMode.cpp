// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/LevelCreatorGameMode.h"

#include "../Public/LevelCreatorPawn.h"
#include "../Public/LevelCreatorPlayerController.h"


ALevelCreatorGameMode::ALevelCreatorGameMode()
{
	// Super::AGameModeBase();
	DefaultPawnClass = ALevelCreatorPawn::StaticClass();
	PlayerControllerClass = ALevelCreatorPlayerController::StaticClass();
}
