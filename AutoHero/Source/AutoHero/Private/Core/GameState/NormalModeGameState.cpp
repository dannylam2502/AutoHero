#include "Core/GameState/NormalModeGameState.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameInstances/NormalGameInstance.h"
#include "Events/ClientGameEventManager.h"
#include "PlayerState/AutoHeroPlayerState.h"
#include "Singletons/UnitDataManager.h"
#include "Defines/Network/NetVisualUpgradeUnit.h"
#include "Actors/UnitCell.h"
#include "Defines/Network/MergeVisualDissolveData.h"

ANormalModeGameState::ANormalModeGameState()
{
    CurrentGamePhase = EGamePhase::None;
}

void ANormalModeGameState::BeginPlay()
{
    Super::BeginPlay();
}

void ANormalModeGameState::LoadLevel(const FString& LevelName, bool ShouldBlockOnLoad)
{
    if (HasAuthority())
    {
        UGameplayStatics::LoadStreamLevel(this, FName(*LevelName), true, ShouldBlockOnLoad, FLatentActionInfo());
    }
}

void ANormalModeGameState::UnloadLevel(const FString& LevelName)
{
    if (HasAuthority())
    {
        UGameplayStatics::UnloadStreamLevel(this, FName(*LevelName), FLatentActionInfo(), true);
    }
}

EGamePhase ANormalModeGameState::GetCurrentGamePhase()
{
    return CurrentGamePhase;
}

void ANormalModeGameState::SetCurrentGamePhase(EGamePhase GamePhase)
{
    CurrentGamePhase = GamePhase;
}

void ANormalModeGameState::ServerOnLevelLoaded()
{
    // Server-side logic for level loaded
    UE_LOG(LogTemp, Log, TEXT("Level Loaded on Server!"));

    // Example: Notify all clients
    MulticastOnLevelLoaded();

    //CurrentPhase = EGamePhase::Preparation_Round1_Blue;
}

void ANormalModeGameState::MulticastOnMergedPhase_Implementation()
{
    
}

void ANormalModeGameState::MulticastVisualMergeDelete_Implementation(
    const TArray<FMergeVisualDissolveData>& MergeDataList)
{
    for (const FMergeVisualDissolveData& MergeData : MergeDataList)
    {
        UE_LOG(LogTemp, Warning, TEXT("MergeData.EffectTag = %s"), *MergeData.EffectTag.ToString());
    }
}

void ANormalModeGameState::ProcessPendingUnits(EActorTeam Team,
                                               const TArray<FPendingUnitData>& PendingUnits)
{
    for (auto PendingUnitData : PendingUnits)
    {
        // 1. Retrieve Unit Data
        FUnitData* UnitData = UUnitDataManager::Get()->GetUnitDataByID(PendingUnitData.UnitType);
        if (!UnitData)
        {
            UE_LOG(LogTemp, Error, TEXT("UnitData is NULL for UnitID: %d"), PendingUnitData.UnitType);
            continue; // Skip this iteration if UnitData is null
        }

        // 2. Retrieve Unit Template
        TSubclassOf<ABaseUnit> UnitTemplate = UnitData->UnitActorInstance;
        if (!UnitTemplate)
        {
            UE_LOG(LogTemp, Error, TEXT("UnitTemplate is NULL for UnitID: %d"), UnitData->UnitType);
            continue; // Skip this iteration if UnitTemplate is null
        }

        // 3. Check World Context
        if (!GetWorld())
        {
            UE_LOG(LogTemp, Error, TEXT("GetWorld() returned NULL!"));
            return;  // Cannot spawn without a valid world
        }

        // 4. Spawn the Unit
        FVector SpawnLocation = PendingUnitData.UnitLocation;
        FRotator SpawnRotation = FRotator::ZeroRotator;  // or a custom rotation
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        ABaseUnit* NewUnit = GetWorld()->SpawnActor<ABaseUnit>(UnitTemplate, SpawnLocation, SpawnRotation, SpawnParams);
        if (!NewUnit)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn unit for UnitID: %d"), UnitData->UnitType);
            continue; // Skip this iteration if spawning fails
        }
        else
        {
            FVector ActualLocation = NewUnit->GetActorLocation();
            UE_LOG(LogTemp, Error, TEXT("UnitID %d — Requested Location: %s | Actual Spawned Location: %s"),
                PendingUnitData.UnitType,
                *SpawnLocation.ToString(),
                *ActualLocation.ToString()
            );
        }

        // 5. Initialize Unit
        NewUnit->SetUnitType(UnitData->UnitType);
        NewUnit->SetUnitState(EUnitState::WaitingForBattle);
        NewUnit->SetReplicates(true);
        NewUnit->SetReplicateMovement(true);
        NewUnit->SetPlacementTime(PendingUnitData.PlacementTime);
        NewUnit->SetGridPosition(PendingUnitData.GridPosition);
        NewUnit->ETeam = Team;
        NewUnit->bIsClientPlaceHolder = false;

        // If Team Red Rotate Y to face Enemy
        if (NewUnit->ETeam == EActorTeam::Red)
        {
            NewUnit->RotateToFaceEnemy();
            //NewUnit->MulticastRotateToFaceEnemy();
        }

        // 6. Add to ServerConfirmedUnits
        TeamToUnitMap.FindOrAdd(Team).Add(NewUnit);
        UE_LOG(LogTemp, Log, TEXT("Successfully added UnitID: %d to ServerConfirmedUnits"), UnitData->UnitType);
    }
    // TODO: May need to check the condition, let's keep it simple for now
    if (CurrentGamePhase == EGamePhase::S2_Preparation_Turn1_Blue
        || CurrentGamePhase == EGamePhase::S3_Preparation_Turn1_Red
        || CurrentGamePhase == EGamePhase::S5_Preparation_Turn2_Blue
        || CurrentGamePhase == EGamePhase::S4_Preparation_Turn2_Red
        || CurrentGamePhase == EGamePhase::S6_Preparation_Turn3_Blue
        || CurrentGamePhase == EGamePhase::S7_Preparation_Turn3_Red)
    {
        ChangeToNextGamePhase();
    }
}

void ANormalModeGameState::MulticastOnLevelLoaded_Implementation()
{
    // UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    // if (NavSys)
    // {
    //     NavSys->Build(); // Force Rebuild
    // }
    // Logic for both server and clients when level is loaded
    UE_LOG(LogTemp, Log, TEXT("Level Loaded for all clients!"));
    // Get the game instance and trigger the delegate
    UNormalGameInstance* GameInstance = Cast<UNormalGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->TriggerLevelLoaded();
    }
}

void ANormalModeGameState::UpdateAllAIBlackboardKeys(bool bCanExecute)
{
    for (TActorIterator<AAIController> It(GetWorld()); It; ++It)
    {
        AAIController* AIController = *It;
        if (AIController && AIController->GetBlackboardComponent())
        {
            AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("bCanExecuteBehavior"), bCanExecute);
        }
    }
}

TArray<ABaseUnit*> ANormalModeGameState::GetUnitsInTeam(EActorTeam Team)
{
    return TeamToUnitMap[Team];
}

void ANormalModeGameState::StartBattle()
{
    if (!HasAuthority()) return;

    // Loop through all units (you might have a list inside GameState)
    PossessUnitsInTeam(EActorTeam::Blue);
    PossessUnitsInTeam(EActorTeam::Red);

    // Ensure all blackboard/AI setup is in place first
    this->UpdateAllAIBlackboardKeys(true);
}

void ANormalModeGameState::PossessUnitsInTeam(EActorTeam Team)
{
    TArray<ABaseUnit*>* List = this->TeamToUnitMap.Find(Team);
    if (!List) return;
    for (ABaseUnit* Unit : *List)
    {
        if (!Unit || Unit->bIsClientPlaceHolder) continue;

        // Skip if already possessed
        if (Unit->GetController()) continue;

        // Spawn AIController manually
        if (Unit->AIControllerClass)
        {
            AAIController* NewAI = GetWorld()->SpawnActor<AAIController>(Unit->AIControllerClass);
            if (NewAI)
            {
                NewAI->Possess(Unit);
                // Optional: temporarily disable AI logic to avoid unwanted movement
                // if (NewAI->BrainComponent)
                // {
                //     NewAI->BrainComponent->StopLogic(TEXT("Delaying AI activation"));
                // }

                // Ensure position is locked to exact grid (skip navmesh adjustment)
                // FVector SnappedLocation = Unit->GetActorLocation();
                // Unit->SetActorLocation(SnappedLocation, false, nullptr, ETeleportType::TeleportPhysics);

                UE_LOG(LogTemp, Log, TEXT("AIController possessed UnitID %d at %s"),
                    Unit->GetUnitType(), *Unit->GetActorLocation().ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AIController for UnitID %d"), Unit->GetUnitType());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No AIControllerClass set on UnitID %d"), Unit->GetUnitType());
        }
    }
}

void ANormalModeGameState::ClientHandleGamePhaseChanged()
{
    AClientGameEventManager::GetInstance(GetWorld())->OnClientGamePhaseChanged.Broadcast(CurrentGamePhase);
    if (CurrentGamePhase == EGamePhase::S1_Start)
    {
        
    }
    else if (CurrentGamePhase == EGamePhase::S2_Preparation_Turn1_Blue)
    {
        // Blue turn 1
    }
}

void ANormalModeGameState::ChangeToNextGamePhase()
{
    CurrentGamePhase = GetNextGamePhase();
    OnGamePhaseChanged();
}

EGamePhase ANormalModeGameState::GetNextGamePhase()
{
    // Cast enum to int, increment, and clamp
    int32 NextPhaseIndex = static_cast<int32>(CurrentGamePhase) + 1;
    int32 MaxPhaseIndex = static_cast<int32>(EGamePhase::S9_Ended);

    if (NextPhaseIndex > MaxPhaseIndex)
    {
        // Optionally loop or clamp at the last phase
        NextPhaseIndex = MaxPhaseIndex;
    }

    return static_cast<EGamePhase>(NextPhaseIndex);
}

void ANormalModeGameState::HandleMergeLogic()
{
    // Row -> List of units on that row
    TMap<int32, TArray<ABaseUnit*>> UnitsByRow;
    for (TPair<EActorTeam, TArray<ABaseUnit*>>& Pair : TeamToUnitMap)
    {
        for (ABaseUnit* Unit : Pair.Value)
        {
            if (!IsValid(Unit)) continue;

            int32 RowIndex = Unit->GetGridPosition().Y;
            UnitsByRow.FindOrAdd(RowIndex).Add(Unit);
        }
    }

    TArray<FNetVisualUpgradeUnit> NetVisualUpgradeUnitDTO;
    TArray<FMergeVisualDissolveData> MergeDataList;
    // Process each row
    for (TPair<int32, TArray<ABaseUnit*>>& RowPair : UnitsByRow)
    {
        TArray<ABaseUnit*> RowUnits = RowPair.Value;

        // Group units by UnitID within this row
        TMap<int32, TArray<ABaseUnit*>> UnitsByType;
        for (ABaseUnit* Unit : RowUnits)
        {
            UnitsByType.FindOrAdd(Unit->UnitType).Add(Unit);
        }

        for (TPair<int32, TArray<ABaseUnit*>>& IDPair : UnitsByType)
        {
            TArray<ABaseUnit*> SameUnits = IDPair.Value;

            if (SameUnits.Num() < 2)
            {
                continue; // Need at least 2 of the same unit ID
            }

            // Sort by PlacementTime (earliest first)
            Algo::Sort(SameUnits, [](ABaseUnit* A, ABaseUnit* B)
            {
                return A->GetPlacementTime() < B->GetPlacementTime();
            });
            // SameUnits.Sort([](ABaseUnit* A, ABaseUnit* B)
            // {
            //     return A->GetPlacementTime() < B->GetPlacementTime();
            // });

            bool bIsFoundInSpecial = false;
            ABaseUnit* UpgradedUnit = nullptr;
            // Try to find the first placed unit on a special cell
            for (ABaseUnit* Unit : SameUnits)
            {
                if (IsValid(Unit) &&
                    Unit->GetCurrentCell() &&
                    Unit->GetCurrentCell()->IsSpecial())
                {
                    UpgradedUnit = Unit;
                    bIsFoundInSpecial = true;
                }
            }

            // If no special cell found, fallback to first placed unit
            if (!bIsFoundInSpecial && IsValid(SameUnits[0]))
            {
                //SameUnits[0]->ShowCrown(true);
                UpgradedUnit = SameUnits[0];
            }

            // Found the Upgrade Unit, send it multicast to client and destroy the rest
            if (UpgradedUnit)
            {
                // FNetVisualUpgradeUnit VisualUpgradeUnit;
                // VisualUpgradeUnit.GridPosition = UpgradedUnit->GetGridPosition();
                // VisualUpgradeUnit.ToUnitID = UpgradedUnit->UnitType; // TODO UnitType->UnitID
                // for (ABaseUnit* Unit : SameUnits)
                // {
                //     if (Unit != UpgradedUnit)
                //     {
                //         VisualUpgradeUnit.FromUnitIDs.Add(Unit->UnitType);
                //     }
                // }
                FMergeVisualDissolveData MergeData;
                MergeData.TargetGridPosition = UpgradedUnit->GetGridPosition();
                MergeData.EffectTag = "Tags I Choose";
                //MergeData.target = UpgradedUnit->UnitType; // TODO UnitType->UnitID
                for (ABaseUnit* Unit : SameUnits)
                {
                    if (Unit != UpgradedUnit)
                    {
                        MergeData.FromUnitInstanceIDs.Add(Unit->UnitType);
                    }
                }
                MergeDataList.Add(MergeData);
            }
        }
    }

    // Send Multicast Dissolve
    MulticastVisualMergeDelete(MergeDataList);
}

void ANormalModeGameState::OnRep_CurrentPhaseState()
{
    FString PhaseName = StaticEnum<EGamePhase>()->GetValueAsString(CurrentGamePhase);
    UE_LOG(LogTemp, Display, TEXT("Current Phase is %s"), *PhaseName);

    if (GetNetMode() == NM_Client)
    {
        ClientHandleGamePhaseChanged();
    }
}

void ANormalModeGameState::OnGamePhaseChanged()
{
    if (CurrentGamePhase == EGamePhase::S8_Merge)
    {
        HandleMergeLogic();
    }
    else if (CurrentGamePhase == EGamePhase::S9_Battle)
    {
        StartBattle();
    }
}

void ANormalModeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANormalModeGameState, CurrentGamePhase);
}

void ANormalModeGameState::StartLoadLevelSequence()
{
    if (HasAuthority())
    {
        // Load the first streaming level with a latent action
        FLatentActionInfo LatentActionInfo;
        LatentActionInfo.CallbackTarget = this;
        LatentActionInfo.ExecutionFunction = FName("OnLevelDevMapLoaded");
        LatentActionInfo.Linkage = 0;
        LatentActionInfo.UUID = __LINE__; // Unique ID for the latent action
        UGameplayStatics::LoadStreamLevel(this, FName("Level_DevMap"), true, true, LatentActionInfo);
    }
}

void ANormalModeGameState::OnLevelDevMapLoaded()
{
    GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString("OnLevelDevMapLoaded"));
    
    // Load the second streaming level when the first one has finished loading
    FLatentActionInfo LatentActionInfo;
    LatentActionInfo.CallbackTarget = this;
    LatentActionInfo.ExecutionFunction = FName("OnIngameMapDetailLoaded");
    LatentActionInfo.Linkage = 0;
    LatentActionInfo.UUID = __LINE__; // Unique ID for the second latent action

    UGameplayStatics::LoadStreamLevel(this, FName("Level_IngameMapDetail"), true, true, LatentActionInfo);
}

void ANormalModeGameState::OnIngameMapDetailLoaded()
{
    GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString("OnIngameMapDetailLoaded"));
    ServerOnLevelLoaded();
}
