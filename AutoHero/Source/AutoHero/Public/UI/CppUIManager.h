// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CppUIManager.generated.h"

class UCppMainMenu;

UCLASS()
class AUTOHERO_API ACppUIManager : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACppUIManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCppMainMenu> mainMenuClass;

	UPROPERTY(EditAnywhere)
		UCppMainMenu* mainMenu;
};
