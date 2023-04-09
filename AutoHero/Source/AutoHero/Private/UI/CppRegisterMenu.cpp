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
	btnBack->OnClicked.AddDynamic(this, &UCppRegisterMenu::OnBackClicked);
	btnRegister->OnClicked.AddDynamic(this, &UCppRegisterMenu::OnRegisterClicked);

	inputUserEmail->OnTextChanged.AddDynamic(this, &UCppRegisterMenu::OnUserEmailInput);
	inputUserName->OnTextChanged.AddDynamic(this, &UCppRegisterMenu::OnUserNameInput);
	inputPassword->OnTextChanged.AddDynamic(this, &UCppRegisterMenu::OnPasswordInput);
}

void UCppRegisterMenu::Init(TArray<UObject*> initParams)
{
	UCppBaseMenu::Init(initParams);
}

void UCppRegisterMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppRegisterMenu::OnBackClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Back Clicked!"));
	Pop();

	TArray<UObject*> initParams;
	ACppUIManager::Push(ACppUIManager::Instance()->loginMenu, initParams);
}

void UCppRegisterMenu::OnRegisterClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Register Clicked!"));
	if (!ACppPlayFabManager::Instance()->userEmail.IsEmpty()
	 && !ACppPlayFabManager::Instance()->registerUserName.IsEmpty()
	 && !ACppPlayFabManager::Instance()->registerUserPassword.IsEmpty())
	{
		ACppPlayFabManager::Instance()->GetRegister();
	}
}

void UCppRegisterMenu::OnUserEmailInput(const FText& text)
{
	if (!text.IsEmptyOrWhitespace())
	{
		ACppPlayFabManager::Instance()->userEmail = text.ToString();
	}
}

void UCppRegisterMenu::OnUserNameInput(const FText& text)
{
	if (!text.IsEmptyOrWhitespace())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, text.ToString());
		ACppPlayFabManager::Instance()->registerUserName = text.ToString();
	}
}

void UCppRegisterMenu::OnPasswordInput(const FText& text)
{
	if (!text.IsEmptyOrWhitespace())
	{
		ACppPlayFabManager::Instance()->registerUserPassword = text.ToString();
	}
}

void UCppRegisterMenu::OnRegisterSuccess()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("On Register Success!"));
	Pop();

	TArray<UObject*> initParams;
	ACppUIManager::Push(ACppUIManager::Instance()->mainMenu, initParams);

	ACppPlayFabManager::Instance()->registerUserPassword = TEXT("");
}

void UCppRegisterMenu::OnRegisterError()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("On Register Error!"));

	ACppPlayFabManager::Instance()->registerUserPassword = TEXT("");
}