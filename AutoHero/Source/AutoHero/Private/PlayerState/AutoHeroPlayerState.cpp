// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/AutoHeroPlayerState.h"

#include "AutoHero/AutoHeroPlayerController.h"
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

void AAutoHeroPlayerState::SetPlayerIndex(int InPlayerIndex)
{
	this->PlayerIndex = InPlayerIndex;
}

int AAutoHeroPlayerState::GetPlayerIndex()
{
	return PlayerIndex;
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
