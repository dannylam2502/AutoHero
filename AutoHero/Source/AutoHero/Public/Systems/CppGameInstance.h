// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CppGameInstance.generated.h"

class ACppMultiplayerManager;
class ACppPlayFabManager;
class ACppUIManager;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UCppGameInstance();

protected:
	virtual void Init() override;

public:
	UPROPERTY(EditAnywhere) TSubclassOf<class ACppMultiplayerManager> multipPlayerManagerClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class ACppPlayFabManager> playerFabManagerClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class ACppUIManager> uiManagerClass;

	ACppMultiplayerManager* multipPlayerManager;
	ACppPlayFabManager* playerFabManager;
	ACppUIManager* uiManager;
};
