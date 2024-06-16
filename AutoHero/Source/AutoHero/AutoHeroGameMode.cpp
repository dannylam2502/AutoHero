// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoHeroGameMode.h"
#include "AutoHeroPlayerController.h"
#include "AutoHeroCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAutoHeroGameMode::AAutoHeroGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAutoHeroPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void AAutoHeroGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Start the 30-second timer
	GetWorldTimerManager().SetTimer(StartGameTimerHandle, this, &AAutoHeroGameMode::StartGame, 30.0f, false);

	// Get the total number of players
	TotalPlayers = GetWorld()->GetNumPlayerControllers();
}

void AAutoHeroGameMode::PlayerReady(APlayerController* PlayerController)
{
	// Add the player to the ready list
	ReadyPlayers.Add(PlayerController);

	// Check if all players are ready
	CheckIfAllPlayersReady();
}

void AAutoHeroGameMode::CheckIfAllPlayersReady()
{
	if (ReadyPlayers.Num() >= TotalPlayers)
	{
		// If all players are ready, start the game
		GetWorldTimerManager().ClearTimer(StartGameTimerHandle);
		StartGame();
	}
}

void AAutoHeroGameMode::StartGame()
{
	// Logic to transition all players to the game state
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AAutoHeroPlayerController* PlayerController = Cast<AAutoHeroPlayerController>(It->Get());
		if (PlayerController)
		{
			PlayerController->ClientTravel(TEXT("Level_DevMap"), ETravelType::TRAVEL_Absolute);
		}
	}
}
