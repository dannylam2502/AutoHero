// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NormalGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API ANormalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseUnit> ActorToSpawn;

	UFUNCTION(BlueprintCallable)
	void Spawn();

	virtual void BeginPlay() override;
	
	TArray<APlayerController*> ReadyPlayers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Networking")
	int32 RequiredNumPlayers;
	FTimerHandle StartGameTimerHandle;
    
	void PlayerReady(APlayerController* PlayerController);
	void CheckIfAllPlayersReady();
	void StartGame();
};
