// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppLoginMenu.h"
#include "SaveGame/CppGameData.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "PlayFab/CppPlayFabManager.h"

#include "UI/CppUIManager.h"
#include "UI/CppMainMenu.h"
#include "UI/CppRegisterMenu.h"

#include "UI/CppMessagePopup.h"

void UCppLoginMenu::Setup()
{
	UCppBaseMenu::Setup();

	menuType = eMenuType::Menu;

	btnQuit->OnClicked.AddDynamic(this, &UCppLoginMenu::OnQuitClicked);
	btnLogin->OnClicked.AddDynamic(this, &UCppLoginMenu::OnLoginClicked);
	btnSignUp->OnClicked.AddDynamic(this, &UCppLoginMenu::OnRegisterClicked);

	inputUserName->OnTextChanged.AddDynamic(this, &UCppLoginMenu::OnUserNameInput);
	inputPassword->OnTextChanged.AddDynamic(this, &UCppLoginMenu::OnPasswordInput);
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
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Login Clicked!"));

	if (!ACppPlayFabManager::I()->loginUserName.IsEmpty()
	 && !ACppPlayFabManager::I()->loginUserPassword.IsEmpty())
	{
		ACppPlayFabManager::I()->GetLogin();
	}
	else
	{
		Pop();
		ACppUIManager::I()->messagePopup->callback.BindLambda([]
		{
			ACppUIManager::I()->Push(ACppUIManager::I()->loginMenu);
		});
		ACppUIManager::I()->Push(ACppUIManager::I()->messagePopup);
		ACppUIManager::I()->messagePopup->SetTextMessage("Please input your user name & password.");
	}
}

void UCppLoginMenu::OnRegisterClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Register Clicked!"));
	Pop();

	ACppUIManager::Push(ACppUIManager::I()->registerMenu);
}

void UCppLoginMenu::OnUserNameInput(const FText& text)
{
	if (!text.IsEmptyOrWhitespace())
	{
		ACppPlayFabManager::I()->loginUserName = text.ToString();
	}
}

void UCppLoginMenu::OnPasswordInput(const FText& text)
{
	if (!text.IsEmptyOrWhitespace())
	{
		ACppPlayFabManager::I()->loginUserPassword = text.ToString();
	}
}

void UCppLoginMenu::OnLoginSuccess()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("On Login Success!"));
	Pop();

	ACppUIManager::Push(ACppUIManager::I()->mainMenu);

	ACppPlayFabManager::I()->loginUserPassword = TEXT("");
}

void UCppLoginMenu::OnLoginError()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("On Login Error!"));

	ACppPlayFabManager::I()->loginUserPassword = TEXT("");
}