// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/CppGameData.h"
#include "SaveGame/CppUserData.h"
#include "Kismet/GameplayStatics.h"

UCppGameData* UCppGameData::instance;

UCppGameData* UCppGameData::Instance()
{
	if (instance == NULL)
	{
		instance = dynamic_cast<UCppGameData*>(UGameplayStatics::CreateSaveGameObject(UCppGameData::StaticClass()));
	}

	return instance;
}

void UCppGameData::SaveGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("Save Game!"));
	UGameplayStatics::SaveGameToSlot(Instance(), saveGameName, userIndex);
}

void UCppGameData::LoadGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Load Game!"));

	UCppGameData* gamedata = dynamic_cast<UCppGameData*>(UGameplayStatics::LoadGameFromSlot(saveGameName, userIndex));
	if (gamedata == NULL)
	{
		gamedata = Instance();
	}

#pragma region User Data.
	userData = gamedata->userData;
	if (userData == NULL)
	{
		userData = new CppUserData();
	}

#pragma endregion

}

#pragma region User Data.
void UCppGameData::SaveUserName(FString userName)
{
	userData->userName = userName;
	SaveGame();
}

void UCppGameData::SaveUserPassword(FString userPassword)
{
	userData->userPassword = userPassword;
	SaveGame();
}

void UCppGameData::SaveUserPlayFabId(FString userPlayFabId)
{
	userData->userPlayFabId = userPlayFabId;
	SaveGame();
}

void UCppGameData::SaveUserEmail(FString userEmail)
{
	userData->userEmail = userEmail;
	SaveGame();
}

#pragma endregion