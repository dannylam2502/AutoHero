#include "GameState/NormalModeGameState.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstances/NormalGameInstance.h"
#include "PlayerState/AutoHeroPlayerState.h"

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
