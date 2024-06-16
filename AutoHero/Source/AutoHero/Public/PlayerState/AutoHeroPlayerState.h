// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AutoHeroPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API AAutoHeroPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Units")
	TArray<int32> SelectedUnitIDs;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Units")
	TArray<int32> CurrentUnitIDs;

	void SetSelectedUnitIDs(const TArray<int32>& UnitIDs);
	void SetCurrentUnitIDs(const TArray<int32>& UnitIDs);
};
