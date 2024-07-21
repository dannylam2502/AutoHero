#pragma once

#include "CoreMinimal.h"
#include "UnitState.generated.h"

UENUM(BlueprintType)
enum class EUnitState : uint8
{
	Default UMETA(DisplayName = "Default"),
	DraggingFromWidget UMETA(DisplayName = "DraggingFromWidget"),
	DraggingInField UMETA(DisplayName = "DraggingInField"),
	WaitingForPlacement UMETA(DisplayName = "Waiting For Placement"),
	WaitingForBattle UMETA(DisplayName = "Waiting For Battle"),
	InBattle UMETA(DisplayName = "In Battle"),
	Dead UMETA(DisplayName = "Dead"),
	// Add more states as needed
};