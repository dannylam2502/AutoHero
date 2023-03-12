// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppUIManager.h"
#include "UI/CppMainMenu.h"
#include "UI/CppSettingPopup.h"
#include "UObject/ConstructorHelpers.h"
#include "AutoHero/AutoHeroGameMode.h"

ACppUIManager* ACppUIManager::instance;

ACppUIManager* ACppUIManager::Instance()
{
	return instance;
}

// Sets default values
ACppUIManager::ACppUIManager()
{
	instance = this;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mainMenuClass = nullptr;
	mainMenu = nullptr;
	settingPopupClass = nullptr;
	settingPopup = nullptr;
}

// Called when the game starts or when spawned
void ACppUIManager::BeginPlay()
{
	Super::BeginPlay();

	mainMenu = CreateWidget<UCppMainMenu>(GetWorld(), mainMenuClass);
	check(mainMenu);
	mainMenu->Setup();

	settingPopup = CreateWidget<UCppSettingPopup>(GetWorld(), settingPopupClass);
	check(settingPopup);
	settingPopup->Setup();

	// Init push menu.
	Push(mainMenu);
}

void ACppUIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	mainMenu = nullptr;
	settingPopup = nullptr;
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

void ACppUIManager::Push(UCppBaseMenu* menu)
{
	menu->Init();
}

void ACppUIManager::Pop(UCppBaseMenu* menu)
{
	menu->Pop();
}