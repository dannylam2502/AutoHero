// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoHeroPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Core/Camera/AHPlayerCameraManager.h"
#include "GameMode/NormalGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Singletons/UnitDataManager.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState/AutoHeroPlayerState.h"

AAutoHeroPlayerController::AAutoHeroPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// Set the custom camera manager class
	PlayerCameraManagerClass = AAHPlayerCameraManager::StaticClass();
}

void AAutoHeroPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	ServerGenerateUnitList();
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

void AAutoHeroPlayerController::ServerGenerateUnitList_Implementation()
{
	if (HasAuthority())
	{
		GenerateUnitList();
	}
}

bool AAutoHeroPlayerController::ServerGenerateUnitList_Validate()
{
	return true;
}

void AAutoHeroPlayerController::GenerateUnitList()
{
	UUnitDataManager* UnitDataManager = UUnitDataManager::Get();
	TArray<int32> AllHeroIDs = {1,2,3,4,5}; // Assuming you have a way to get all hero IDs
	FMath::RandInit(FDateTime::Now().GetMillisecond()); // Seed random generator

	TArray<int32> RandomUnitIDs;
	while (RandomUnitIDs.Num() < 10 && AllHeroIDs.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, AllHeroIDs.Num() - 1);
		RandomUnitIDs.Add(AllHeroIDs[Index]);
		AllHeroIDs.RemoveAt(Index);
	}

	AAutoHeroPlayerState* PS = GetPlayerState<AAutoHeroPlayerState>();
	if (PS)
	{
		PS->SetCurrentUnitIDs(RandomUnitIDs);
	}
}
