// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppMainMenu.h"
#include "Components/Button.h"
#include "EngineGlobals.h"

void UCppMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	btnSetting->OnClicked.AddDynamic(this, &UCppMainMenu::OnSettingClicked);
	btnPvp->OnClicked.AddDynamic(this, &UCppMainMenu::OnPvpClicked);
	btnPve->OnClicked.AddDynamic(this, &UCppMainMenu::OnPveClicked);
}

void UCppMainMenu::OnSettingClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnSettingClicked!"));
}

void UCppMainMenu::OnPvpClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnPvpClicked!"));
}

void UCppMainMenu::OnPveClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 11.0f, FColor::Red, TEXT("OnPveClicked!"));
}
