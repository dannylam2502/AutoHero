// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppUIManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SaveGame/CppGameData.h"

#pragma region Menu
#include "UI/CppLoginMenu.h"
#include "UI/CppRegisterMenu.h"
#include "UI/CppMainMenu.h"
#include "UI/CppPVPMenu.h"
#include "UI/CppPVEMenu.h"
#include "UI/CppBattleMenu.h"
#include "UI/CppSummaryMenu.h"
#include "UI/CppMultiplayerMenu.h"
#pragma endregion

#pragma region Popup
#include "UI/CppSettingPopup.h"
#include "UI/CppRewardGiftPopup.h"
#include "UI/CppMessagePopup.h"
#include "UI/CppChatBoxPopup.h"
#include "UI/CppBlockPopup.h"
#pragma endregion

ACppUIManager* ACppUIManager::i;
ACppUIManager* ACppUIManager::I()
{
	return i;
}

// Sets default values
ACppUIManager::ACppUIManager()
{
	i = this;
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetNullAllVariable();

}

void ACppUIManager::SetNullAllVariable()
{
#pragma region Menu.
	loginMenuClass = nullptr;
	loginMenu = nullptr;

	registerMenuClass = nullptr;
	registerMenu = nullptr;

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

	multiplayerMenuClass = nullptr;
	multiplayerMenu = nullptr;
#pragma endregion

#pragma region Popup.
	settingPopupClass = nullptr;
	settingPopup = nullptr;

	rewardGiftPopupClass = nullptr;
	rewardGiftPopup = nullptr;

	messagePopupClass = nullptr;
	messagePopup = nullptr;

	chatBoxPopupClass = nullptr;
	chatBoxPopup = nullptr;

	blockPopupClass = nullptr;
	blockPopup = nullptr;
#pragma endregion
}

// Called when the game starts or when spawned
void ACppUIManager::BeginPlay()
{
	//UCppGameData::Instance()->LoadGame();
	
	Super::BeginPlay();

	initCallback.BindUObject(this, &ACppUIManager::OnInitCallBack);
	popCallback.BindUObject(this, &ACppUIManager::OnPopupCallBack);

	UGameplayStatics::GetPlayerController(i->GetWorld(), 0)->bShowMouseCursor = true;

	SetInputUI();

#pragma region Menu.
	loginMenu = dynamic_cast<UCppLoginMenu*>(SetupMenu(loginMenu, loginMenuClass));
	registerMenu = dynamic_cast<UCppRegisterMenu*>(SetupMenu(registerMenu, registerMenuClass));
	mainMenu = dynamic_cast<UCppMainMenu*>(SetupMenu(mainMenu, mainMenuClass));
	pvpMenu = dynamic_cast<UCppPVPMenu*>(SetupMenu(pvpMenu, pvpMenuClass));
	pveMenu = dynamic_cast<UCppPVEMenu*>(SetupMenu(pveMenu, pveMenuClass));
	battleMenu = dynamic_cast<UCppBattleMenu*>(SetupMenu(battleMenu, battleMenuClass));
	summaryMenu = dynamic_cast<UCppSummaryMenu*>(SetupMenu(summaryMenu, summaryMenuClass));
	multiplayerMenu = dynamic_cast<UCppMultiplayerMenu*>(SetupMenu(multiplayerMenu, multiplayerMenuClass));
#pragma endregion

#pragma region Popup.
	settingPopup = dynamic_cast<UCppSettingPopup*>(SetupMenu(settingPopup, settingPopupClass));
	rewardGiftPopup = dynamic_cast<UCppRewardGiftPopup*>(SetupMenu(rewardGiftPopup, rewardGiftPopupClass));
	messagePopup = dynamic_cast<UCppMessagePopup*>(SetupMenu(messagePopup, messagePopupClass));
	chatBoxPopup = dynamic_cast<UCppChatBoxPopup*>(SetupMenu(chatBoxPopup, chatBoxPopupClass));
	blockPopup = dynamic_cast<UCppBlockPopup*>(SetupMenu(blockPopup, blockPopupClass));
#pragma endregion

	// Init push menu.
	Push(multiplayerMenu);
}

void ACppUIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

#if UE_EDITOR
	for (UCppBaseMenu* menu : arrayMenu)
	{
		menu->OnExitGame();
	}

	SetNullAllVariable();
#endif

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
	for (UCppBaseMenu* menu : i->arrayMenu)
	{
		menu->Pop();
	}
}

void ACppUIManager::SetInputUI()
{
	UGameplayStatics::GetPlayerController(i->GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
}

void ACppUIManager::SetInputGameplay()
{
	UGameplayStatics::GetPlayerController(i->GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
}

void ACppUIManager::QuitGame()
{
	UKismetSystemLibrary::QuitGame(i->GetWorld(), UGameplayStatics::GetPlayerController(i->GetWorld(), 0), EQuitPreference::Quit, false);
}

void ACppUIManager::OnInitCallBack()
{
}

void ACppUIManager::OnPopupCallBack()
{
}