// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/LevelCreatorPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ALevelCreatorPawn::ALevelCreatorPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ViewTransform = CreateDefaultSubobject<USceneComponent>(TEXT("View Transform"));
	RootComponent = ViewTransform;

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Collision->SetCapsuleRadius(22.0);
	Collision->SetCapsuleHalfHeight(44.0);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AttachToComponent(Collision, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArm->SocketOffset = FVector(0, 0, 150);
	SpringArm->TargetArmLength = DesiredArmLength;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 2.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->bAutoActivate = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ALevelCreatorPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelCreatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!AllowZoom) return;
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, DesiredArmLength, GetWorld()->DeltaTimeSeconds, 10.f);
	if (FMath::IsNearlyEqual(SpringArm->TargetArmLength, DesiredArmLength, 0.01f)) AllowZoom = false;
}

// Called to bind functionality to input
void ALevelCreatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALevelCreatorPawn::Zoom(float Value)
{
	DesiredArmLength += Value;
	if (DesiredArmLength < MinLength) DesiredArmLength = MinLength;
	AllowZoom = true;
}


