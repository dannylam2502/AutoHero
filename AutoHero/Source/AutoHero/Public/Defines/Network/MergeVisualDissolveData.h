#pragma once

#include "CoreMinimal.h"
#include "MergeVisualDissolveData.generated.h"

USTRUCT(BlueprintType)
struct FMergeVisualDissolveData
{
	GENERATED_BODY()

	// List of units that should dissolve
	UPROPERTY()
	TArray<int32> FromUnitInstanceIDs;

	UPROPERTY()
	EActorTeam Team;

	UPROPERTY()
	int32 UpgradeUnitType;

	// Location where the new merged unit will appear
	UPROPERTY()
	FVector TargetLocation;

	// Optional: GridPosition (for aligning visuals if needed)
	UPROPERTY()
	FVector2D TargetGridPosition;

	// Optional: Delay before effect plays (if needed)
	UPROPERTY()
	float DissolveDelay = 0.f;

	// Optional: Style or type of dissolve (if multiple effects exist)
	UPROPERTY()
	FName EffectTag;
};

