  // Fill out your copyright notice in the Description page of Project Settings.


#include "PathNode.h"
#include "WarBoardLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Math/NumericLimits.h"

// Sets default values
APathNode::APathNode()
{
 	// Never Tick
	PrimaryActorTick.bCanEverTick = false;

	// Create Default Components
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	auto s = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/WarBoard/StaticMesh/Sphere.Sphere'"));
	if (s.Object) Sphere->SetStaticMesh(s.Object);

	Display = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Display"));
	Display->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Import Materials
	auto m1 = ConstructorHelpers::FObjectFinder<UMaterialInstance>(TEXT("UMaterialInstance'/WarBoard/Material/Node_Unchecked_MI.Node_Unchecked_MI'"));
	if (m1.Object) M_Unchecked = m1.Object;
	auto m2 = ConstructorHelpers::FObjectFinder<UMaterialInstance>(TEXT("UMaterialInstance'/WarBoard/Material/Node_Checked_MI.Node_Checked_MI'"));
	if (m2.Object) M_Checked = m2.Object;
	auto m3 = ConstructorHelpers::FObjectFinder<UMaterialInstance>(TEXT("UMaterialInstance'/WarBoard/Material/Node_Start_MI.Node_Start_MI'"));
	if (m3.Object) M_Start = m3.Object;
	auto m4 = ConstructorHelpers::FObjectFinder<UMaterialInstance>(TEXT("UMaterialInstance'/WarBoard/Material/Node_End_MI.Node_End_MI'"));
	if (m4.Object) M_End = m4.Object;
	auto m5 = ConstructorHelpers::FObjectFinder<UMaterialInstance>(TEXT("UMaterialInstance'/WarBoard/Material/Node_Path_MI.Node_Path_MI'"));
	if (m5.Object) M_Path = m5.Object;
	auto m6 = ConstructorHelpers::FObjectFinder<UMaterialInstance>(TEXT("UMaterialInstance'/WarBoard/Material/Node_Ignored_MI.Node_Ignored_MI'"));
	if (m6.Object) M_Ignored = m6.Object;
}

void APathNode::Reset()
{
	F = 0;
	Cost = 0;
	Heu = 0;
	WarBoardLib::WorldToIndex(GetActorLocation(), Index);
	ParentIndex = TNumericLimits<int32>::Lowest();
	Sphere->SetMaterial(0, M_Ignored);
	Sphere->SetVisibility(false);
	Display->SetVisibility(false);
}

void APathNode::SetNodeValues(int32 TravelCost, int32 Heuristic, int32 Parent, int32 Steps)
{
	Cost = TravelCost;
	Heu = Heuristic;
	F = Cost + Heuristic;
	ParentIndex = Parent;
	Sphere->SetMaterial(0, M_Unchecked);
	Step = Steps;
}

void APathNode::SetAsPath()
{
	Sphere->SetMaterial(0, M_Path);
}

void APathNode::SetAsStart()
{
	Sphere->SetMaterial(0, M_Start);
}

void APathNode::SetAsEnd()
{
	Sphere->SetMaterial(0, M_End);
}

void APathNode::SetAsChecked()
{
	Sphere->SetMaterial(0, M_Checked);
}

void APathNode::EnableDebugMode()
{
	Sphere->SetVisibility(true);
	Display->SetVisibility(true);
}

void APathNode::DisplayStat(int32 Stat)
{
	FString text;
	switch (Stat)
	{
	case 1:
		text = "P: " + ParentIndex;
		break;
	case 2:
		text = "C: " + Cost;
		break;
	case 3:
		text = "H: " + Heu;
		break;
	case 4:
		text = "F: " + F;
	default:
		text = "";
		break;
	}
	Display->SetText(text);
}

// Called when the game starts or when spawned
void APathNode::BeginPlay()
{
	Super::BeginPlay();
	
}

