// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/AutoHeroPlayerState.h"

#include "GameEnums.h"
#include "AutoHero/AutoHeroPlayerController.h"
#include "Core/GameState/NormalModeGameState.h"
#include "Events/ClientGameEventManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Singletons/UnitDataManager.h"


EActorTeam AAutoHeroPlayerState::GetTeam()
{
	return Team;
}

void AAutoHeroPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAutoHeroPlayerState, SelectedUnitIds);
	DOREPLIFETIME(AAutoHeroPlayerState, CurGeneratedUnitsInfo);
	DOREPLIFETIME(AAutoHeroPlayerState, PlayerIndex);
	DOREPLIFETIME(AAutoHeroPlayerState, Team);
}

AAutoHeroPlayerState::AAutoHeroPlayerState()
{
	bReplicates = true;
	Team = EActorTeam::Blue;
}

void AAutoHeroPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AAutoHeroPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (!HasAuthority())
	{
		AClientGameEventManager* Manager = AClientGameEventManager::GetInstance(GetWorld());
		if (Manager)
		{
			Manager->OnClientUnitSpawned.RemoveDynamic(this, &AAutoHeroPlayerState::OnClientUnitSpawned);
			Manager->OnClientUnitRemovedFromField.RemoveDynamic(this, &AAutoHeroPlayerState::OnClientUnitRemoved);
		}
	}
}

void AAutoHeroPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
	// Register some events for unit
	AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitSpawned.AddDynamic(this, &AAutoHeroPlayerState::OnClientUnitSpawned);
	AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitRemovedFromField.AddDynamic(this, &AAutoHeroPlayerState::OnClientUnitRemoved);
	// Ensure PlayerIndex is unique
	// if (AController* OwnerController = GetOwner<AController>())
	// {
	// 	int PlayerIndex = OwnerController->GetPlayerState<APlayerState>()->GetPlayerId();
	// 	UE_LOG(LogTemp, Log, TEXT("PlayerState initialized with PlayerIndex: %d"), PlayerIndex);
	// 	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, FString::Printf(TEXT("Player Index = %d"), PlayerIndex));
	// }
}

void AAutoHeroPlayerState::SetSelectedUnitIDs(const TArray<int32>& UnitIDs)
{
	SelectedUnitIds = UnitIDs;
}

void AAutoHeroPlayerState::SetCurGeneratedUnitIDs(const TArray<FGeneratedUnitInfoDTO>& UnitIDs)
{
	CurGeneratedUnitsInfo = UnitIDs;
}

void AAutoHeroPlayerState::OnRep_SelectedUnitIds()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("OnRep_SelectedUnitIDs"));
}

void AAutoHeroPlayerState::OnRep_PlayerIndex()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && PC->PlayerState == this)
	{
		Team = PlayerIndex == 2 ? EActorTeam::Red : EActorTeam::Blue;
		GEngine->AddOnScreenDebugMessage(3, 10.0f, FColor::Blue,
			FString::Printf(TEXT("PlayerState Index = %d"), PlayerIndex));
		if (Team == EActorTeam::Red)
		{
			APlayerController* PlayerController = this->GetPlayerController();
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
		}
	}
}

void AAutoHeroPlayerState::OnRep_CurGeneratedUnitsInfo()
{
}

void AAutoHeroPlayerState::OnClientUnitSpawned(ABaseUnit* BaseUnit)
{
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("OnClientUnitSpawned"));
}

void AAutoHeroPlayerState::OnClientUnitRemoved(ABaseUnit* BaseUnit)
{
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("OnClientUnitRemoved"));
}

void AAutoHeroPlayerState::SetPlayerIndex(int InPlayerIndex)
{
	this->PlayerIndex = InPlayerIndex;
	this->Team = PlayerIndex == 2 ? EActorTeam::Red : EActorTeam::Blue;
}

int AAutoHeroPlayerState::GetPlayerIndex()
{
	return PlayerIndex;
}

void AAutoHeroPlayerState::ServerProcessPendingUnits_Implementation(const TArray<FPendingUnitData>& PendingUnits)
{
	if (!HasAuthority()) return;
	ANormalModeGameState* NMGameState = GetWorld()->GetGameState<ANormalModeGameState>();
	if (NMGameState)
	{
		//EActorTeam Team = PlayerIndex == 1 ? EActorTeam::Blue : EActorTeam::Red;
		UE_LOG(LogTemp, Log, TEXT("Process Team = %hs"), Team == EActorTeam::Blue ? "Blue" : "Red");
		NMGameState->Server_ProcessPendingUnits(Team, PendingUnits);
	}
}

bool AAutoHeroPlayerState::ServerProcessPendingUnits_Validate(const TArray<FPendingUnitData>& PendingUnits)
{
	return true;
}
