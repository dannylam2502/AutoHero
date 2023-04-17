// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppRegisterMenu.h"
#include "UI/CppUIManager.h"
#include "UI/CppMainMenu.h"
#include "UI/CppLoginMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "PlayFab/CppPlayFabManager.h"

void UCppRegisterMenu::Setup()
{
	UCppBaseMenu::Setup();

	menuType = eMenuType::Menu;

	btnBack->OnClicked.AddDynamic(this, &UCppRegisterMenu::OnBackClicked);
	btnRegister->OnClicked.AddDynamic(this, &UCppRegisterMenu::OnRegisterClicked);

	inputUserEmail->OnTextChanged.AddDynamic(this, &UCppRegisterMenu::OnUserEmailInput);
	inputUserName->OnTextChanged.AddDynamic(this, &UCppRegisterMenu::OnUserNameInput);
	inputPassword->OnTextChanged.AddDynamic(this, &UCppRegisterMenu::OnPasswordInput);
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
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Back Clicked!"));
	Pop();

	ACppUIManager::Push(ACppUIManager::I()->loginMenu);
}

void UCppRegisterMenu::OnRegisterClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Register Clicked!"));
	if (!ACppPlayFabManager::I()->userEmail.IsEmpty()
	 && !ACppPlayFabManager::I()->registerUserName.IsEmpty()
	 && !ACppPlayFabManager::I()->registerUserPassword.IsEmpty())
	{
		ACppPlayFabManager::I()->GetRegister();
	}
}

void UCppRegisterMenu::OnUserEmailInput(const FText& text)
{
	if (!text.IsEmptyOrWhitespace())
	{
		ACppPlayFabManager::I()->userEmail = text.ToString();
	}
}

void UCppRegisterMenu::OnUserNameInput(const FText& text)
{
	if (!text.IsEmptyOrWhitespace())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, text.ToString());
		ACppPlayFabManager::I()->registerUserName = text.ToString();
	}
}

void UCppRegisterMenu::OnPasswordInput(const FText& text)
{
	if (!text.IsEmptyOrWhitespace())
	{
		ACppPlayFabManager::I()->registerUserPassword = text.ToString();
	}
}

void UCppRegisterMenu::OnRegisterSuccess()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("On Register Success!"));
	Pop();

	ACppUIManager::Push(ACppUIManager::I()->mainMenu);

	ACppPlayFabManager::I()->registerUserPassword = TEXT("");
}

void UCppRegisterMenu::OnRegisterError()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("On Register Error!"));

	ACppPlayFabManager::I()->registerUserPassword = TEXT("");
}

void UCppRegisterMenu::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	btnBack = nullptr;
	btnRegister = nullptr;
	inputUserEmail = nullptr;
	inputUserName = nullptr;
	inputPassword = nullptr;
}