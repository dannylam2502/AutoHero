// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppMessagePopup.h"
#include "UI/CppUIManager.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCppMessagePopup::Setup()
{
	UCppBaseMenu::Setup();

	menuType = eMenuType::Popup;

	btnClose->OnClicked.AddDynamic(this, &UCppMessagePopup::OnCloseClicked);
}

void UCppMessagePopup::Init()
{
	UCppBaseMenu::Init();
}

void UCppMessagePopup::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppMessagePopup::SetTextMessage(FString content)
{
	textMessage->SetText(FText::FromString(content));
}

void UCppMessagePopup::OnCloseClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnCloseClicked!"));

	Pop();
}