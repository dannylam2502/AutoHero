// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CppMainMenu.h"
#include "Components/Button.h"
#include "EngineGlobals.h"
#include "AutoHero/AutoHeroGameMode.h"

void UCppMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	gameMode = dynamic_cast<AAutoHeroGameMode*>(GetWorld()->GetAuthGameMode());
	btnSetting->OnClicked.AddDynamic(this, &UCppMainMenu::OnSettingClicked);
	btnPvp->OnClicked.AddDynamic(this, &UCppMainMenu::OnPvpClicked);
	btnPve->OnClicked.AddDynamic(this, &UCppMainMenu::OnPveClicked);
}

void UCppMainMenu::OnSettingClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnSettingClicked!"));

	if (gameMode)
	{
		gameMode->GoInGame();
	}
}

void UCppMainMenu::OnPvpClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnPvpClicked!"));
	SetVisibility(ESlateVisibility::Hidden);
}

void UCppMainMenu::OnPveClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 11.0f, FColor::Red, TEXT("OnPveClicked!"));
}
