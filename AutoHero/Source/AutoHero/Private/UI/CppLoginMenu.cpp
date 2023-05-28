// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppLoginMenu.h"
#include "SaveGame/CppGameData.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "PlayFab/CppPlayFabManager.h"

#include "Systems/CppGameInstance.h"
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
	UCppGameInstance::QuitGame();
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
		UCppGameInstance::I()->messagePopup->callback.BindLambda([]
		{
			UCppGameInstance::I()->Push(UCppGameInstance::I()->loginMenu);
		});
		UCppGameInstance::I()->Push(UCppGameInstance::I()->messagePopup);
		UCppGameInstance::I()->messagePopup->SetTextMessage("Please input your user name & password.");
	}
}

void UCppLoginMenu::OnRegisterClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Register Clicked!"));
	Pop();

	UCppGameInstance::Push(UCppGameInstance::I()->registerMenu);
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

	UCppGameInstance::Push(UCppGameInstance::I()->mainMenu);

	ACppPlayFabManager::I()->loginUserPassword = TEXT("");
}

void UCppLoginMenu::OnLoginError()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("On Login Error!"));

	ACppPlayFabManager::I()->loginUserPassword = TEXT("");
}

void UCppLoginMenu::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	btnQuit = nullptr;
	inputUserName = nullptr;
	inputPassword = nullptr;
	btnLogin = nullptr;
	btnSignUp = nullptr;
}