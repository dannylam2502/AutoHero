// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines/PendingUnitData.h"
#include "GameFramework/GameStateBase.h"
#include "GameEnums.h"
#include "Defines/Network/MergeVisualDissolveData.h"
#include "NormalModeGameState.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	None,
	S1_Start,
	S2_Preparation_Turn1_Blue,
	S3_Preparation_Turn1_Red,
	S4_Preparation_Turn2_Red,
	S5_Preparation_Turn2_Blue,
	S6_Preparation_Turn3_Blue,
	S7_Preparation_Turn3_Red,
	S8_Merge,
	S9_Battle,
	S9_Ended
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
	EGamePhase CurrentGamePhase;

	FTimerHandle RoundTimerHandle;
	FTimerHandle PreparationTimerHandle;

	// For Team, Players
	TMap<EActorTeam, TArray<ABaseUnit*>> TeamToUnitMap;

	UFUNCTION()
	void OnRep_CurrentPhaseState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnGamePhaseChanged();

public:
	void StartLoadLevelSequence();
	UFUNCTION()
	void OnLevelDevMapLoaded();
	UFUNCTION()
	void OnIngameMapDetailLoaded();
	void LoadLevel(const FString& LevelName, bool ShouldBlockOnLoad);
	void UnloadLevel(const FString& LevelName);
	EGamePhase GetCurrentGamePhase();
	void SetCurrentGamePhase(EGamePhase GamePhase);
	// Function to trigger level loaded logic
	UFUNCTION(BlueprintCallable)
	void ServerOnLevelLoaded();
	// Multicast delegate to notify clients
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnLevelLoaded();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnMergedPhase();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastVisualMergeDelete(const TArray<FMergeVisualDissolveData>& MergeDataList);

	// Process submitted units from Client
	void ProcessPendingUnits(EActorTeam Team, const TArray<FPendingUnitData>& PendingUnits);

	// Set the flag to control the AI behaviors
	void UpdateAllAIBlackboardKeys(bool bCanExecute);

	TArray<ABaseUnit*> GetUnitsInTeam(EActorTeam Team);

	UFUNCTION()
	void StartBattle();

	void PossessUnitsInTeam(EActorTeam Team);

	// Process the new game phase
	void ClientHandleGamePhaseChanged();

	// Change to the next game phase on server
	void ChangeToNextGamePhase();
	EGamePhase GetNextGamePhase();

	void HandleMergeLogic();
};

