// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoHeroGameMode.h"

AAutoHeroGameMode::AAutoHeroGameMode()
{
	// use our custom PlayerController class
	//PlayerControllerClass = AAutoHeroPlayerController::StaticClass();

	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	//if (PlayerPawnBPClass.Class != nullptr)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;

	//	autoHeroCharacter = dynamic_cast<AAutoHeroCharacter*>(PlayerPawnBPClass.Class.GetDefaultObject());
	//	if (autoHeroCharacter)
	//	{
	//		autoHeroCharacter->SetActorHiddenInGame(true);
	//		autoHeroCharacter->SetActorEnableCollision(false);
	//		autoHeroCharacter->SetActorTickEnabled(false);
	//	}
	//}

	//// set default controller to our Blueprinted controller
	//static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	//if (PlayerControllerBPClass.Class != NULL)
	//{
	//	PlayerControllerClass = PlayerControllerBPClass.Class;
	//}

}
