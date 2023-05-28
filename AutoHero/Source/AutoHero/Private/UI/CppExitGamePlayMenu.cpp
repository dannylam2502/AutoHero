// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppExitGamePlayMenu.h"
#include "Components/Button.h"

#include "Systems/CppGameInstance.h"
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
	UCppGameInstance::I()->Push(UCppGameInstance::I()->blockPopup);

	if (UCppGameInstance::I()->isHost)
	{
		UCppGameInstance::I()->DestroySession(FName(*UCppGameInstance::I()->sessionName));
		UCppGameInstance::I()->SetInputUI();
	}
	else if (UCppGameInstance::I()->isClient)
	{
		UCppGameInstance::I()->UnregisterPlayer(FName(*UCppGameInstance::I()->sessionName));
		UCppGameInstance::I()->SetInputUI();
	}
}

void UCppExitGamePlayMenu::OnQuitClicked()
{
	UCppGameInstance::QuitGame();
}
