// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CppPlatformGameInstance.h"

#include "Multiplayer/CppMultiplayerManager.h"
#include "PlayFab/CppPlayFabManager.h"
#include "UI/CppUIManager.h"

void UCppPlatformGameInstance::Init()
{
	multipPlayerManager = GetWorld()->SpawnActor<ACppMultiplayerManager>(multipPlayerManagerClass);
	playerFabManager = GetWorld()->SpawnActor<ACppPlayFabManager>(playerFabManagerClass);
	uiManager = GetWorld()->SpawnActor<ACppUIManager>(uiManagerClass);
}