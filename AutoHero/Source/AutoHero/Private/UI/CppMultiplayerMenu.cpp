// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppMultiplayerMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "PlayFab/CppPlayFabManager.h"

#include "UI/CppUIManager.h"
#include "UI/CppBlockPopup.h"

#include "Multiplayer/CppMultiplayerManager.h"

void UCppMultiplayerMenu::Setup()
{
	createSession->OnClicked.AddDynamic(this, &UCppMultiplayerMenu::OnCreateSessionClicked);
	findSession->OnClicked.AddDynamic(this, &UCppMultiplayerMenu::OnFindSessionClicked);
	joinSession->OnClicked.AddDynamic(this, &UCppMultiplayerMenu::OnJoinSessionClicked);
	inputMaxPlayer->OnTextChanged.AddDynamic(this, &UCppMultiplayerMenu::OnMaxPlayerInput);
	checkBoxLAN->OnCheckStateChanged.AddDynamic(this, &UCppMultiplayerMenu::OnCheckBoxLAN);
}

void UCppMultiplayerMenu::Init()
{
	UCppBaseMenu::Init();
}

void UCppMultiplayerMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppMultiplayerMenu::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	createSession = nullptr;
	findSession = nullptr;
	checkBoxLAN = nullptr;
}

void UCppMultiplayerMenu::OnCreateSessionClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Create Session Clicked!"));
	ACppUIManager::I()->Push(ACppUIManager::I()->blockPopup);

	if (!ACppMultiplayerManager::I()->isHost)
	{
		ACppMultiplayerManager::I()->CreateSession("Host", maxPlayerValue, isHostLAN);
	}
	else
	{
		ACppUIManager::I()->Pop(ACppUIManager::I()->multiplayerMenu);
	}
}

void UCppMultiplayerMenu::OnFindSessionClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Find Session Clicked!"));
	ACppUIManager::I()->Push(ACppUIManager::I()->blockPopup);

	ACppMultiplayerManager::I()->FindSessions(isHostLAN);
}

void UCppMultiplayerMenu::OnJoinSessionClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Join Session Clicked!"));
	ACppUIManager::I()->Push(ACppUIManager::I()->blockPopup);

	ACppMultiplayerManager::I()->JoinSession(FName(*ACppMultiplayerManager::I()->sessionName));
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