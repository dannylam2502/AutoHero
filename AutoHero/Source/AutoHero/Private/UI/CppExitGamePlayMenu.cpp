// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppExitGamePlayMenu.h"
#include "Components/Button.h"
#include "Multiplayer/CppMultiplayerManager.h"

#include "UI/CppUIManager.h"
#include "UI/CppBlockPopup.h"

void UCppExitGamePlayMenu::Setup()
{
	btnExit->OnClicked.AddDynamic(this, &UCppExitGamePlayMenu::OnExitClicked);
	btnQuit->OnClicked.AddDynamic(this, &UCppExitGamePlayMenu::OnQuitClicked);
}

void UCppExitGamePlayMenu::Init()
{
	UCppBaseMenu::Init();
}

void UCppExitGamePlayMenu::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppExitGamePlayMenu::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	btnExit = nullptr;
}

void UCppExitGamePlayMenu::OnExitClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("On Exit Clicked!"));
	ACppUIManager::I()->Push(ACppUIManager::I()->blockPopup);

	if (ACppMultiplayerManager::I()->isHost)
	{
		ACppMultiplayerManager::I()->DestroySession(FName(*ACppMultiplayerManager::I()->sessionName));
		ACppUIManager::I()->SetInputUI();
	}
	else if (ACppMultiplayerManager::I()->isClent)
	{
		ACppMultiplayerManager::I()->UnregisterPlayer(FName(*ACppMultiplayerManager::I()->sessionName));
		ACppUIManager::I()->SetInputUI();
	}
}

void UCppExitGamePlayMenu::OnQuitClicked()
{
	ACppUIManager::QuitGame();
}
