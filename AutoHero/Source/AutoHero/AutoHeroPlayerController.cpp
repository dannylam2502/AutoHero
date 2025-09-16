// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoHeroPlayerController.h"

#include "AutoHeroGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/UnitCell.h"
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
	GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Red, FString::Printf(TEXT("Submitted to Server Num = %d"), LocalPendingUnits.Num()));
	// Send to Server
	AAutoHeroPlayerState* AAPlayerState = GetPlayerState<AAutoHeroPlayerState>();
	if (AAPlayerState)
	{
		AAPlayerState->ServerProcessPendingUnits(LocalPendingUnits);
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
			// AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitRemovedFromField.Broadcast(LocalUnit.BaseUnit);
			LocalUnit.BaseUnit->Destroy();
		}
	}
	LocalPendingUnits.Empty();
}

void AAutoHeroPlayerController::ServerStartQuickTest_Implementation()
{
	ANormalModeGameState* GameState = GetWorld()->GetGameState<ANormalModeGameState>();
	if (GameState)
	{
		// Hack
		GameState->SetCurrentGamePhase(EGamePhase::S7_Preparation_Turn3_Red);
		GameState->ChangeToNextGamePhase();
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

	//ServerGenerateUnitList();

	// Check if this is Client
	ENetMode NetMode = GetNetMode();
	if (NetMode == NM_Standalone || NetMode == NM_Client)
	{
		AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitDropped.AddDynamic(this, &AAutoHeroPlayerController::OnClientUnitDropped);
		AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitOccupied.AddDynamic(this, &AAutoHeroPlayerController::OnClientUnitOccupied);
		AClientGameEventManager::GetInstance(GetWorld())->OnClientGamePhaseChanged.AddDynamic(this, &AAutoHeroPlayerController::OnClientGamePhaseChanged);
		AClientGameEventManager::GetInstance(GetWorld())->OnClientVacateCell.AddDynamic(this, &AAutoHeroPlayerController::OnClientVacateCell);
		AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitRemovedFromField.AddDynamic(this, &AAutoHeroPlayerController::OnClientUnitRemovedFromField);
	
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
			Manager->OnClientUnitOccupied.RemoveDynamic(this, &AAutoHeroPlayerController::OnClientUnitOccupied);
			Manager->OnClientVacateCell.RemoveDynamic(this, &AAutoHeroPlayerController::OnClientVacateCell);
			Manager->OnClientUnitRemovedFromField.RemoveDynamic(this, &AAutoHeroPlayerController::OnClientUnitRemovedFromField);
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

	TArray<FGeneratedUnitInfoDTO> RandomUnitsInfo;
	while (RandomUnitsInfo.Num() < 10 && AllHeroIDs.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, AllHeroIDs.Num() - 1);
		FGeneratedUnitInfoDTO dto;
		dto.UnitType = AllHeroIDs[Index];
		RandomUnitsInfo.Add(dto);
		//AllHeroIDs.RemoveAt(Index);
	}

	AAutoHeroPlayerState* PS = GetPlayerState<AAutoHeroPlayerState>();
	if (PS)
	{
		PS->SetCurGeneratedUnitIDs(RandomUnitsInfo);
	}
}

void AAutoHeroPlayerController::ServerClearSelectableUnitsList_Implementation()
{
	AAutoHeroPlayerState* PS = GetPlayerState<AAutoHeroPlayerState>();
	if (PS)
	{
		PS->SetCurGeneratedUnitIDs(TArray<FGeneratedUnitInfoDTO>());
	}
}

bool AAutoHeroPlayerController::ServerClearSelectableUnitsList_Validate()
{
	return true;
}

void AAutoHeroPlayerController::OnClientUnitDropped(ABaseUnit* BaseUnit, FVector2D InDropPosition)
{
	
}

void AAutoHeroPlayerController::OnClientUnitOccupied(ABaseUnit* BaseUnit, FVector2D InGridPosition)
{
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, TEXT("OnClientUnitOccupied"));
	FPendingUnitData PendingData;
	PendingData.UnitType = BaseUnit->UnitType;
	PendingData.UnitLocation = BaseUnit->GetActorLocation();
	PendingData.PlacementTime = BaseUnit->GetTimeSpawned();
	PendingData.BaseUnit = BaseUnit;
	PendingData.GridPosition.X = BaseUnit->GetCurrentCell()->GetCellCol();
	PendingData.GridPosition.Y = BaseUnit->GetCurrentCell()->GetCellRow();
	LocalPendingUnits.Add(PendingData);

	UpdateCrownVisuals();
}

void AAutoHeroPlayerController::OnClientGamePhaseChanged(EGamePhase GamePhase)
{
	FString PhaseName = StaticEnum<EGamePhase>()->GetValueAsString(GamePhase);
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, FString::Printf(TEXT("GamePhase = %s"), *PhaseName));
	AAutoHeroPlayerState* AAPlayerState = this->GetPlayerState<AAutoHeroPlayerState>();
	// Blue Team cases
	if (AAPlayerState->GetTeam() == EActorTeam::Blue)
	{
		if (GamePhase == EGamePhase::S2_Preparation_Turn1_Blue
			|| GamePhase == EGamePhase::S5_Preparation_Turn2_Blue
			|| GamePhase == EGamePhase::S6_Preparation_Turn3_Blue)
		{
			//FString CleanName = StaticEnum<EGamePhase>()->GetNameStringByValue(static_cast<int64>(PlayerState->GetTeam()));
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, TEXT("Round 1 Blue Team Blue"));
			// Blue Team, ask for the list, it will update player state and then the UI will be updated correctly after
			ServerGenerateUnitList();
		}
		// else if (GamePhase == EGamePhase::S3_Preparation_Round1_Red
		// 	|| GamePhase == EGamePhase::S5_Preparation_Round2_Red
		// 	|| GamePhase == EGamePhase::S7_Preparation_Round3_Red)
		else
		{
			ServerClearSelectableUnitsList();
		}
	}
	else // Red Team cases
	{
		if (GamePhase == EGamePhase::S3_Preparation_Turn1_Red
			|| GamePhase == EGamePhase::S4_Preparation_Turn2_Red
			|| GamePhase == EGamePhase::S7_Preparation_Turn3_Red)
		{
			ServerGenerateUnitList();
		}
		else
		{
			ServerClearSelectableUnitsList();
		}
	}
}

void AAutoHeroPlayerController::OnClientVacateCell(ABaseUnit* BaseUnit)
{
	for (int i = 0; i < LocalPendingUnits.Num(); i++)
	{
		if (LocalPendingUnits[i].BaseUnit == BaseUnit)
		{
			LocalPendingUnits.RemoveAt(i);
			break;
		}
	}
	UpdateCrownVisuals();
}

void AAutoHeroPlayerController::OnClientUnitRemovedFromField(ABaseUnit* BaseUnit)
{
	for (int i = 0; i < LocalPendingUnits.Num(); i++)
	{
		if (LocalPendingUnits[i].BaseUnit == BaseUnit)
		{
			LocalPendingUnits.RemoveAt(i);
			break;
		}
	}
}

void AAutoHeroPlayerController::ServerNotifyClientMergeReady_Implementation()
{
	ANormalModeGameState* GameState = GetWorld() ? GetWorld()->GetGameState<ANormalModeGameState>() : nullptr;
	if (GameState)
	{
		GameState->OnClientReportedMergeReady(this);
	}
}

bool AAutoHeroPlayerController::ServerNotifyClientMergeReady_Validate()
{
	return true;
}

void AAutoHeroPlayerController::UpdateSelectableUnitsUI(EActorTeam Team, TArray<FGeneratedUnitInfoDTO> SelectableUnitsDTO)
{
	AClientGameEventManager::GetInstance(GetWorld())->BroadCastSelectableUnitsGeneratedEvent(Team, SelectableUnitsDTO);
}

void AAutoHeroPlayerController::UpdateCrownVisuals()
{
    // Group units by row
    TMap<int32, TArray<FPendingUnitData>> UnitsByRow;

    for (const FPendingUnitData& Data : LocalPendingUnits)
    {
        int32 Row = static_cast<int32>(Data.GridPosition.Y);
        UnitsByRow.FindOrAdd(Row).Add(Data);
    }

    // Clear all crowns
    for (const FPendingUnitData& Data : LocalPendingUnits)
    {
        if (IsValid(Data.BaseUnit))
        {
            Data.BaseUnit->ShowCrown(false);
        }
    }

    // Process each row
    for (auto& RowPair : UnitsByRow)
    {
        TArray<FPendingUnitData>& RowUnits = RowPair.Value;

        // Group units by UnitID within this row
        TMap<int32, TArray<FPendingUnitData>> UnitsByID;
        for (const FPendingUnitData& Data : RowUnits)
        {
            UnitsByID.FindOrAdd(Data.UnitType).Add(Data);
        }

        for (auto& IDPair : UnitsByID)
        {
            TArray<FPendingUnitData>& SameUnits = IDPair.Value;

            if (SameUnits.Num() < 2)
            {
                continue; // Need at least 2 of the same unit ID
            }

        	// Try to find the first placed unit on a special cell
        	for (FPendingUnitData& Data : SameUnits)
        	{
        		if (IsValid(Data.BaseUnit) &&
					Data.BaseUnit->GetCurrentCell() &&
					Data.BaseUnit->GetCurrentCell()->IsSpecial())
        		{
        			Data.BaseUnit->ShowCrown(true);
        			goto NextIDGroup;
        		}
        	}

            // Sort by PlacementTime (earliest first)
            SameUnits.Sort([](const FPendingUnitData& A, const FPendingUnitData& B)
            {
                return A.PlacementTime < B.PlacementTime;
            });

            // If no special cell found, fallback to first placed unit
            if (IsValid(SameUnits[0].BaseUnit))
            {
                SameUnits[0].BaseUnit->ShowCrown(true);
            }

        NextIDGroup:
            continue;
        }
    }
}

void AAutoHeroPlayerController::SendCheatToServer_Implementation(int CheatID)
{
	AAutoHeroPlayerState* AAPlayerState = GetPlayerState<AAutoHeroPlayerState>();
	if (AAPlayerState)
	{
		AAPlayerState->SendCheatToGameState(CheatID);
	}
}

bool AAutoHeroPlayerController::SendCheatToServer_Validate(int CheatID)
{
	return true;
}

