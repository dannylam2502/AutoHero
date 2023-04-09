// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppMessagePopup.h"
#include "UI/CppUIManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCppMessagePopup::Setup()
{
	UCppBaseMenu::Setup();

	btnClose->OnClicked.AddDynamic(this, &UCppMessagePopup::OnCloseClicked);
}

void UCppMessagePopup::Init(TArray<UObject*> initParams)
{
	UCppBaseMenu::Init(initParams);
}

void UCppMessagePopup::Pop()
{
	UCppBaseMenu::Pop();

	// invot delegate
}

void UCppMessagePopup::SetTextMessage(FString content)
{
	textMessage->SetText(FText::FromString(content));
}

void UCppMessagePopup::OnCloseClicked()
{
	Pop();
}