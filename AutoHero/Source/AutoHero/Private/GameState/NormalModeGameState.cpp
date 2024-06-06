#include "GameState/NormalModeGameState.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

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

bool ANormalModeGameState::IsPreparationPhase()
{
    return bIsPreparationPhase;
}

int32 ANormalModeGameState::GetCurrentRound()
{
    return CurrentRound;
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
