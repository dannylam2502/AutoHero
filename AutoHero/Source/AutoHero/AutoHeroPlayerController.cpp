// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoHeroPlayerController.h"

#include "AutoHeroGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "Camera/AHPlayerCameraManager.h"
#include "Core/GameMode/NormalGameMode.h"
#include "Core/GameState/NormalModeGameState.h"
#include "Events/ClientGameEventManager.h"
#include "Kismet/GameplayStatics.h"
#include "Singletons/UnitDataManager.h"
#include "PlayerState/AutoHeroPlayerState.h"

AAutoHeroPlayerController::AAutoHeroPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// Set the custom camera manager class
	PlayerCameraManagerClass = AAHPlayerCameraManager::StaticClass();
}

void AAutoHeroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Ensure input actions are bound correctly
		if (IA_LeftMouse)
		{
			EnhancedInputComponent->BindAction(IA_LeftMouse, ETriggerEvent::Completed, this, &AAutoHeroPlayerController::OnLeftMouseReleased);
			EnhancedInputComponent->BindAction(IA_TestCameraView, ETriggerEvent::Completed, this, &AAutoHeroPlayerController::OnCameraSymmetricTest);

		}
	}
}

void AAutoHeroPlayerController::OnLeftMouseReleased()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, TEXT("OnLeftMouseReleased"));
}

void AAutoHeroPlayerController::OnCameraSymmetricTest()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, TEXT("OnCameraSymmetricTest"));
	ANormalGameMode* GameMode = GetWorld()->GetAuthGameMode<ANormalGameMode>();
	if (GameMode)
	{
		ANormalModeGameState* GameState = GameMode->GetGameState<ANormalModeGameState>();
		// Toggle between Player 1 and Player 2 for testing
		static bool bIsPlayer1 = true;
		bIsPlayer1 = !bIsPlayer1;
	}
}

void AAutoHeroPlayerController::SubmitUnitsToServer()
{
	if (LocalPendingUnits.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No pending units to submit!"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Red, FString::Printf(TEXT("Submitted to Server Num = %d"), LocalPendingUnits.Num()));
	// Send to Server
	AAutoHeroPlayerState* AAPlayerState = GetPlayerState<AAutoHeroPlayerState>();
	if (AAPlayerState)
	{
		TArray<FPendingUnitData> PendingUnitsData;
		for (auto Unit : LocalPendingUnits)
		{
			FPendingUnitData UnitData;
			UnitData.UnitID = Unit->GetUnitID();
			UnitData.UnitLocation = Unit->GetActorLocation();
			PendingUnitsData.Add(UnitData);
		}
		AAPlayerState->ServerProcessPendingUnits(PendingUnitsData);
		RemoveLocalUnitsOnField();
	}
}

void AAutoHeroPlayerController::RemoveLocalUnitsOnField()
{
	// This Client has local units, remove them
	if (!LocalPendingUnits.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Red, TEXT("Destroy Local Units"));
		for (auto LocalUnit : LocalPendingUnits)
		{
			AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitRemovedFromField.Broadcast(LocalUnit);
			LocalUnit->Destroy();
		}
	}
	LocalPendingUnits.Empty();
}

void AAutoHeroPlayerController::ServerStartQuickTest_Implementation()
{
	ANormalModeGameState* GameState = GetWorld()->GetGameState<ANormalModeGameState>();
	if (GameState)
	{
		GameState->StartBattle();
	}
}

bool AAutoHeroPlayerController::ServerStartQuickTest_Validate()
{
	return true;
}

void AAutoHeroPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	ServerGenerateUnitList();

	// Check if this is Client
	ENetMode NetMode = GetNetMode();
	if (NetMode == NM_Standalone || NetMode == NM_Client)
	{
		AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitDropped.AddDynamic(this, &AAutoHeroPlayerController::OnClientUnitDropped);
		AClientGameEventManager::GetInstance(GetWorld())->OnClientGamePhaseChanged.AddDynamic(this, &AAutoHeroPlayerController::OnClientGamePhaseChanged);
	}
}

void AAutoHeroPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ENetMode NetMode = GetNetMode();
	if (NetMode == NM_Standalone || NetMode == NM_Client)
	{
		AClientGameEventManager* Manager = AClientGameEventManager::GetInstance(GetWorld());
		if (Manager)
		{
			Manager->OnClientUnitDropped.RemoveDynamic(this, &AAutoHeroPlayerController::OnClientUnitDropped);
			Manager->OnClientGamePhaseChanged.RemoveDynamic(this, &AAutoHeroPlayerController::OnClientGamePhaseChanged);
		}
	}
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
		//AllHeroIDs.RemoveAt(Index);
	}

	AAutoHeroPlayerState* PS = GetPlayerState<AAutoHeroPlayerState>();
	if (PS)
	{
		PS->SetCurrentUnitIDs(RandomUnitIDs);
	}
}

void AAutoHeroPlayerController::OnClientUnitDropped(ABaseUnit* BaseUnit, FVector2D InDropPosition)
{
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, TEXT("OnClientUnitDropped"));
	LocalPendingUnits.Add(BaseUnit);
}

void AAutoHeroPlayerController::OnClientGamePhaseChanged(EGamePhase GamePhase)
{
	FString PhaseName = StaticEnum<EGamePhase>()->GetValueAsString(GamePhase);
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, FString::Printf(TEXT("GamePhase = %s"), *PhaseName));
	if (GamePhase == EGamePhase::Preparation_Round1_Blue)
	{
		AAutoHeroPlayerState* AAPlayerState = this->GetPlayerState<AAutoHeroPlayerState>();
		if (AAPlayerState->GetTeam() == EActorTeam::Blue)
		{
			//FString CleanName = StaticEnum<EGamePhase>()->GetNameStringByValue(static_cast<int64>(PlayerState->GetTeam()));
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, TEXT("Round 1 Blue Team Blue"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("Round 1 Blue Team Red"));
		}
	}
}
