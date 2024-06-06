// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NormalModeGameState.generated.h"

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

	UPROPERTY(ReplicatedUsing = OnRep_RoundState)
	int32 CurrentRound;

	UPROPERTY(ReplicatedUsing = OnRep_IsPreparationPhase)
	bool bIsPreparationPhase;

	FTimerHandle RoundTimerHandle;
	FTimerHandle PreparationTimerHandle;

	UFUNCTION()
	void OnRep_RoundState();

	UFUNCTION()
	void OnRep_IsPreparationPhase();

	void UpdateAllAIBlackboardKeys(bool bCanExecute);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	void StartRound();
	void EndRound();
	void StartPreparation();
	void EndPreparation();
	void LoadLevel(const FString& LevelName);
	void UnloadLevel(const FString& LevelName);

	bool IsPreparationPhase();
	int32 GetCurrentRound();
};
