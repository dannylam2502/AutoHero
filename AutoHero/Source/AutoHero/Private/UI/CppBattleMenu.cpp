// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppBattleMenu.h"
#include "UI/CppUIManager.h"
#include "UI/CppSummaryMenu.h"
#include "Components/Button.h"

void UCppBattleMenu::Setup()
{
	UCppBaseMenu::Setup();

	btnEndGame->OnClicked.AddDynamic(this, &UCppBattleMenu::OnEndGameClicked);
}

void UCppBattleMenu::Init(TArray<UObject*> initParams)
{
	UCppBaseMenu::Init(initParams);
}

void UCppBattleMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppBattleMenu::OnEndGameClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnEndGameClicked!"));
	Pop();
	ACppUIManager::Push(ACppUIManager::Instance()->summaryMenu);
}