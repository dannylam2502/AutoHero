// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/AutoHeroPlayerState.h"

#include "Net/UnrealNetwork.h"


void AAutoHeroPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAutoHeroPlayerState, SelectedUnitIDs);
	DOREPLIFETIME(AAutoHeroPlayerState, CurrentUnitIDs);
}

void AAutoHeroPlayerState::SetSelectedUnitIDs(const TArray<int32>& UnitIDs)
{
	SelectedUnitIDs = UnitIDs;
}

void AAutoHeroPlayerState::SetCurrentUnitIDs(const TArray<int32>& UnitIDs)
{
	CurrentUnitIDs = UnitIDs;
}

void AAutoHeroPlayerState::SendRequestSubmit()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SendRequestSubmit"));

	if (HasAuthority())
	{
		ServerSetSelectedUnits(SelectedUnitIDs);
	}
}

void AAutoHeroPlayerState::OnRep_SelectedUnitIDs()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("OnRep_SelectedUnitIDs"));
}

void AAutoHeroPlayerState::ServerSetSelectedUnits_Implementation(const TArray<int32>& UnitIDs)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ServerSetSelectedUnits_Implementation"));
	SelectedUnitIDs = UnitIDs;
}

bool AAutoHeroPlayerState::ServerSetSelectedUnits_Validate(const TArray<int32>& UnitIDs)
{
	return true;
}
