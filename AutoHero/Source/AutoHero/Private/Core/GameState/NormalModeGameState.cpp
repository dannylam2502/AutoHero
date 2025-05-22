#include "Core/GameState/NormalModeGameState.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameInstances/NormalGameInstance.h"
#include "PlayerState/AutoHeroPlayerState.h"
#include "Singletons/UnitDataManager.h"

ANormalModeGameState::ANormalModeGameState()
{
    CurrentPhase = EGamePhase::Start;
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

void ANormalModeGameState::ServerOnLevelLoaded()
{
    // Server-side logic for level loaded
    UE_LOG(LogTemp, Log, TEXT("Level Loaded on Server!"));

    // Example: Notify all clients
    MulticastOnLevelLoaded();

    CurrentPhase = EGamePhase::Preparation_Round1_Blue;
}

bool ANormalModeGameState::Server_ProcessPendingUnits_Validate(EActorTeam Team, const TArray<FPendingUnitData>& PendingUnits)
{
    return true;
}

void ANormalModeGameState::Server_ProcessPendingUnits_Implementation(EActorTeam Team,
                                                                     const TArray<FPendingUnitData>& PendingUnits)
{
    for (auto PendingUnitData : PendingUnits)
    {
        // 1. Retrieve Unit Data
        FUnitData* UnitData = UUnitDataManager::Get()->GetUnitDataByID(PendingUnitData.UnitID);
        if (!UnitData)
        {
            UE_LOG(LogTemp, Error, TEXT("UnitData is NULL for UnitID: %d"), PendingUnitData.UnitID);
            continue; // Skip this iteration if UnitData is null
        }

        // 2. Retrieve Unit Template
        TSubclassOf<ABaseUnit> UnitTemplate = UnitData->UnitActorInstance;
        if (!UnitTemplate)
        {
            UE_LOG(LogTemp, Error, TEXT("UnitTemplate is NULL for UnitID: %d"), UnitData->UnitID);
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
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn unit for UnitID: %d"), UnitData->UnitID);
            continue; // Skip this iteration if spawning fails
        }
        else
        {
            FVector ActualLocation = NewUnit->GetActorLocation();
            UE_LOG(LogTemp, Error, TEXT("UnitID %d — Requested Location: %s | Actual Spawned Location: %s"),
                PendingUnitData.UnitID,
                *SpawnLocation.ToString(),
                *ActualLocation.ToString()
            );
        }

        // 5. Initialize Unit
        NewUnit->SetUnitID(UnitData->UnitID);
        NewUnit->SetUnitState(EUnitState::WaitingForBattle);
        NewUnit->SetReplicates(true);
        NewUnit->SetReplicateMovement(true);
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
        UE_LOG(LogTemp, Log, TEXT("Successfully added UnitID: %d to ServerConfirmedUnits"), UnitData->UnitID);
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
                    Unit->GetUnitID(), *Unit->GetActorLocation().ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AIController for UnitID %d"), Unit->GetUnitID());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No AIControllerClass set on UnitID %d"), Unit->GetUnitID());
        }
    }
}

void ANormalModeGameState::OnRep_CurrentPhaseState()
{
    FString PhaseName = StaticEnum<EGamePhase>()->GetValueAsString(CurrentPhase);
    UE_LOG(LogTemp, Display, TEXT("Current Phase is %s"), *PhaseName);
}

void ANormalModeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANormalModeGameState, CurrentPhase);
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
