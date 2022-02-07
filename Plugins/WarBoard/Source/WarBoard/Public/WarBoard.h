// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "../LevelCreator/Public/SavedEnvironments.h"
#include "../LevelCreator/Public/EnvironmentLayout.h"
#include "Kismet/GameplayStatics.h"

class FWarBoardModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
