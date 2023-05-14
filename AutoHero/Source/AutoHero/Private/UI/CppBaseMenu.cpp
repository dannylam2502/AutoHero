// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppBaseMenu.h"
#include "Blueprint/UserWidget.h"

void UCppBaseMenu::Setup() { }

void UCppBaseMenu::Init()
{
	isHaveUse = true;

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, TEXT("Init!"));

	AddToViewport();
}

void UCppBaseMenu::Pop()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, TEXT("Pop!"));
	RemoveFromParent();

	if (callback.GetHandle().IsValid())
	{
		callback.Execute();
	}
	callback = nullptr;
}

void UCppBaseMenu::OnExitGame()
{
	callback = nullptr;
}