// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/LevelCreatorPlayerController.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"
#include "Camera/CameraComponent.h"

#include "../Public/LevelCreatorPawn.h"
#include "../Public/LevelCreatorBase.h"
#include "../Public/TraceFloor.h"
#include "../Public/EnvironmentComponent.h"
#include "../Public/LevelCreatorMenuLoad.h"
#include "../Public/LevelCreatorMenuSave.h"
#include "../Public/EnvironmentLayout.h"
#include "../Public/SavedEnvironments.h"


ALevelCreatorPlayerController::ALevelCreatorPlayerController(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> LoadMenuBPClass(TEXT("/WarBoard/LevelCreator/UI/WBP_LoadMenu"));
	ConstructorHelpers::FClassFinder<UUserWidget> SaveMenuBPClass(TEXT("/WarBoard/LevelCreator/UI/WBP_SaveMenu"));
	ConstructorHelpers::FClassFinder<UUserWidget> SEResultBPClass(TEXT("/WarBoard/LevelCreator/UI/WBP_SavedEnvironment"));
	if (!ensure(LoadMenuBPClass.Class != nullptr) || !ensure(SaveMenuBPClass.Class != nullptr) ||
		!ensure(SEResultBPClass.Class != nullptr)) return;

	LoadMenuClass = LoadMenuBPClass.Class;
	SaveMenuClass = SaveMenuBPClass.Class;
	SavedResultClass = SEResultBPClass.Class;

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bAttachToPawn = true;
}

void ALevelCreatorPlayerController::BeginPlay()
{
	TArray<AActor*> FoundCreators;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelCreatorBase::StaticClass(), FoundCreators);

	if (FoundCreators.Num() > 0) Creator = static_cast<ALevelCreatorBase*>(FoundCreators[0]);
	else
	{
		UWorld* world = GetWorld();
		if (!ensure(world != nullptr)) exit(EXIT_FAILURE);

		Creator = world->SpawnActor<ALevelCreatorBase>(ALevelCreatorBase::StaticClass(), FVector(0), FRotator());

		if (Creator == nullptr) exit(EXIT_FAILURE);		// Ensure existance of Creator
	}

	// TODO: Setup HUD which I don't current have
	// HUD should detail location and biome

}

void ALevelCreatorPlayerController::Tick(float DeltaTime)
{
	if (PlayerPawn == nullptr)
	{
		PlayerPawn = static_cast<ALevelCreatorPawn*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	}

	if (bMoveForward || bMoveRight)
	{
		FVector Location = PlayerPawn->GetActorLocation();
		if (bMoveForward) Location += Forward;
		if (bMoveRight) Location += Right;
		Location.Z = PlayerPawn->GetActorLocation().Z;
		PlayerPawn->SetActorLocation(Location);
	}

	if (bRotationEnabled)
	{
		ClickTimer += DeltaTime;

		float DeltaX, DeltaY;
		GetInputMouseDelta(DeltaX, DeltaY);
		GetPawn()->SetActorRotation(FRotator(0, ControlRotation.Yaw, 0).Quaternion());
		float RestrictedPitch = FMath::Clamp((DeltaY * 4) + ControlRotation.Pitch, -89.f, 0.f);

		FRotator NewRotation = FRotator(RestrictedPitch, (DeltaX * 4) + ControlRotation.Yaw, ControlRotation.Roll);
		ControlRotation = NewRotation;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> Types;
	Types.Add(EObjectTypeQuery::ObjectTypeQuery2);
	FHitResult Res;
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursorForObjects(Types, true, Res))
	{
		// TODO: Move a cursor actor to Tile at ImpactPoint
		HoverTile = Res.ImpactPoint;
		Creator->Floor->ShiftFloor(HoverTile);
	}
}

void ALevelCreatorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FKey WKey = EKeys::W;
	FKey AKey = EKeys::A;
	FKey SKey = EKeys::S;
	FKey DKey = EKeys::D;
	FKey F5Key = EKeys::F5;
	FKey F9Key = EKeys::F9;
	FKey RCKey = EKeys::RightMouseButton;
	FKey LCKey = EKeys::LeftMouseButton;
	FKey SUpKey = EKeys::MouseScrollUp;
	FKey SDownKey = EKeys::MouseScrollDown;
	
	// Movement Bindings
	FInputKeyBinding WBinding(FInputChord(WKey, false, false, false, false), EInputEvent::IE_Pressed);
	WBinding.bConsumeInput = true;
	WBinding.bExecuteWhenPaused = false;
	WBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { MoveForward(1); });
	this->InputComponent->KeyBindings.Add(WBinding);

	FInputKeyBinding WRBinding(FInputChord(WKey, false, false, false, false), EInputEvent::IE_Released);
	WRBinding.bConsumeInput = true;
	WRBinding.bExecuteWhenPaused = false;
	WRBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { bMoveForward = false; });
	this->InputComponent->KeyBindings.Add(WRBinding);

	FInputKeyBinding ABinding(FInputChord(AKey, false, false, false, false), EInputEvent::IE_Pressed);
	ABinding.bConsumeInput = true;
	ABinding.bExecuteWhenPaused = false;
	ABinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { MoveRight(-1); });
	this->InputComponent->KeyBindings.Add(ABinding);

	FInputKeyBinding ARBinding(FInputChord(AKey, false, false, false, false), EInputEvent::IE_Released);
	ARBinding.bConsumeInput = true;
	ARBinding.bExecuteWhenPaused = false;
	ARBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { bMoveRight = false; });
	this->InputComponent->KeyBindings.Add(ARBinding);

	FInputKeyBinding SBinding(FInputChord(SKey, false, false, false, false), EInputEvent::IE_Pressed);
	SBinding.bConsumeInput = true;
	SBinding.bExecuteWhenPaused = false;
	SBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { MoveForward(-1); });
	this->InputComponent->KeyBindings.Add(SBinding);

	FInputKeyBinding SRBinding(FInputChord(SKey, false, false, false, false), EInputEvent::IE_Released);
	SRBinding.bConsumeInput = true;
	SRBinding.bExecuteWhenPaused = false;
	SRBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { bMoveForward = false; });
	this->InputComponent->KeyBindings.Add(SRBinding);

	FInputKeyBinding DBinding(FInputChord(DKey, false, false, false, false), EInputEvent::IE_Pressed);
	DBinding.bConsumeInput = true;
	DBinding.bExecuteWhenPaused = false;
	DBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { MoveRight(1); });
	this->InputComponent->KeyBindings.Add(DBinding);

	FInputKeyBinding DRBinding(FInputChord(DKey, false, false, false, false), EInputEvent::IE_Released);
	DRBinding.bConsumeInput = true;
	DRBinding.bExecuteWhenPaused = false;
	DRBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { bMoveRight = false; });
	this->InputComponent->KeyBindings.Add(DRBinding);

	// Rotation and - Biome Binding
	FInputKeyBinding RCPBinding(FInputChord(RCKey, false, false, false, false), EInputEvent::IE_Pressed);
	RCPBinding.bConsumeInput = true;
	RCPBinding.bExecuteWhenPaused = false;
	RCPBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { bRotationEnabled = true; });
	this->InputComponent->KeyBindings.Add(RCPBinding);

	FInputKeyBinding RCRBinding(FInputChord(RCKey, false, false, false, false), EInputEvent::IE_Released);
	RCRBinding.bConsumeInput = true;
	RCRBinding.bExecuteWhenPaused = false;
	RCRBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key)
	{ 
		bRotationEnabled = false;
		if (ClickTimer < 0.2) DecreaseBiome(); // Minor time passed, button clicked not held
		ClickTimer = 0.0;
	});
	this->InputComponent->KeyBindings.Add(RCRBinding);

	// Zoom Binding
	FInputKeyBinding SUBinding(FInputChord(SUpKey, false, false, false, false), EInputEvent::IE_Pressed);
	SUBinding.bConsumeInput = true;
	SUBinding.bExecuteWhenPaused = false;
	SUBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { PlayerPawn->Zoom(-50); });
	this->InputComponent->KeyBindings.Add(SUBinding);

	FInputKeyBinding SDBinding(FInputChord(SDownKey, false, false, false, false), EInputEvent::IE_Pressed);
	SDBinding.bConsumeInput = true;
	SDBinding.bExecuteWhenPaused = false;
	SDBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { PlayerPawn->Zoom(50); });
	this->InputComponent->KeyBindings.Add(SDBinding);

	// Raise/Lower Binding
	FInputKeyBinding SSUBinding(FInputChord(SUpKey, true, false, false, false), EInputEvent::IE_Pressed);
	SSUBinding.bConsumeInput = true;
	SSUBinding.bExecuteWhenPaused = false;
	SSUBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { Creator->Floor->ShiftLevel(1); });
	this->InputComponent->KeyBindings.Add(SSUBinding);

	FInputKeyBinding SSDBinding(FInputChord(SDownKey, true, false, false, false), EInputEvent::IE_Pressed);
	SSDBinding.bConsumeInput = true;
	SSDBinding.bExecuteWhenPaused = false;
	SSDBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { Creator->Floor->ShiftLevel(-1); });
	this->InputComponent->KeyBindings.Add(SSDBinding);

	// + Biome Binding
	FInputKeyBinding LCBinding(FInputChord(LCKey, false, false, false, false), EInputEvent::IE_Pressed);
	LCBinding.bConsumeInput = true;
	LCBinding.bExecuteWhenPaused = false;
	LCBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { IncreaseBiome(); });
	this->InputComponent->KeyBindings.Add(LCBinding);

	// MultiSelect Binding
	FInputKeyBinding CLCBinding(FInputChord(LCKey, false, true, false, false), EInputEvent::IE_Pressed);
	CLCBinding.bConsumeInput = true;
	CLCBinding.bExecuteWhenPaused = false;
	CLCBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { ToggleSingleSelection(); });
	this->InputComponent->KeyBindings.Add(CLCBinding);

	FInputKeyBinding SLCBinding(FInputChord(LCKey, true, false, false, false), EInputEvent::IE_Pressed);
	SLCBinding.bConsumeInput = true;
	SLCBinding.bExecuteWhenPaused = false;
	SLCBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { GroupSelection(); });
	this->InputComponent->KeyBindings.Add(SLCBinding);

	FInputKeyBinding CSLCBinding(FInputChord(LCKey, true, true, false, false), EInputEvent::IE_Pressed);
	CSLCBinding.bConsumeInput = true;
	CSLCBinding.bExecuteWhenPaused = false;
	CSLCBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { ToggleGroupSelection(); });
	this->InputComponent->KeyBindings.Add(CSLCBinding);

	// Clear MultiSelect Binding
	FInputKeyBinding ALCBinding(FInputChord(LCKey, false, false, true, false), EInputEvent::IE_Released);
	ALCBinding.bConsumeInput = true;
	ALCBinding.bExecuteWhenPaused = false;
	ALCBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { ClearMultiselect(); });
	this->InputComponent->KeyBindings.Add(ALCBinding);

	// Load Binding
	FInputKeyBinding F9Binding(FInputChord(F9Key, false, false, false, false), EInputEvent::IE_Released);
	F9Binding.bConsumeInput = true;
	F9Binding.bExecuteWhenPaused = false;
	F9Binding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { Load(); });
	this->InputComponent->KeyBindings.Add(F9Binding);

	// Save Binding
	FInputKeyBinding F5Binding(FInputChord(F5Key, false, false, false, false), EInputEvent::IE_Released);
	F5Binding.bConsumeInput = true;
	F5Binding.bExecuteWhenPaused = false;
	F5Binding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { Save(); });
	this->InputComponent->KeyBindings.Add(F5Binding);

	FInputKeyBinding SF5Binding(FInputChord(F5Key, true, false, false, false), EInputEvent::IE_Released);
	SF5Binding.bConsumeInput = true;
	SF5Binding.bExecuteWhenPaused = false;
	SF5Binding.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key) { SaveAs(); });
	this->InputComponent->KeyBindings.Add(SF5Binding);

}

void ALevelCreatorPlayerController::MoveForward(float Value)
{
	bMoveForward = true;
	Forward = PlayerPawn->GetActorForwardVector() * (Value * 25);
}

void ALevelCreatorPlayerController::MoveRight(float Value)
{
	bMoveRight = true;
	Right = PlayerPawn->GetActorRightVector() * (Value * 25);
}

void ALevelCreatorPlayerController::IncreaseBiome()
{
	if (!Creator->Environment->IsValid(HoverTile))
	{
		Creator->Environment->ChangeTile(FTileBiome(HoverTile));
	}
	else
	{
		EBiome Bio = Creator->Environment->GetBiome(HoverTile);		// Set any changed tiles to same biome
		if (Creator->MultiselectArray.Num() > 0)
		{
			for (FTile tile : Creator->MultiselectArray)
			{
				if (Bio < (EBiome::TT_Type_MAX - 1)) Creator->Environment->ChangeTile(FTileBiome(tile, static_cast<EBiome>(Bio + 1)));
				else Creator->Environment->RemoveTile(tile);
			}
		}
		else
		{
			if (Bio < (EBiome::TT_Type_MAX - 1)) Creator->Environment->ChangeTile(FTileBiome(HoverTile, static_cast<EBiome>(Bio + 1)));
			else Creator->Environment->RemoveTile(HoverTile);
		}
	}
}

void ALevelCreatorPlayerController::DecreaseBiome()
{
	if (!Creator->Environment->IsValid(HoverTile))
	{
		return;
	}
	else
	{
		EBiome Bio = Creator->Environment->GetBiome(HoverTile);		// Set any changed tiles to same biome
		if (Creator->MultiselectArray.Num() > 0)
		{
			for (FTile tile : Creator->MultiselectArray)
			{
				if (Bio == 0) Creator->Environment->RemoveTile(tile);
				else Creator->Environment->ChangeTile(FTileBiome(tile, static_cast<EBiome>(Bio - 1)));
			}
		}
		else
		{
			if (Bio == 0) Creator->Environment->RemoveTile(HoverTile);
			else Creator->Environment->ChangeTile(FTileBiome(HoverTile, static_cast<EBiome>(Bio - 1)));
		}
	}
}

void ALevelCreatorPlayerController::ToggleSingleSelection()
{
	if (Creator->MultiselectArray.Contains(HoverTile))
	{
		Creator->RemoveHighlight(HoverTile);
	}
	else
	{
		Creator->AddHighlight(HoverTile);
	}
}

void ALevelCreatorPlayerController::GroupSelection()
{
	Creator->ClearHighlights();
	if (!StartMultiselect.IsSet())
	{
		StartMultiselect = HoverTile;
	}
	else if (StartMultiselect.GetValue() != HoverTile)		// Ensure start and end are never same tile
	{
		EndMultiselect = HoverTile;
		int rowStart = FMath::Min(StartMultiselect.GetValue().ToOrtho().Row, EndMultiselect.GetValue().ToOrtho().Row);
		int colStart = FMath::Min(StartMultiselect.GetValue().ToOrtho().Column, EndMultiselect.GetValue().ToOrtho().Column);
		int rowEnd = FMath::Max(StartMultiselect.GetValue().ToOrtho().Row, EndMultiselect.GetValue().ToOrtho().Row);
		int colEnd = FMath::Max(StartMultiselect.GetValue().ToOrtho().Column, EndMultiselect.GetValue().ToOrtho().Column);

		for (int i = rowStart; i <= rowEnd; i++)
		{
			for (int j = colStart; j <= colEnd; j++)
			{
				FTile tile = FTile(i, j);
				Creator->AddHighlight(tile);
			}
		}

		StartMultiselect.Reset();
		EndMultiselect.Reset();
	}
}

void ALevelCreatorPlayerController::ToggleGroupSelection()
{
	if (!StartMultiselect.IsSet())
	{
		StartMultiselect = HoverTile;
	}
	else if (StartMultiselect.GetValue() != HoverTile)		// Ensure start and end are never same tile
	{
		EndMultiselect = HoverTile;
		int rowStart = FMath::Min(StartMultiselect.GetValue().ToOrtho().Row, EndMultiselect.GetValue().ToOrtho().Row);
		int colStart = FMath::Min(StartMultiselect.GetValue().ToOrtho().Column, EndMultiselect.GetValue().ToOrtho().Column);
		int rowEnd = FMath::Max(StartMultiselect.GetValue().ToOrtho().Row, EndMultiselect.GetValue().ToOrtho().Row);
		int colEnd = FMath::Max(StartMultiselect.GetValue().ToOrtho().Column, EndMultiselect.GetValue().ToOrtho().Column);

		for (int i = rowStart; i <= rowEnd; i++)
		{
			for (int j = colStart; j <= colEnd; j++)
			{
				FTile tile = FTile(i, j);

				// If both start and end are highlighted remove group, else add group
				if (Creator->MultiselectArray.Contains(StartMultiselect.GetValue()) && Creator->MultiselectArray.Contains(EndMultiselect.GetValue()))
				{
					Creator->RemoveHighlight(tile);
				}
				else
				{
					Creator->AddHighlight(tile);
				}
			}
		}

		StartMultiselect.Reset();
		EndMultiselect.Reset();
	}
}

void ALevelCreatorPlayerController::ClearMultiselect()
{
	Creator->ClearHighlights();
	StartMultiselect.Reset();
	EndMultiselect.Reset();
}

void ALevelCreatorPlayerController::Load()
{
	if (!ensure(LoadMenuClass != nullptr)) return;

	auto LoadMenu = CreateWidget<ULevelCreatorMenuLoad>(this, LoadMenuClass);
	if (!ensure(LoadMenu != nullptr)) return;

	LoadMenu->Setup();
}

void ALevelCreatorPlayerController::Load(FString Name)
{
	FString SlotName = FString(TEXT("SEL_"));
	SlotName += Name;
	if (UEnvironmentLayout* LoadFile = Cast<UEnvironmentLayout>(UGameplayStatics::LoadGameFromSlot(SlotName, 0)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Successfully Loaded Environment"));
		Creator->Environment->ClearEnvironment();
		Creator->Environment->BiomeSettings = LoadFile->Biomes;
		Creator->Environment->InitializeBiomes();
		for (auto Tile : LoadFile->Layout) Creator->Environment->ChangeTile(Tile);

		FileName = Name;
	}
	else UE_LOG(LogTemp, Warning, TEXT("Failed To Load Environment"));
}

void ALevelCreatorPlayerController::Save()
{
	if (FileName.IsEmpty()) SaveAs();

	FString SlotName = FString(TEXT("SEL_"));
	SlotName += FileName;
	if (UEnvironmentLayout* SaveFile = Cast<UEnvironmentLayout>(UGameplayStatics::CreateSaveGameObject(UEnvironmentLayout::StaticClass())))
	{
		SaveFile->Layout = Creator->Environment->GetLayout(); 
		SaveFile->Biomes = Creator->Environment->BiomeSettings;

		if (UGameplayStatics::SaveGameToSlot(SaveFile, SlotName, 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully Saved Environment"));
		}
		else UE_LOG(LogTemp, Warning, TEXT("Failed To Save Environment"));
	}
}

void ALevelCreatorPlayerController::SaveAs()
{
	if (!ensure(SaveMenuClass != nullptr)) return;

	auto SaveMenu = CreateWidget<ULevelCreatorMenuSave>(this, SaveMenuClass);
	if (!ensure(SaveMenu != nullptr)) return;

	SaveMenu->Setup();
}

void ALevelCreatorPlayerController::SaveAs(FString Name)
{
	bool Success = false;
	if (USavedEnvironments* EnvNames = Cast<USavedEnvironments>(UGameplayStatics::LoadGameFromSlot(TEXT("SE_SavedEnvironmentNames"), 0)))
	{
		EnvNames->EnvironmentNames.AddUnique(Name);
		if (UGameplayStatics::SaveGameToSlot(EnvNames, TEXT("SE_SavedEnvironmentNames"), 0))
		{
			Success = true;
			UE_LOG(LogTemp, Warning, TEXT("Successfully Updated Environment Names"));
		}
	}
	if (!Success) UE_LOG(LogTemp, Warning, TEXT("Failed To Update Environment Names"));

	FileName = Name; // This file may be saved even if the Environment Names list is not updated
	Save();
}


