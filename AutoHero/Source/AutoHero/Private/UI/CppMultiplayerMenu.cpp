// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppMultiplayerMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "PlayFab/CppPlayFabManager.h"

#include "Systems/CppGameInstance.h"
#include "UI/CppBlockPopup.h"
#include "UI/CppExitGamePlayMenu.h"

void UCppMultiplayerMenu::Setup()
{
	btnCreateSession->OnClicked.AddDynamic(this, &UCppMultiplayerMenu::OnCreateSessionClicked);
	btnFindSession->OnClicked.AddDynamic(this, &UCppMultiplayerMenu::OnFindSessionClicked);
	btnJoinSession->OnClicked.AddDynamic(this, &UCppMultiplayerMenu::OnJoinSessionClicked);
	inputMaxPlayer->OnTextChanged.AddDynamic(this, &UCppMultiplayerMenu::OnMaxPlayerInput);
	checkBoxLAN->OnCheckStateChanged.AddDynamic(this, &UCppMultiplayerMenu::OnCheckBoxLAN);
	btnExitGamePlayMenu->OnClicked.AddDynamic(this, &UCppMultiplayerMenu::OnExitGamePlayClicked);
	btnQuit->OnClicked.AddDynamic(this, &UCppMultiplayerMenu::OnQuitClicked);
}

void UCppMultiplayerMenu::Init()
{
	UCppBaseMenu::Init();

	btnJoinSession->SetVisibility(ESlateVisibility::Hidden);
}

void UCppMultiplayerMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppMultiplayerMenu::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	btnCreateSession = nullptr;
	btnFindSession = nullptr;
	btnJoinSession = nullptr;
	inputMaxPlayer = nullptr;
	checkBoxLAN = nullptr;
}

void UCppMultiplayerMenu::OnCreateSessionClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Create Session Clicked!"));
	UCppGameInstance::I()->Push(UCppGameInstance::I()->blockPopup);

	if (!UCppGameInstance::I()->isHost)
	{
		UCppGameInstance::I()->CreateSession("Host", maxPlayerValue, isHostLAN);
	}
	else
	{
		UCppGameInstance::I()->Pop(UCppGameInstance::I()->multiplayerMenu);
	}
}

void UCppMultiplayerMenu::OnFindSessionClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Find Session Clicked!"));
	UCppGameInstance::I()->Push(UCppGameInstance::I()->blockPopup);

	UCppGameInstance::I()->FindSessions(isHostLAN);
}

void UCppMultiplayerMenu::OnJoinSessionClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Join Session Clicked!"));
	UCppGameInstance::I()->Push(UCppGameInstance::I()->blockPopup);

	UCppGameInstance::I()->JoinSessionNew(FName(*UCppGameInstance::I()->sessionName));
}

void UCppMultiplayerMenu::OnMaxPlayerInput(const FText& text)
{
	FString strText = text.ToString();
	maxPlayerValue = FCString::Atoi(*strText);
}

void UCppMultiplayerMenu::OnCheckBoxLAN(bool bIsChecked)
{
	isHostLAN = bIsChecked;
}

void UCppMultiplayerMenu::OnExitGamePlayClicked()
{
	Pop();
	UCppGameInstance::I()->Push(UCppGameInstance::I()->exitGamePlayMenu);
	UCppGameInstance::I()->SetInputGameplay();
}

void UCppMultiplayerMenu::OnQuitClicked()
{
	UCppGameInstance::QuitGame();
}
