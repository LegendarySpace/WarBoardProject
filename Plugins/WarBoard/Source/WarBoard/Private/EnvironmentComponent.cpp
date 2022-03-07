// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentComponent.h"

#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"
#include <string.h>

#include "BiomeManager.h"
#include "WarBoardLibrary.h"

// Sets default values
UEnvironmentComponent::UEnvironmentComponent()
{
 	// Should never tick
	PrimaryComponentTick.bCanEverTick = false;

	BuildBiomeManagers();

}

void UEnvironmentComponent::InitializeBiomes()
{
	for (auto type = EBiome::TT_Normal; type != EBiome::TT_Type_MAX; type = EBiome(type + 1))
	{
		int32 i = 0;
		auto Biome = GetBiomeManager(type);
		if (BiomeSettings.Find(type, i)) Biome->SetupInstance(BiomeSettings[i]);
		else Biome->SetupInstance(FBiomeSetup(type));

		Biome->Padding = Padding;
	}

}

bool UEnvironmentComponent::ChangeTile(FTileBiome Instance)
{
	int32 i = 0;
	if (BiomeTiles.Find(Instance, i) && !(BiomeTiles[i] == Instance.Biome)) RemoveTile(Instance.Coord);
	else OnTileAdd.Broadcast(Instance.Coord);
	GetBiomeManager(Instance.Biome)->AddTile(Instance.Coord);
	BiomeTiles.AddUnique(Instance);
	OnTileChange.Broadcast(Instance.Coord, Instance.Biome);
	return true;
}

bool UEnvironmentComponent::RemoveTile(FOrtho Tile)
{
	return RemoveTile(FTile(Tile));
}

bool UEnvironmentComponent::RemoveTile(FTile Tile)
{
	int i = 0;
	if (BiomeTiles.Find(Tile.ToOrtho(), i))
	{
		FTileBiome TB = BiomeTiles[i];
		GetBiomeManager(TB.Biome)->RemoveTile(Tile);
		BiomeTiles.RemoveAt(i);
		OnTileRemove.Broadcast(Tile.ToOrtho());
	}
	return true;
}

bool UEnvironmentComponent::RemoveTile(FCubic Tile)
{
	return RemoveTile(FTile(Tile));
}

void UEnvironmentComponent::Populate(TArray<FTileBiome> Tiles)
{
	for (auto& Instance : Tiles)
	{
		ChangeTile(Instance);
	}
}

TArray<FOrtho> UEnvironmentComponent::GetTiles()
{
	TArray<FOrtho> Tiles;
	for (auto& Instance : BiomeTiles) Tiles.Add(Instance.Coord);
	return Tiles;
}

EBiome UEnvironmentComponent::GetBiome(FOrtho Tile)
{
	return GetBiome(FTile(Tile));
}

EBiome UEnvironmentComponent::GetBiome(FTile Tile)
{
	int32 i = 0;
	if (!BiomeTiles.Find(Tile.ToOrtho(), i)) return EBiome::TT_Type_MAX;
	return static_cast<EBiome>(BiomeTiles[i].Biome);
}

EBiome UEnvironmentComponent::GetBiome(FCubic Tile)
{
	return GetBiome(FTile(Tile));
}

bool UEnvironmentComponent::IsValid(FOrtho Tile)
{
	int32 i;
	return BiomeTiles.Find(Tile, i);
}

bool UEnvironmentComponent::IsValid(FTile Tile)
{
	return IsValid(Tile.ToOrtho());
}

bool UEnvironmentComponent::IsValid(FCubic Tile)
{
	return IsValid(FTile(Tile).ToOrtho());
}

void UEnvironmentComponent::SetPadding(float InPadding)
{
	this->Padding = InPadding;
	BiomeManager0->SetPadding(InPadding);
	BiomeManager1->SetPadding(InPadding);
	BiomeManager2->SetPadding(InPadding);
	BiomeManager3->SetPadding(InPadding);
	BiomeManager4->SetPadding(InPadding);
	BiomeManager5->SetPadding(InPadding);
	BiomeManager6->SetPadding(InPadding);
	BiomeManager7->SetPadding(InPadding);
	BiomeManager8->SetPadding(InPadding);
	BiomeManager9->SetPadding(InPadding);
}

void UEnvironmentComponent::ClearEnvironment()
{
	BiomeManager0->ClearTiles();
	BiomeManager1->ClearTiles();
	BiomeManager2->ClearTiles();
	BiomeManager3->ClearTiles();
	BiomeManager4->ClearTiles();
	BiomeManager5->ClearTiles();
	BiomeManager6->ClearTiles();
	BiomeManager7->ClearTiles();
	BiomeManager8->ClearTiles();
	BiomeManager9->ClearTiles();
}

void UEnvironmentComponent::BuildBiomeManagers()
{
	BiomeManager0 = CreateDefaultSubobject<UBiomeManager>(TEXT("NormalBiome"));
	BiomeManager0->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BiomeManager0->SetPadding(Padding);

	BiomeManager1 = CreateDefaultSubobject<UBiomeManager>(TEXT("ForestBiome"));
	BiomeManager1->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BiomeManager1->SetPadding(Padding);

	BiomeManager2 = CreateDefaultSubobject<UBiomeManager>(TEXT("PlainsBiome"));
	BiomeManager2->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BiomeManager2->SetPadding(Padding);

	BiomeManager3 = CreateDefaultSubobject<UBiomeManager>(TEXT("MountainBiome"));
	BiomeManager3->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BiomeManager3->SetPadding(Padding);

	BiomeManager4 = CreateDefaultSubobject<UBiomeManager>(TEXT("SeaBiome"));
	BiomeManager4->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BiomeManager4->SetPadding(Padding);



	BiomeManager5 = CreateDefaultSubobject<UBiomeManager>(TEXT("Biome 6"));
	BiomeManager5->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BiomeManager5->SetPadding(Padding);

	BiomeManager6 = CreateDefaultSubobject<UBiomeManager>(TEXT("Biome 7"));
	BiomeManager6->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BiomeManager6->SetPadding(Padding);

	BiomeManager7 = CreateDefaultSubobject<UBiomeManager>(TEXT("Biome 8"));
	BiomeManager7->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BiomeManager7->SetPadding(Padding);

	BiomeManager8 = CreateDefaultSubobject<UBiomeManager>(TEXT("Biome 9"));
	BiomeManager8->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BiomeManager8->SetPadding(Padding);

	BiomeManager9 = CreateDefaultSubobject<UBiomeManager>(TEXT("Biome 10"));
	BiomeManager9->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BiomeManager9->SetPadding(Padding);

}

// Called when the game starts or when spawned
void UEnvironmentComponent::BeginPlay()
{
	Super::BeginPlay();

}

UBiomeManager * UEnvironmentComponent::GetBiomeManager(EBiome Biome)
{
	UBiomeManager* Manager = nullptr;
	switch (Biome)
	{
	default:
	case TT_Normal:
		Manager = BiomeManager0;
		break;
	case TT_Forest:
		Manager = BiomeManager1;
		break;
	case TT_Plains:
		Manager = BiomeManager2;
		break;
	case TT_Mountain:
		Manager = BiomeManager3;
		break;
	case TT_Sea:
		Manager = BiomeManager4;
		break;
	}
	return Manager;
}

