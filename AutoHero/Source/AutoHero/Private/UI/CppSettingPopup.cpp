// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppSettingPopup.h"
#include "UI/CppUIManager.h"
#include "Components/Button.h"
#include "EngineGlobals.h"

void UCppSettingPopup::Setup()
{
	UCppBaseMenu::Setup();

	btnBack->OnClicked.AddDynamic(this, &UCppSettingPopup::OnBackClicked);
	btnMusic->OnClicked.AddDynamic(this, &UCppSettingPopup::OnMusicClicked);
	btnSound->OnClicked.AddDynamic(this, &UCppSettingPopup::OnSoundClicked);
}

void UCppSettingPopup::Init(TArray<UObject*> initParams)
{
	UCppBaseMenu::Init(initParams);
}
	
void UCppSettingPopup::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppSettingPopup::OnBackClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnBackClicked!"));
	Pop();
}

void UCppSettingPopup::OnMusicClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnMusicClicked!"));
}

void UCppSettingPopup::OnSoundClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnSoundClicked!"));
}