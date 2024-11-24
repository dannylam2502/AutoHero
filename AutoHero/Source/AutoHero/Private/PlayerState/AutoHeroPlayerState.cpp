// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/AutoHeroPlayerState.h"

#include "Net/UnrealNetwork.h"


void AAutoHeroPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAutoHeroPlayerState, SelectedUnitIds);
	DOREPLIFETIME(AAutoHeroPlayerState, CurrentUnitIds);
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
		// Server code
		ServerSetSelectedUnits(SelectedUnitIds);
	}
	else
	{
		// Client code, allow requesting to server function
		ServerSetSelectedUnits(SelectedUnitIds);
	}
}

void AAutoHeroPlayerState::OnRep_SelectedUnitIds()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("OnRep_SelectedUnitIDs"));
}

void AAutoHeroPlayerState::ServerSetSelectedUnits_Implementation(const TArray<int32>& UnitIDs)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ServerSetSelectedUnits_Implementation"));
	SelectedUnitIds = UnitIDs;
}

bool AAutoHeroPlayerState::ServerSetSelectedUnits_Validate(const TArray<int32>& UnitIDs)
{
	return true;
}
