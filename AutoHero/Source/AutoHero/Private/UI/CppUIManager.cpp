// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppUIManager.h"
#include "UI/CppMainMenu.h"

// Sets default values
ACppUIManager::ACppUIManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mainMenuClass = nullptr;
	mainMenu = nullptr;
}

// Called when the game starts or when spawned
void ACppUIManager::BeginPlay()
{
	Super::BeginPlay();

	mainMenu = CreateWidget<UCppMainMenu>(GetWorld(), mainMenuClass);
	check(mainMenu);
	mainMenu->AddToPlayerScreen();
}

void ACppUIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	mainMenu->RemoveFromParent();
	mainMenu = nullptr;
}

// Called every frame
void ACppUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACppUIManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

