// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CppMainMenu.h"
#include "UI/CppUIManager.h"
#include "Components/Button.h"
#include "UI/CppSettingPopup.h"
#include "UI/CppPVPMenu.h"
#include "UI/CppPVEMenu.h"
#include "EngineGlobals.h"

void UCppMainMenu::Setup()
{
	UCppBaseMenu::Setup();

	btnQuit->OnClicked.AddDynamic(this, &UCppMainMenu::OnQuitClicked);
	btnSetting->OnClicked.AddDynamic(this, &UCppMainMenu::OnSettingClicked);
	btnPvp->OnClicked.AddDynamic(this, &UCppMainMenu::OnPvpClicked);
	btnPve->OnClicked.AddDynamic(this, &UCppMainMenu::OnPveClicked);
}

void UCppMainMenu::Init(TArray<UObject*> initParams)
{
	UCppBaseMenu::Init(initParams);
}

void UCppMainMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppMainMenu::OnQuitClicked()
{
	ACppUIManager::QuitGame();
}

void UCppMainMenu::OnSettingClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnSettingClicked!"));

	TArray<UObject*> initParams;
	ACppUIManager::Push(ACppUIManager::Instance()->settingPopup, initParams);
}

void UCppMainMenu::OnPvpClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnPvpClicked!"));
	Pop();

	TArray<UObject*> initParams;
	ACppUIManager::Push(ACppUIManager::Instance()->pvpMenu, initParams);
}

void UCppMainMenu::OnPveClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnPveClicked!"));
	Pop();

	TArray<UObject*> initParams;
	ACppUIManager::Push(ACppUIManager::Instance()->pveMenu, initParams);
}