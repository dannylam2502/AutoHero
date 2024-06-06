// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NormalGameMode.h"

#include "GameEnums.h"
#include "Core/Actors/BaseUnit.h"
#include "GameState/NormalModeGameState.h"
#include "Kismet/GameplayStatics.h"

ANormalGameMode::ANormalGameMode()
{
	bGameStarted = false;
}

void ANormalGameMode::Spawn()
{
	return;
	// if (ActorToSpawn)
	// {
	// 	UWorld* world = GetWorld();
	// 	if (world)
	// 	{
	// 		FActorSpawnParameters spawnParams;
	// 		spawnParams.Owner = UGameplayStatics::GetPlayerController(world, 0);
	// 		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// 		FRotator rotator;
	// 		FVector spawnLocation(0.0f, -270.0f, 89.0f);
	// 		ABaseUnit* spawnedUnit = world->SpawnActor<ABaseUnit>(ActorToSpawn, spawnLocation, rotator, spawnParams);
	// 		spawnedUnit->ETeam = EActorTeam::Red;
	// 		if (GEngine)
	// 			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ANormalGameMode::Spawn");
	// 	}
	// }
}


void ANormalGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Start the 30-second timer
	GetWorldTimerManager().SetTimer(StartGameTimerHandle, this, &ANormalGameMode::StartGame, 30000.0f, false);
}

void ANormalGameMode::PlayerReady(APlayerController* PlayerController)
{
	if (bGameStarted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game already started"));
		return;
	}
	// Add the player to the ready list
	ReadyPlayers.Add(PlayerController);

	// Check if all players are ready
	CheckIfAllPlayersReady();
}

void ANormalGameMode::CheckIfAllPlayersReady()
{
	if (ReadyPlayers.Num() >= RequiredNumPlayers)
	{
		// If all players are ready, start the game
		GetWorldTimerManager().ClearTimer(StartGameTimerHandle);
		StartGame();
	}
}

void ANormalGameMode::StartGame()
{
	if (bGameStarted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game already started"));
		return;
	}
	bGameStarted = true;
	// Clear the timer to ensure it doesn't try to start the game again
	GetWorldTimerManager().ClearTimer(StartGameTimerHandle);

	FString LevelName = TEXT("/Game/Maps/Level_DevMap");  // Replace with your map path
	UE_LOG(LogTemp, Warning, TEXT("Starting game, traveling to map: %s"), *LevelName);

	bool bTravelSuccess = GetWorld()->ServerTravel(LevelName + TEXT("?listen"));
	if (bTravelSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerTravel called successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ServerTravel failed."));
	}

	ANormalModeGameState* NormalGameState = GetGameState<ANormalModeGameState>();
	if (NormalGameState && HasAuthority())
	{
		NormalGameState->StartPreparation();
	}
}
