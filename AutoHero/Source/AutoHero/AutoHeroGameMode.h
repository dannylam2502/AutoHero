// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AutoHeroGameMode.generated.h"

UCLASS(minimalapi)
class AAutoHeroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAutoHeroGameMode();

	virtual void BeginPlay() override;
	
	TArray<APlayerController*> ReadyPlayers;
    int32 TotalPlayers;
    FTimerHandle StartGameTimerHandle;
    
    void PlayerReady(APlayerController* PlayerController);
    void CheckIfAllPlayersReady();
    void StartGame();
};



