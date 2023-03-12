// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppBaseMenu.h"
#include "Blueprint/UserWidget.h"

void UCppBaseMenu::Setup()
{
}

void UCppBaseMenu::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Init!"));
	AddToViewport();
}

void UCppBaseMenu::Pop()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Pop!"));
	RemoveFromViewport();
}