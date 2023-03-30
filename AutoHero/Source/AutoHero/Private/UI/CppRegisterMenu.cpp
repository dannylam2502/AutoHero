// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppRegisterMenu.h"
#include "UI/CppUIManager.h"
#include "UI/CppLoginMenu.h"
#include "Components/Button.h"

void UCppRegisterMenu::Setup()
{
	btnBack->OnClicked.AddDynamic(this, &UCppRegisterMenu::OnBackClicked);
	btnRegister->OnClicked.AddDynamic(this, &UCppRegisterMenu::OnRegisterClicked);
}

void UCppRegisterMenu::Init()
{
	UCppBaseMenu::Init();
}

void UCppRegisterMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppRegisterMenu::OnBackClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnBackClicked!"));
	Pop();
	ACppUIManager::Push(ACppUIManager::Instance()->loginMenu);
}

void UCppRegisterMenu::OnRegisterClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnRegisterClicked!"));
}