// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoHeroGameMode.h"
#include "AutoHero/AutoHeroPlayerController.h"
#include "AutoHero/AutoHeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"

#include "Multiplayer/CppMultiplayerManager.h"
#include "PlayFab/CppPlayFabManager.h"
#include "UI/CppUIManager.h"

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

	GetWorld()->SpawnActor<ACppMultiplayerManager>(multipPlayerManagerClass);
	GetWorld()->SpawnActor<ACppPlayFabManager>(playerFabManagerClass);
	GetWorld()->SpawnActor<ACppUIManager>(uiManagerClass);
}

void AAutoHeroGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	myCharacter = nullptr;
}

//void AAutoHeroGameMode::PostLogin(APlayerController* NewPlayer)
//{
//	Super::PostLogin(NewPlayer);
//
//	// Spawn character
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	FVector local = FVector(1000.0, 1810.0, 98.3375);
//	FRotator rota = FRotator(0, 0, 0);
//	AAutoHeroCharacter* NewCharacter = GetWorld()->SpawnActor<AAutoHeroCharacter>(AAutoHeroCharacter::StaticClass(), local, rota, SpawnParams);
//
//	// Possess character
//	if (NewCharacter != nullptr)
//	{
//		NewPlayer->Possess(NewCharacter);
//	}
//}

void AAutoHeroGameMode::LoadLevelStreamingByName(const FName& levelName)
{
	UWorld* world = GetWorld(); // Get a pointer to the world.
	if (world != nullptr) // Ensure the world exists.
	{
		ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(world, levelName);
		if (Level != nullptr) // Ensure the level exists.
		{
			Level->bShouldBlockOnLoad = false; // The game will not freeze while the level is loading.
			// Set the level to be loaded and visible initially.
			Level->SetShouldBeLoaded(true);
			Level->SetShouldBeVisible(true);

			// Flush level streaming to ensure the level is loaded.
			world->FlushLevelStreaming(EFlushLevelStreamingType::Full);
		}
	}

}

void AAutoHeroGameMode::UnLoadLevelStreamingByName(const FName& levelName)
{
	UWorld* world = GetWorld(); // Get a pointer to the world.
	if (world != nullptr) // Ensure the world exists.
	{
		ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(world, levelName);
		if (Level != nullptr) // Ensure the level exists.
		{
			// Set the level to not be loaded and not be visible.
			Level->SetShouldBeLoaded(false);
			Level->SetShouldBeVisible(false);

			// Flush level streaming to ensure the level is unloaded.
			world->FlushLevelStreaming(EFlushLevelStreamingType::Full);
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
