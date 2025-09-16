// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.h"
#include "Defines/GeneratedUnitInfoDTO.h"
#include "Defines/PendingUnitData.h"
#include "GameFramework/PlayerState.h"
#include "AutoHeroPlayerState.generated.h"

class ABaseUnit;
/**
 * 
 */
UCLASS()
class AUTOHERO_API AAutoHeroPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AAutoHeroPlayerState();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void ClientInitialize(AController* C) override;
	// The list of selected units on this player's board, will be replicated
	UPROPERTY(ReplicatedUsing = OnRep_SelectedUnitIds, BlueprintReadOnly, Category = "Units")
	TArray<int32> SelectedUnitIds;

	// The Current List of Units that this player can choose from, appear from the UI
	UPROPERTY(ReplicatedUsing = OnRep_CurGeneratedUnitsInfo)
	TArray<FGeneratedUnitInfoDTO> CurGeneratedUnitsInfo;

	void SetSelectedUnitIDs(const TArray<int32>& UnitIDs);
	void SetCurGeneratedUnitIDs(const TArray<FGeneratedUnitInfoDTO>& UnitIDs);
	
	UFUNCTION()
	void OnRep_SelectedUnitIds();

	UFUNCTION()
	void OnClientUnitSpawned(ABaseUnit* BaseUnit);
	UFUNCTION()
	void OnClientUnitRemoved(ABaseUnit* BaseUnit);

	void SetPlayerIndex(int InPlayerIndex);
	int GetPlayerIndex();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerProcessPendingUnits(const TArray<FPendingUnitData>& PendingUnits);

	UFUNCTION(BlueprintCallable)
	EActorTeam GetTeam();

	/*
	 * Send cheat to gamemode
	 */
	void SendCheatToGameState(int CheatID);
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerIndex, BlueprintReadOnly, Category = "Units")
	int PlayerIndex;
	// Confirmed Units from Server
	TArray<ABaseUnit*> ServerConfirmedUnits;
	
	UFUNCTION()
	void OnRep_PlayerIndex();

	UFUNCTION()
	void OnRep_CurGeneratedUnitsInfo();

	UPROPERTY(Replicated)
	EActorTeam Team;
};
