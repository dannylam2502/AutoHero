// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppSummaryMenu.h"
#include "Systems/CppGameInstance.h"
#include "UI/CppMainMenu.h"
#include "UI/CppRewardGiftPopup.h"

#include "Components/Button.h"

void UCppSummaryMenu::Setup()
{
	UCppBaseMenu::Setup();

	menuType = eMenuType::Menu;

	btnBack->OnClicked.AddDynamic(this, &UCppSummaryMenu::OnBackClicked);
}

void UCppSummaryMenu::Init()
{
	UCppBaseMenu::Init();

	UCppGameInstance::Push(UCppGameInstance::I()->rewardGiftPopup);
}

void UCppSummaryMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppSummaryMenu::OnBackClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnBackClicked!"));
	Pop();

	UCppGameInstance::Push(UCppGameInstance::I()->mainMenu);
}

void UCppSummaryMenu::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	btnBack = nullptr;
}