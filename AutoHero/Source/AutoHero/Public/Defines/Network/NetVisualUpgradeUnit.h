#pragma once

#include "CoreMinimal.h"
#include "NetVisualUpgradeUnit.generated.h"

USTRUCT(BlueprintType)
struct FNetVisualUpgradeUnit
{
	GENERATED_BODY()

	UPROPERTY()
	FVector2D GridPosition;

	UPROPERTY()
	int32 ToUnitID;

	UPROPERTY()
	TArray<int32> FromUnitIDs;
};
