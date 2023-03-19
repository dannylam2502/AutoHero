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

void UCppMainMenu::Init()
{
	UCppBaseMenu::Init();
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
	ACppUIManager::Push(ACppUIManager::Instance()->settingPopup);
}

void UCppMainMenu::OnPvpClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnPvpClicked!"));
	Pop();
	ACppUIManager::Push(ACppUIManager::Instance()->pvpMenu);
}

void UCppMainMenu::OnPveClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnPveClicked!"));
	Pop();
	ACppUIManager::Push(ACppUIManager::Instance()->pveMenu);
}