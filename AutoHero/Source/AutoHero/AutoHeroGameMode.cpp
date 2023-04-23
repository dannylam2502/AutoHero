// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoHeroGameMode.h"
#include "AutoHeroPlayerController.h"
#include "AutoHeroCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/CppUIManager.h"
#include "CppGameState.h"
#include "CppPlayerState.h"

AAutoHeroGameMode::AAutoHeroGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAutoHeroPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<ACppPlayerState> playerState(TEXT("/Game/GameSystem/BpPlayerState"));
	if (playerState.Class != nullptr)
	{
		PlayerStateClass = playerState.Class;
	}

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		DefaultPawnClass.GetDefaultObject()->SetPlayerState(PlayerStateClass.GetDefaultObject());
	}

	static ConstructorHelpers::FClassFinder<ACppGameState> gameState(TEXT("/Game/GameSystem/BpGameState"));
	if (gameState.Class != NULL)
	{
		GameStateClass = gameState.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

}
