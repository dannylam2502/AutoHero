// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppPVPMenu.h"

#include "PlayFab/CppPlayFabManager.h"
#include "UI/CppUIManager.h"

#include "Components/Button.h"

#include "UI/CppMainMenu.h"
#include "UI/CppBattleMenu.h"

void UCppPVPMenu::Setup()
{
	UCppBaseMenu::Setup();

	menuType = eMenuType::Menu;

	btnBack->OnClicked.AddDynamic(this, &UCppPVPMenu::OnBackClicked);
	btnFindMatch->OnClicked.AddDynamic(this, &UCppPVPMenu::OnFindMatchClicked);
}

void UCppPVPMenu::Init()
{
	UCppBaseMenu::Init();
}

void UCppPVPMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppPVPMenu::OnBackClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnBackClicked!"));
	Pop();

	ACppUIManager::Push(ACppUIManager::I()->mainMenu);
}

void UCppPVPMenu::OnFindMatchClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnFindMatchClicked!"));
	Pop();

	ACppPlayFabManager::I()->CreateMatchmakingTicket();

	//ACppUIManager::Push(ACppUIManager::I()->battleMenu);
}

void UCppPVPMenu::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	btnBack = nullptr;
	btnFindMatch = nullptr;
}