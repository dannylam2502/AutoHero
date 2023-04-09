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
	btnQuit->OnClicked.AddDynamic(this, &UCppLoginMenu::OnQuitClicked);
	btnLogin->OnClicked.AddDynamic(this, &UCppLoginMenu::OnLoginClicked);
	btnSignUp->OnClicked.AddDynamic(this, &UCppLoginMenu::OnRegisterClicked);

	inputUserName->OnTextChanged.AddDynamic(this, &UCppLoginMenu::OnUserNameInput);
	inputPassword->OnTextChanged.AddDynamic(this, &UCppLoginMenu::OnPasswordInput);
}

void UCppLoginMenu::Init(TArray<UObject*> initParams)
{
	UCppBaseMenu::Init(initParams);
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

	if (!ACppPlayFabManager::Instance()->loginUserName.IsEmpty()
	 && !ACppPlayFabManager::Instance()->loginUserPassword.IsEmpty())
	{
		ACppPlayFabManager::Instance()->GetLogin();
	}
	else
	{
		TArray<UObject*> initParams;// add callback
		ACppUIManager::Instance()->Push(ACppUIManager::Instance()->messagePopup, initParams);
		FString strContent = "Please input your user name & password.";
		ACppUIManager::Instance()->messagePopup->SetTextMessage(strContent);
	}
}

void UCppLoginMenu::OnRegisterClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Register Clicked!"));
	Pop();

	TArray<UObject*> initParams;
	ACppUIManager::Push(ACppUIManager::Instance()->registerMenu, initParams);
}

void UCppLoginMenu::OnUserNameInput(const FText& text)
{
	if (!text.IsEmptyOrWhitespace())
	{
		ACppPlayFabManager::Instance()->loginUserName = text.ToString();
	}
}

void UCppLoginMenu::OnPasswordInput(const FText& text)
{
	if (!text.IsEmptyOrWhitespace())
	{
		ACppPlayFabManager::Instance()->loginUserPassword = text.ToString();
	}
}

void UCppLoginMenu::OnLoginSuccess()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("On Login Success!"));
	Pop();

	TArray<UObject*> initParams;
	ACppUIManager::Push(ACppUIManager::Instance()->mainMenu, initParams);

	ACppPlayFabManager::Instance()->loginUserPassword = TEXT("");
}

void UCppLoginMenu::OnLoginError()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("On Login Error!"));

	ACppPlayFabManager::Instance()->loginUserPassword = TEXT("");
}