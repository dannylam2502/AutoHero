// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "AutoHeroPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AAutoHeroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAutoHeroPlayerController();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGenerateHeroList();

	void GenerateHeroList();

protected:
	// To add mapping context
	virtual void BeginPlay();

	// Networking
	UFUNCTION(BlueprintCallable)
	void SetPlayerReady();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerReady();
	void ServerSetPlayerReady_Implementation();
	bool ServerSetPlayerReady_Validate();

	

	UFUNCTION(BlueprintCallable)
	void JoinGame();
};


