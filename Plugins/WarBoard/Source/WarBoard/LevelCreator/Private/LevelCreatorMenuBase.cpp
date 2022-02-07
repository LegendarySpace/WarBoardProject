// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/LevelCreatorMenuBase.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"

#include "../Public/SEResult.h"

#include "../Public/SavedEnvironments.h"


ULevelCreatorMenuBase::ULevelCreatorMenuBase(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> SEResultBPClass(TEXT("/WarBoard/LevelCreator/UI/WBP_SavedEnvironment"));
	if (!ensure(SEResultBPClass.Class != nullptr)) return;

	SavedResultClass = SEResultBPClass.Class;
}

bool ULevelCreatorMenuBase::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success || !ensure(CloseButton != nullptr) || !ensure(AcceptButton != nullptr)) return false;
	CloseButton->OnClicked.AddDynamic(this, &ULevelCreatorMenuBase::CloseMenu);
	AcceptButton->OnClicked.AddDynamic(this, &ULevelCreatorMenuBase::Accept);

	return true;
}

void ULevelCreatorMenuBase::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->bShowMouseCursor = true;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);

	LoadEnvironmentsList();
}

void ULevelCreatorMenuBase::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	InputModeData.SetConsumeCaptureMouseDown(true);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void ULevelCreatorMenuBase::CloseMenu()
{
	CloseMenu_Implementation();
}

void ULevelCreatorMenuBase::CloseMenu_Implementation()
{
	Teardown();
}

void ULevelCreatorMenuBase::Accept()
{
	Accept_Implementation();
}

void ULevelCreatorMenuBase::Accept_Implementation()
{
	Teardown();
}

void ULevelCreatorMenuBase::LoadEnvironmentsList()
{
	if (USavedEnvironments* LoadEnv = Cast<USavedEnvironments>(UGameplayStatics::LoadGameFromSlot(TEXT("SE_SavedEnvironmentNames"), 0)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Successfully Loaded Environment Names"));
		SetSavedEnvironmentsList(LoadEnv->EnvironmentNames);
	}
	else UE_LOG(LogTemp, Warning, TEXT("Failed To Load Environment Names"));
	// UPDATE: If no Environments exist display message saying so
}

void ULevelCreatorMenuBase::SetSavedEnvironmentsList(TArray<FString> EnvironmentList)
{
	if (!ensure(SavedEnvScrollBox != nullptr) || !ensure(SavedResultClass != nullptr)) return;
	SavedEnvScrollBox->ClearChildren();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	uint32 i = 0;
	for (const FString& SavedEnvironment : EnvironmentList)
	{
		auto Row = CreateWidget<USEResult>(World, SavedResultClass);
		if (!ensure(Row != nullptr)) return;
		Row->SetName(SavedEnvironment);
		Row->Setup(this, i);
		++i;

		SavedEnvScrollBox->AddChild(Row);
	}
}

void ULevelCreatorMenuBase::SelectEnv(uint32 Index, FString Name)
{
	SelectedIndex = Index;
	SelectedName = Name;
	UpdateChildren();
}

void ULevelCreatorMenuBase::UpdateChildren()
{
	if (!ensure(SavedEnvScrollBox != nullptr)) return;

	for (int32 i = 0; i < SavedEnvScrollBox->GetChildrenCount(); i++)
	{
		auto Row = Cast<USEResult>(SavedEnvScrollBox->GetChildAt(i));
		if (Row != nullptr)
		{
			bool Sel = SelectedIndex.IsSet() && SelectedIndex.GetValue() == i;
			Row->bIsSelected = Sel;
			Row->UpdateSelection();
		}
	}
}


