// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppRewardGiftPopup.h"
#include "Components/Button.h"

void UCppRewardGiftPopup::Setup()
{
	UCppBaseMenu::Setup();

	btnClaim->OnClicked.AddDynamic(this, &UCppRewardGiftPopup::OnClaimClicked);
}

void UCppRewardGiftPopup::Init(TArray<UObject*> initParams)
{
	UCppBaseMenu::Init(initParams);
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