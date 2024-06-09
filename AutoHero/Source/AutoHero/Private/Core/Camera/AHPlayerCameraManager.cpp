// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Camera/AHPlayerCameraManager.h"

AAHPlayerCameraManager::AAHPlayerCameraManager()
{
	// Set default values for the camera position and rotation
	InitialCameraPosition = FVector::ZeroVector;
	InitialCameraRotation = FRotator::ZeroRotator;
}

void AAHPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	// Set the initial camera position and rotation
	SetInitialCameraPosition(InitialCameraPosition, InitialCameraRotation);
}

void AAHPlayerCameraManager::SetInitialCameraPosition(const FVector& Position, const FRotator& Rotation)
{
	// SetCameraLocation(Position);
	// SetCameraRotation(Rotation);
	SetActorLocation(Position);
	SetActorRotation(Rotation);
}

