// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/LevelCreatorBase.h"

#include "../Public/TraceFloor.h"
#include "../Public/EnviromentComponent.h"

// Sets default values
ALevelCreatorBase::ALevelCreatorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Floor = CreateDefaultSubobject<UTraceFloor>(TEXT("Floor"));
	Floor->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	Enviroment = CreateDefaultSubobject<UEnviromentComponent>(TEXT("Enviroment"));
	Enviroment->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	// TODO: Should initialize Biomes
}

// Called when the game starts or when spawned
void ALevelCreatorBase::BeginPlay()
{
	Super::BeginPlay();

	Floor->CreateFloor(FloorSize);
	Enviroment->BiomeSettings = Biomes;
	Enviroment->InitializeBiomes();

}

// Called every frame
void ALevelCreatorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

