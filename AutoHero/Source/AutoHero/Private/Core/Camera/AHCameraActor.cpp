// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Camera/AHCameraActor.h"
#include "Camera/CameraComponent.h"

// Sets default values
AAHCameraActor::AAHCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	RootComponent = CameraComponent;

	// Set default camera position and rotation
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 1000.0f)); // Adjust these values as needed
	CameraComponent->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f)); // Adjust these values as needed
}

// Called when the game starts or when spawned
void AAHCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAHCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

