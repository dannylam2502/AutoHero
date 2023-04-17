// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppRewardGiftPopup.h"
#include "Components/Button.h"

void UCppRewardGiftPopup::Setup()
{
	UCppBaseMenu::Setup();

	menuType = eMenuType::Popup;

	btnClaim->OnClicked.AddDynamic(this, &UCppRewardGiftPopup::OnClaimClicked);
}

void UCppRewardGiftPopup::Init()
{
	UCppBaseMenu::Init();
}

void UCppRewardGiftPopup::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppRewardGiftPopup::OnClaimClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnClaimClicked!"));
	Pop();
}

void UCppRewardGiftPopup::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	btnClaim = nullptr;
}