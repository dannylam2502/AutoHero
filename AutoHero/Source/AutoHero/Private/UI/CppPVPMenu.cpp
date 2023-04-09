// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppPVPMenu.h"
#include "UI/CppUIManager.h"
#include "UI/CppMainMenu.h"
#include "UI/CppBattleMenu.h"
#include "Components/Button.h"

void UCppPVPMenu::Setup()
{
	UCppBaseMenu::Setup();

	btnBack->OnClicked.AddDynamic(this, &UCppPVPMenu::OnBackClicked);
	btnFindMatch->OnClicked.AddDynamic(this, &UCppPVPMenu::OnFindMatchClicked);
}

void UCppPVPMenu::Init(TArray<UObject*> initParams)
{
	UCppBaseMenu::Init(initParams);
}

void UCppPVPMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppPVPMenu::OnBackClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnBackClicked!"));
	Pop();

	TArray<UObject*> initParams;
	ACppUIManager::Push(ACppUIManager::Instance()->mainMenu, initParams);
}

void UCppPVPMenu::OnFindMatchClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnFindMatchClicked!"));
	Pop();

	TArray<UObject*> initParams;
	ACppUIManager::Push(ACppUIManager::Instance()->battleMenu, initParams);
}