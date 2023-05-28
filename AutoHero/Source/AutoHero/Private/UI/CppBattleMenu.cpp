// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppBattleMenu.h"
#include "Systems/CppGameInstance.h"
#include "UI/CppSummaryMenu.h"
#include "Components/Button.h"

void UCppBattleMenu::Setup()
{
	UCppBaseMenu::Setup();

	menuType = eMenuType::Menu;

	btnEndGame->OnClicked.AddDynamic(this, &UCppBattleMenu::OnEndGameClicked);
}

void UCppBattleMenu::Init()
{
	UCppBaseMenu::Init();
}

void UCppBattleMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppBattleMenu::OnEndGameClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnEndGameClicked!"));
	Pop();

	UCppGameInstance::Push(UCppGameInstance::I()->summaryMenu);
}

void UCppBattleMenu::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	btnEndGame = nullptr;
}