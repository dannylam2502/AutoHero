// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoHeroGameMode.h"
#include "AutoHeroPlayerController.h"
#include "AutoHeroCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/CppUIManager.h"

AAutoHeroGameMode::AAutoHeroGameMode()
{
	static ConstructorHelpers::FClassFinder<ACppUIManager> _uiManager(TEXT("/Game/Developers/KimVanTuyen/Collections/UI/BpUIManager"));
	if (_uiManager.Class != NULL)
	{
		uiManager = _uiManager.Class.GetDefaultObject();
	}

}

void AAutoHeroGameMode::GoInGame()
{
	//// use our custom PlayerController class
	//PlayerControllerClass = AAutoHeroPlayerController::StaticClass();

	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	//if (PlayerPawnBPClass.Class != nullptr)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}

	//// set default controller to our Blueprinted controller
	//static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	//if (PlayerControllerBPClass.Class != NULL)
	//{
	//	PlayerControllerClass = PlayerControllerBPClass.Class;
	//}

}
