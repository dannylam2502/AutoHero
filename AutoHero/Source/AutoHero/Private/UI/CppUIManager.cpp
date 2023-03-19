// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppUIManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "UI/CppMainMenu.h"
#include "UI/CppPVPMenu.h"
#include "UI/CppPVEMenu.h"
#include "UI/CppBattleMenu.h"
#include "UI/CppSummaryMenu.h"

#include "UI/CppSettingPopup.h"
#include "UI/CppRewardGiftPopup.h"

ACppUIManager* ACppUIManager::instance;

ACppUIManager* ACppUIManager::Instance()
{
	return instance;
}

// Sets default values
ACppUIManager::ACppUIManager()
{
	instance = this;
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mainMenuClass = nullptr;
	mainMenu = nullptr;

	pvpMenuClass = nullptr;
	pvpMenu = nullptr;

	pveMenuClass = nullptr;
	pveMenu = nullptr;

	battleMenuClass = nullptr;
	battleMenu = nullptr;

	summaryMenuClass = nullptr;
	summaryMenu = nullptr;

	settingPopupClass = nullptr;
	settingPopup = nullptr;

	rewardGiftPopupClass = nullptr;
	rewardGiftPopup = nullptr;
}

// Called when the game starts or when spawned
void ACppUIManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(instance->GetWorld(), 0)->bShowMouseCursor = true;

	SetInputUI();

#pragma region Menu.
	mainMenu = dynamic_cast<UCppMainMenu*>(SetupMenu(mainMenu, mainMenuClass));
	pvpMenu = dynamic_cast<UCppPVPMenu*>(SetupMenu(pvpMenu, pvpMenuClass));
	pveMenu = dynamic_cast<UCppPVEMenu*>(SetupMenu(pveMenu, pveMenuClass));
	battleMenu = dynamic_cast<UCppBattleMenu*>(SetupMenu(battleMenu, battleMenuClass));
	summaryMenu = dynamic_cast<UCppSummaryMenu*>(SetupMenu(summaryMenu, summaryMenuClass));
#pragma endregion

#pragma region Popup.
	settingPopup = dynamic_cast<UCppSettingPopup*>(SetupMenu(settingPopup, settingPopupClass));
	rewardGiftPopup = dynamic_cast<UCppRewardGiftPopup*>(SetupMenu(rewardGiftPopup, rewardGiftPopupClass));
#pragma endregion

	// Init push menu.
	Push(mainMenu);
}

void ACppUIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	mainMenu = nullptr;
	pvpMenu = nullptr;
	pveMenu = nullptr;
	battleMenu = nullptr;
	summaryMenu = nullptr;
	settingPopup = nullptr;
	rewardGiftPopup = nullptr;

	for (UCppBaseMenu* menu : arrayMenu)
	{
		menu = nullptr;
	}
	arrayMenu.SetNum(0);
}

UCppBaseMenu* ACppUIManager::SetupMenu(UCppBaseMenu* menu, TSubclassOf<class UCppBaseMenu> menuClass)
{
	menu = CreateWidget<UCppBaseMenu>(GetWorld(), menuClass);
	check(menu);
	menu->Setup();
	arrayMenu.Add(menu);
	return menu;
}

void ACppUIManager::Push(UCppBaseMenu* menu)
{
	menu->Init();
}

void ACppUIManager::Pop(UCppBaseMenu* menu)
{
	menu->Pop();
}

void ACppUIManager::PopAll()
{
	for (UCppBaseMenu* menu : instance->arrayMenu)
	{
		menu->Pop();
	}
}

void ACppUIManager::SetInputUI()
{
	UGameplayStatics::GetPlayerController(instance->GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
}

void ACppUIManager::SetInputGameplay()
{
	UGameplayStatics::GetPlayerController(instance->GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
}

void ACppUIManager::QuitGame()
{
	UKismetSystemLibrary::QuitGame(instance->GetWorld(), UGameplayStatics::GetPlayerController(instance->GetWorld(), 0), EQuitPreference::Quit, false);
}