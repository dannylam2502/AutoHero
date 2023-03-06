// Fill out your copyright notice in the Description page of Project Settings.


#include "CppMainMenu.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UCppMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GenerateRandom();

	btnGenerate->OnClicked.AddUniqueDynamic(this, &UCppMainMenu::OnPlayClicked);
}

void UCppMainMenu::OnPlayClicked()
{
	GenerateRandom();
}

void UCppMainMenu::GenerateRandom()
{
	int32 randomValue = FMath::RandRange(0, 100);
	textNumber->SetText(FText::AsNumber(randomValue));
}