// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppPVEMenu.h"
#include "UI/CppUIManager.h"
#include "UI/CppMainMenu.h"
#include "UI/CppBattleMenu.h"
#include "Components/Button.h"

void UCppPVEMenu::Setup()
{
	UCppBaseMenu::Setup();

	btnBack->OnClicked.AddDynamic(this, &UCppPVEMenu::OnBackClicked);
	btnFindMatch->OnClicked.AddDynamic(this, &UCppPVEMenu::OnFindMatchClicked);
}

void UCppPVEMenu::Init()
{
	UCppBaseMenu::Init();
}

void UCppPVEMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppPVEMenu::OnBackClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnBackClicked!"));
	Pop();
	ACppUIManager::Push(ACppUIManager::Instance()->mainMenu);
}

void UCppPVEMenu::OnFindMatchClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnFindMatchClicked!"));
	Pop();
	ACppUIManager::Push(ACppUIManager::Instance()->battleMenu);
}