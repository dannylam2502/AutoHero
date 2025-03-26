// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	virtual void ClientInitialize(AController* C) override;
	// The list of selected units on this player's board, will be replicated
	UPROPERTY(ReplicatedUsing = OnRep_SelectedUnitIds, BlueprintReadOnly, Category = "Units")
	TArray<int32> SelectedUnitIds;

	// The Current List of Units that this player can choose from, appear from the UI
	UPROPERTY(Replicated)
	TArray<int32> CurrentUnitIds;

	void SetSelectedUnitIDs(const TArray<int32>& UnitIDs);
	void SetCurrentUnitIDs(const TArray<int32>& UnitIDs);
	
	UFUNCTION()
	void OnRep_SelectedUnitIds();

	UFUNCTION()
	void OnClientUnitSpawned(ABaseUnit* BaseUnit);
	UFUNCTION()
	void OnClientUnitRemoved(ABaseUnit* BaseUnit);

	void SetPlayerIndex(int InPlayerIndex);
	int GetPlayerIndex();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ProcessPendingUnits(const TArray<FPendingUnitData>& PendingUnits);
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerIndex, BlueprintReadOnly, Category = "Units")
	int PlayerIndex;
	// Confirmed Units from Server
	TArray<ABaseUnit*> ServerConfirmedUnits;
	
	UFUNCTION()
	void OnRep_PlayerIndex();
};
