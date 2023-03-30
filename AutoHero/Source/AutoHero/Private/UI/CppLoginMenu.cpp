// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppLoginMenu.h"
#include "UI/CppUIManager.h"
#include "UI/CppMainMenu.h"
#include "UI/CppRegisterMenu.h"
#include "Components/Button.h"

void UCppLoginMenu::Setup()
{
	btnQuit->OnClicked.AddDynamic(this, &UCppLoginMenu::OnQuitClicked);
	btnLogin->OnClicked.AddDynamic(this, &UCppLoginMenu::OnLoginClicked);
	btnRegister->OnClicked.AddDynamic(this, &UCppLoginMenu::OnRegisterClicked);
}

void UCppLoginMenu::Init()
{
	UCppBaseMenu::Init();
}

void UCppLoginMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppLoginMenu::OnQuitClicked()
{
	ACppUIManager::QuitGame();
}

void UCppLoginMenu::OnLoginClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnLoginClicked!"));
	Pop();
	ACppUIManager::Push(ACppUIManager::Instance()->mainMenu);
}

void UCppLoginMenu::OnRegisterClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnRegisterClicked!"));
	Pop();
	ACppUIManager::Push(ACppUIManager::Instance()->registerMenu);
}