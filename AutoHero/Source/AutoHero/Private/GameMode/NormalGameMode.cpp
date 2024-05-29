// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NormalGameMode.h"

#include "GameEnums.h"
#include "Core/Actors/BaseUnit.h"
#include "Kismet/GameplayStatics.h"

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
	GetWorldTimerManager().SetTimer(StartGameTimerHandle, this, &ANormalGameMode::StartGame, 30.0f, false);
}

void ANormalGameMode::PlayerReady(APlayerController* PlayerController)
{
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
	// Logic to transition all players to the game state
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			PlayerController->ClientTravel(TEXT("Level_DevMap"), ETravelType::TRAVEL_Absolute);
		}
	}
}
