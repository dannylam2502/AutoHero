// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "AHPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API AAHPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	AAHPlayerCameraManager();
	virtual void BeginPlay() override;
    
	void SetInitialCameraPosition(const FVector& Position, const FRotator& Rotation);

protected:
	FVector InitialCameraPosition;
	FRotator InitialCameraRotation;
	
};
