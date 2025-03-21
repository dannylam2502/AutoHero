// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/AutoHeroPlayerState.h"

#include "AutoHero/AutoHeroPlayerController.h"
#include "Events/ClientGameEventManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Singletons/UnitDataManager.h"


void AAutoHeroPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAutoHeroPlayerState, SelectedUnitIds);
	DOREPLIFETIME(AAutoHeroPlayerState, CurrentUnitIds);
	DOREPLIFETIME(AAutoHeroPlayerState, PlayerIndex);
	DOREPLIFETIME(AAutoHeroPlayerState, PendingUnits);
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
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("OnRep_PendingUnits"));
	APlayerController* PlayerController = GetPlayerController();
	if (AAutoHeroPlayerController* AutoHeroPlayerController = Cast<AAutoHeroPlayerController>(PlayerController))
	{
		AutoHeroPlayerController->UpdateUnitOnFieldFromServer(PendingUnits);
	}
}

void AAutoHeroPlayerState::Server_SpawnPendingUnits(const TArray<FPendingUnitData>& ReceivedUnits)
{
	for (auto PendingUnitData : ReceivedUnits)
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

		// 6. Add to ServerConfirmedUnits
		ServerConfirmedUnits.Add(NewUnit);
		UE_LOG(LogTemp, Log, TEXT("Successfully added UnitID: %d to ServerConfirmedUnits"), UnitData->UnitID);
	}

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
	//PendingUnits = ReceivedUnits;
	Server_SpawnPendingUnits(ReceivedUnits);
	//OnRep_PendingUnits();
}

bool AAutoHeroPlayerState::Server_ProcessPendingUnits_Validate(const TArray<FPendingUnitData>& ReceivedUnits)
{
	return true;
}
