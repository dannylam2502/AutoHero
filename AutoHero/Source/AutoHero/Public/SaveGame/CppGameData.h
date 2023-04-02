// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CppGameData.generated.h"

class CppUserData;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppGameData : public USaveGame
{
	GENERATED_BODY()

private:
	static UCppGameData* instance;
public:
	static UCppGameData* Instance();

private:
	const FString saveGameName = "GameData";

public:
	void SaveGame();
	void LoadGame();

#pragma region User Data.
public:
	CppUserData* userData;

	int32 userIndex = 0;

public:
	void SaveUserName(FString userName);
	void SaveUserPassword(FString userPassword);
	void SaveUserPlayFabId(FString userPlayFabId);
	void SaveUserEmail(FString userEmail);

#pragma endregion

};
