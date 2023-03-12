// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppSummaryMenu.h"
#include "UI/CppUIManager.h"
#include "UI/CppMainMenu.h"
#include "UI/CppRewardGiftPopup.h"

#include "Components/Button.h"

void UCppSummaryMenu::Setup()
{
	UCppBaseMenu::Setup();

	btnBack->OnClicked.AddDynamic(this, &UCppSummaryMenu::OnBackClicked);
}

void UCppSummaryMenu::Init()
{
	UCppBaseMenu::Init();

	ACppUIManager::Push(ACppUIManager::Instance()->rewardGiftPopup);
}

void UCppSummaryMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppSummaryMenu::OnBackClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnBackClicked!"));
	Pop();
	ACppUIManager::Push(ACppUIManager::Instance()->mainMenu);
}