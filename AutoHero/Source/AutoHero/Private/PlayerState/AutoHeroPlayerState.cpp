// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/AutoHeroPlayerState.h"

#include "Net/UnrealNetwork.h"


void AAutoHeroPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAutoHeroPlayerState, CurrentUnitIDs);
	DOREPLIFETIME(AAutoHeroPlayerState, SelectedUnitIDs);
}

void AAutoHeroPlayerState::SetSelectedUnitIDs(const TArray<int32>& UnitIDs)
{
	SelectedUnitIDs = UnitIDs;
}

void AAutoHeroPlayerState::SetCurrentUnitIDs(const TArray<int32>& UnitIDs)
{
	CurrentUnitIDs = UnitIDs;
}
