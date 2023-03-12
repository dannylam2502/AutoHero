// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppUIManager.h"

#include "UI/CppMainMenu.h"
#include "UI/CppPVPMenu.h"
#include "UI/CppPVEMenu.h"
#include "UI/CppBattleMenu.h"
#include "UI/CppSummaryMenu.h"

#include "UI/CppSettingPopup.h"
#include "UI/CppRewardGiftPopup.h"

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
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
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

#pragma region Menu.
	mainMenu = CreateWidget<UCppMainMenu>(GetWorld(), mainMenuClass);
	check(mainMenu);
	mainMenu->Setup();

	pvpMenu = CreateWidget<UCppPVPMenu>(GetWorld(), pvpMenuClass);
	check(pvpMenu);
	pvpMenu->Setup();

	pveMenu = CreateWidget<UCppPVEMenu>(GetWorld(), pveMenuClass);
	check(pveMenu);
	pveMenu->Setup();

	battleMenu = CreateWidget<UCppBattleMenu>(GetWorld(), battleMenuClass);
	check(battleMenu);
	battleMenu->Setup();

	summaryMenu = CreateWidget<UCppSummaryMenu>(GetWorld(), summaryMenuClass);
	check(summaryMenu);
	summaryMenu->Setup();
#pragma endregion

#pragma region Popup.
	settingPopup = CreateWidget<UCppSettingPopup>(GetWorld(), settingPopupClass);
	check(settingPopup);
	settingPopup->Setup();

	rewardGiftPopup = CreateWidget<UCppRewardGiftPopup>(GetWorld(), rewardGiftPopupClass);
	check(rewardGiftPopup);
	rewardGiftPopup->Setup();
#pragma endregion

	// Init push menu.
	Push(mainMenu);
}

void ACppUIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	mainMenu = nullptr;
	settingPopup = nullptr;
}

void ACppUIManager::Push(UCppBaseMenu* menu)
{
	menu->Init();
}

void ACppUIManager::Pop(UCppBaseMenu* menu)
{
	menu->Pop();
}