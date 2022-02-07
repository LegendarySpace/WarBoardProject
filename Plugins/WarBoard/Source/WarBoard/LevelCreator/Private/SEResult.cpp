// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/SEResult.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "../Public/LevelCreatorMenuBase.h"


void USEResult::SetName(FString Name)
{
	EnvName->SetText(FText::FromString(Name));
}

void USEResult::Setup(ULevelCreatorMenuBase* Parent, uint32 Index)
{
	if (!ensure(RowButton != nullptr)) return;

	RowButton->OnClicked.AddDynamic(this, &USEResult::Clicked);

	ParentWidget = Parent;
	WidgetIndex = Index;
}

void USEResult::Clicked()
{
	if (!ensure(ParentWidget != nullptr)) return;
	ParentWidget->SelectEnv(WidgetIndex, EnvName->GetText().ToString());
}

void USEResult::UpdateSelection()
{
	if (bIsSelected != bPrevious)
	{
		bPrevious = bIsSelected;
	}
}
