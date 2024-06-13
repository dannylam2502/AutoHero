// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoHeroPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Core/Camera/AHPlayerCameraManager.h"
#include "GameMode/NormalGameMode.h"
#include "Kismet/GameplayStatics.h"

AAutoHeroPlayerController::AAutoHeroPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// Set the custom camera manager class
	PlayerCameraManagerClass = AAHPlayerCameraManager::StaticClass();
}

void AAutoHeroPlayerController::ServerGenerateHeroList_Implementation()
{
	if (HasAuthority())
	{
		GenerateHeroList();
	}
}

bool AAutoHeroPlayerController::ServerGenerateHeroList_Validate()
{
	return true;
}

void AAutoHeroPlayerController::GenerateHeroList()
{
	
}

void AAutoHeroPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AAutoHeroPlayerController::SetPlayerReady()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		// Handle directly if this is the server
		ServerSetPlayerReady_Implementation();
	}
	else
	{
		// Otherwise, send a request to the server
		ServerSetPlayerReady();
	}
}

void AAutoHeroPlayerController::ServerSetPlayerReady_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		// Ensure this function is only called on the server
		if (HasAuthority())
		{
			ANormalGameMode* GameMode = Cast<ANormalGameMode>(UGameplayStatics::GetGameMode(this));
			if (GameMode)
			{
				GameMode->PlayerReady(this);
			}
		}
	}
}

bool AAutoHeroPlayerController::ServerSetPlayerReady_Validate()
{
	
	return true;
}

void AAutoHeroPlayerController::JoinGame()
{
	if (!HasAuthority())
	{
		FString ServerAddress = TEXT("127.0.0.1:7777");
		GetWorld()->GetFirstPlayerController()->ClientTravel(ServerAddress, TRAVEL_Absolute);
	}
}
