// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoHeroGameMode.h"
#include "AutoHero/AutoHeroPlayerController.h"
#include "AutoHero/AutoHeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/CppGameInstance.h"

#include "PlayFab/CppPlayFabManager.h"
#include "UI/CppExitGamePlayMenu.h"
#include "UI/CppMultiplayerMenu.h"

AAutoHeroGameMode* AAutoHeroGameMode::i;
AAutoHeroGameMode* AAutoHeroGameMode::I()
{
	return i;
}

AAutoHeroGameMode::AAutoHeroGameMode()
{
	i = this;

	// use our custom PlayerController class
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

void AAutoHeroGameMode::BeginPlay()
{
	Super::BeginPlay();

	/*multipPlayerManager = GetWorld()->SpawnActor<UCppGameInstance>(multipPlayerManagerClass);
	playerFabManager = GetWorld()->SpawnActor<ACppPlayFabManager>(playerFabManagerClass);
	uiManager = GetWorld()->SpawnActor<ACppUIManager>(uiManagerClass);*/

	UCppGameInstance::I()->gameMode = this;

	PlayerControllerClass = GetWorld()->GetFirstPlayerController()->GetClass();
}

void AAutoHeroGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	myCharacter = nullptr;

}

void AAutoHeroGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	bool isHost = UCppGameInstance::I()->isHost;
	bool isClient = UCppGameInstance::I()->isClient;

	if (isHost || isClient)
	{
		// Spawn character
		FActorSpawnParameters SpawnParams;
		//SpawnParams.Owner = this;
		//SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector local = FVector(1000.0, 1810.0, 98.3375);
		FRotator rota = FRotator(0, 0, 0);
		myCharacter = GetWorld()->SpawnActor<AAutoHeroCharacter>(myCharacterClass, local, rota, SpawnParams);

		// Possess character
		if (myCharacter != nullptr)
		{
			NewPlayer->Possess(myCharacter);

			APlayerController* playerController = GetWorld()->GetFirstPlayerController();
			playerController->SetViewTargetWithBlend(myCharacter);
			playerController->Possess(myCharacter);
			playerController->SetInputMode(FInputModeGameAndUI());

			PlayerControllerClass = playerController->GetClass();
		}

		if (isClient)
		{
			UCppGameInstance::I()->RegisterPlayer(FName(*UCppGameInstance::I()->sessionName));
		}
	}
}

void AAutoHeroGameMode::SpawnCharacter()
{
	// Spawn character
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector local = FVector(1000.0, 1810.0, 98.3375);
	FRotator rota = FRotator(0, 0, 0);
	myCharacter = GetWorld()->SpawnActor<AAutoHeroCharacter>(myCharacterClass, local, rota, SpawnParams);

	DefaultPawnClass = myCharacterClass;

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->SetViewTargetWithBlend(myCharacter);
	playerController->Possess(myCharacter);
	playerController->SetInputMode(FInputModeGameAndUI());

	PlayerControllerClass = playerController->GetClass();
}
