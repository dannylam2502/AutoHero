// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines/PendingUnitData.h"
#include "GameFramework/GameStateBase.h"
#include "GameEnums.h"
#include "NormalModeGameState.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	Start,
	Preparation_Round1_Blue,
	Preparation_Round1_Red,
	Preparation_Round2_Blue,
	Preparation_Round2_Red,
	Preparation_Round3_Blue,
	Preparation_Round3_Red,
	Battle,
	Ended
};

class ABaseUnit;
/**
 * 
 */
UCLASS()
class AUTOHERO_API ANormalModeGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ANormalModeGameState();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentPhaseState)
	EGamePhase CurrentPhase;

	FTimerHandle RoundTimerHandle;
	FTimerHandle PreparationTimerHandle;

	// For Team, Players
	TMap<EActorTeam, TArray<ABaseUnit*>> TeamToUnitMap;

	UFUNCTION()
	void OnRep_CurrentPhaseState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	void StartLoadLevelSequence();
	UFUNCTION()
	void OnLevelDevMapLoaded();
	UFUNCTION()
	void OnIngameMapDetailLoaded();
	void LoadLevel(const FString& LevelName, bool ShouldBlockOnLoad);
	void UnloadLevel(const FString& LevelName);
	EGamePhase GetCurrentPhase();
	// Function to trigger level loaded logic
	UFUNCTION(BlueprintCallable)
	void ServerOnLevelLoaded();
	// Multicast delegate to notify clients
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnLevelLoaded();

	// Process submitted units from Client
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ProcessPendingUnits(EActorTeam Team, const TArray<FPendingUnitData>& PendingUnits);

	// Set the flag to control the AI behaviors
	void UpdateAllAIBlackboardKeys(bool bCanExecute);

	TArray<ABaseUnit*> GetUnitsInTeam(EActorTeam Team);

	UFUNCTION()
	void StartBattle();

	void PossessUnitsInTeam(EActorTeam Team);

	// Process the new game phase
	void ClientHandleGamePhaseChanged();
};
