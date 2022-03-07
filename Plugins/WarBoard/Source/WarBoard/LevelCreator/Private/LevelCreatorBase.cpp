// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/LevelCreatorBase.h"

#include "../Public/TraceFloor.h"
#include "../Public/EnvironmentComponent.h"

// Sets default values
ALevelCreatorBase::ALevelCreatorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Floor = CreateDefaultSubobject<UTraceFloor>(TEXT("Floor"));
	Floor->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	Environment = CreateDefaultSubobject<UEnvironmentComponent>(TEXT("Environment"));
	Environment->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	SelectionHighlighter = CreateDefaultSubobject<UBiomeManager>(TEXT("Selection Highlighter"));
	SelectionHighlighter->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	HighlighterMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/WarBoard/Material/M_Highlighter_Base.M_Highlighter_Base")).Object;
}

void ALevelCreatorBase::AddTileHighlight(FOrtho Tile)
{
	AddHighlight(FTile(Tile));
}

void ALevelCreatorBase::AddHighlight(FTile Tile)
{
	if (!MultiselectArray.Contains(Tile))
	{
		MultiselectArray.AddUnique(Tile);
		SelectionHighlighter->AddTile(Tile);
	}
}

void ALevelCreatorBase::AddHighlight(FOrtho Tile)
{
	AddHighlight(FTile(Tile));
}

void ALevelCreatorBase::AddHighlight(FCubic Tile)
{
	AddHighlight(FTile(Tile));
}

void ALevelCreatorBase::RemoveTileHighlight(FOrtho Tile)
{
	RemoveHighlight(FTile(Tile));
}

void ALevelCreatorBase::RemoveHighlight(FTile Tile)
{
	if (MultiselectArray.Contains(Tile))
	{
		MultiselectArray.Remove(Tile);
		SelectionHighlighter->RemoveTile(Tile);
	}
}

void ALevelCreatorBase::RemoveHighlight(FOrtho Tile)
{
	RemoveHighlight(FTile(Tile));
}

void ALevelCreatorBase::RemoveHighlight(FCubic Tile)
{
	RemoveHighlight(FTile(Tile));
}

void ALevelCreatorBase::ClearHighlights()
{
	SelectionHighlighter->ClearTiles();
	MultiselectArray.Empty();
}

// Called when the game starts or when spawned
void ALevelCreatorBase::BeginPlay()
{
	Super::BeginPlay();

	Floor->CreateFloor(FloorSize);
	Environment->BiomeSettings = Biomes;
	Environment->InitializeBiomes();

	FBiomeSetup HighlighterSetup = FBiomeSetup();
	HighlighterSetup.Mat = HighlighterMaterial;
	SelectionHighlighter->SetupInstance(HighlighterSetup);
	SelectionHighlighter->MeshSize = 99;
}

// Called every frame
void ALevelCreatorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

