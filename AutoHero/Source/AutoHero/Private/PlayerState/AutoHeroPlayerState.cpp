// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/AutoHeroPlayerState.h"

#include "AutoHero/AutoHeroPlayerController.h"
#include "Events/ClientGameEventManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"



void AAutoHeroPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAutoHeroPlayerState, SelectedUnitIds);
	DOREPLIFETIME(AAutoHeroPlayerState, CurrentUnitIds);
	DOREPLIFETIME(AAutoHeroPlayerState, PlayerIndex);
}

AAutoHeroPlayerState::AAutoHeroPlayerState()
{
	bReplicates = true;
}

void AAutoHeroPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AAutoHeroPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
	// Register some events for unit
	AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitSpawned.AddDynamic(this, &AAutoHeroPlayerState::OnClientUnitSpawned);
	AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitRemovedFromField.AddDynamic(this, &AAutoHeroPlayerState::OnClientUnitRemoved);
	// Ensure PlayerIndex is unique
	if (AController* OwnerController = GetOwner<AController>())
	{
		PlayerIndex = OwnerController->GetPlayerState<APlayerState>()->GetPlayerId();
	}

	UE_LOG(LogTemp, Log, TEXT("PlayerState initialized with PlayerIndex: %d"), PlayerIndex);
}

void AAutoHeroPlayerState::SetSelectedUnitIDs(const TArray<int32>& UnitIDs)
{
	SelectedUnitIds = UnitIDs;
}

void AAutoHeroPlayerState::SetCurrentUnitIDs(const TArray<int32>& UnitIDs)
{
	CurrentUnitIds = UnitIDs;
}

void AAutoHeroPlayerState::SendRequestSubmit()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SendRequestSubmit"));

	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SendRequestSubmit HasAuthority"));
		// Server code
		ServerSetSelectedUnits(SelectedUnitIds);
	}
	else
	{
		// Client code, allow requesting to server function
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SendRequestSubmit Not HasAuthority"));
		ServerSetSelectedUnits(SelectedUnitIds);
	}
}

void AAutoHeroPlayerState::OnRep_SelectedUnitIds()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("OnRep_SelectedUnitIDs"));
}

void AAutoHeroPlayerState::OnRep_PlayerIndex()
{
	if (HasAuthority())
	{
		
		if (this->PlayerIndex == 1)
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

void AAutoHeroPlayerState::OnRep_PendingUnits()
{
	UE_LOG(LogTemp, Log, TEXT("Units confirmed by server, updating all clients"));
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
}

int AAutoHeroPlayerState::GetPlayerIndex()
{
	return PlayerIndex;
}

void AAutoHeroPlayerState::Server_ProcessPendingUnits_Implementation(const TArray<FPendingUnitData>& ReceivedUnits)
{
	if (!HasAuthority()) return;
	PendingUnits = ReceivedUnits;
	OnRep_PendingUnits();
}

bool AAutoHeroPlayerState::Server_ProcessPendingUnits_Validate(const TArray<FPendingUnitData>& ReceivedUnits)
{
	return true;
}

void AAutoHeroPlayerState::ServerSetSelectedUnits_Implementation(const TArray<int32>& UnitIDs)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ServerSetSelectedUnits_Implementation"));
	UE_LOG(LogTemp, Warning, TEXT("ServerSetSelectedUnits has been called on the server Num = %d"), UnitIDs.Num());
	SelectedUnitIds = UnitIDs;
}

bool AAutoHeroPlayerState::ServerSetSelectedUnits_Validate(const TArray<int32>& UnitIDs)
{
	return true;
}
