#include "Core/GameState/NormalModeGameState.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameInstances/NormalGameInstance.h"
#include "PlayerState/AutoHeroPlayerState.h"
#include "Singletons/UnitDataManager.h"

ANormalModeGameState::ANormalModeGameState()
{
    CurrentRound = 0;
    bIsPreparationPhase = false;
}

void ANormalModeGameState::BeginPlay()
{
    Super::BeginPlay();
}

void ANormalModeGameState::StartPreparation()
{
    if (HasAuthority())
    {
        bIsPreparationPhase = true;
        OnRep_IsPreparationPhase();
        GetWorldTimerManager().SetTimer(PreparationTimerHandle, this, &ANormalModeGameState::EndPreparation, 30.0f, false);
    }
}

void ANormalModeGameState::EndPreparation()
{
    if (HasAuthority())
    {
        bIsPreparationPhase = false;
        OnRep_IsPreparationPhase();
        StartRound();
    }
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

bool ANormalModeGameState::IsPreparationPhase()
{
    return bIsPreparationPhase;
}

int32 ANormalModeGameState::GetCurrentRound()
{
    return CurrentRound;
}

void ANormalModeGameState::ServerOnLevelLoaded()
{
    // Server-side logic for level loaded
    UE_LOG(LogTemp, Log, TEXT("Level Loaded on Server!"));

    // Example: Notify all clients
    MulticastOnLevelLoaded();
}

void ANormalModeGameState::SetSymmetricView(APlayerController* PlayerController)
{
    if (!PlayerController) return;

    // Get the controlled pawn (player's character)
    APawn* ControlledPawn = PlayerController->GetPawn();
    if (!ControlledPawn) return;

    // Get the spring arm component attached to the pawn
    USpringArmComponent* SpringArm = ControlledPawn->FindComponentByClass<USpringArmComponent>();
    if (!SpringArm) return;

    // Get the current spring arm rotation
    FRotator SpringArmRotation = SpringArm->GetComponentRotation();

    // Flip the Z (yaw) component for a mirrored view
    SpringArmRotation.Yaw += 180.0f;
    SpringArmRotation.Yaw = FMath::Fmod(SpringArmRotation.Yaw, 360.0f); // Keep yaw within [0, 360)

    // Apply the flipped rotation
    SpringArm->SetWorldRotation(SpringArmRotation);

    // Inform other systems of the player's role
    // NotifySymmetricViewSetup(PlayerController, bIsPlayer1);
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

        // 5. Initialize Unit
        NewUnit->SetUnitID(UnitData->UnitID);
        NewUnit->SetUnitState(EUnitState::WaitingForBattle);
        NewUnit->ETeam = Team;

        // If Team Red Rotate Y to face Enemy
        if (NewUnit->ETeam == EActorTeam::Red)
        {
            NewUnit->MulticastRotateToFaceEnemy();
        }

        // 6. Add to ServerConfirmedUnits
        TeamToUnitMap.Add(Team, NewUnit);
        UE_LOG(LogTemp, Log, TEXT("Successfully added UnitID: %d to ServerConfirmedUnits"), UnitData->UnitID);
    }
}

void ANormalModeGameState::MulticastOnLevelLoaded_Implementation()
{
    // Logic for both server and clients when level is loaded
    UE_LOG(LogTemp, Log, TEXT("Level Loaded for all clients!"));
    // Get the game instance and trigger the delegate
    UNormalGameInstance* GameInstance = Cast<UNormalGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->TriggerLevelLoaded();
    }

    // Iterate through all player controllers and set symmetric view for Player 2
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        APlayerController* PlayerController = Iterator->Get(); // Get the player controller
        if (!PlayerController) continue;
        AAutoHeroPlayerState* PlayerState = Cast<AAutoHeroPlayerState>(PlayerController->PlayerState);
        // Check if this is Player 2 based on some custom logic (e.g., index or role)
        if (PlayerState->GetPlayerIndex() == 0) // Assuming NetPlayerIndex == 1 for Player 2
        {
            SetSymmetricView(PlayerController); // Call your symmetric view function
        }
    }
}

void ANormalModeGameState::StartRound()
{
    if (HasAuthority())
    {
        CurrentRound++;
        OnRep_RoundState();
        // Logic to start the round, e.g., enable player controls, etc.
        GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &ANormalModeGameState::EndRound, 60.0f, false); // Example round duration
    }
}

void ANormalModeGameState::EndRound()
{
    if (HasAuthority())
    {
        if (CurrentRound < 3)
        {
            StartPreparation();
        }
        else
        {
            // Logic for game over
        }
    }
}

void ANormalModeGameState::OnRep_RoundState()
{
    // Logic to handle round state changes on clients
    if (CurrentRound > 0)
    {
        // Logic for starting a round
    }
}

void ANormalModeGameState::OnRep_IsPreparationPhase()
{
    // Logic to handle preparation phase changes on clients
    if (bIsPreparationPhase)
    {
        // Logic for starting preparation phase
    }
    else
    {
        // Logic for ending preparation phase
    }
    UpdateAllAIBlackboardKeys(!bIsPreparationPhase);
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

void ANormalModeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANormalModeGameState, CurrentRound);
    DOREPLIFETIME(ANormalModeGameState, bIsPreparationPhase);
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
    StartPreparation();
}
