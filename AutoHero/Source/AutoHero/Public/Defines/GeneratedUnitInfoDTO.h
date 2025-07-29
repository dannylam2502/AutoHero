#pragma once

#include "CoreMinimal.h"
#include "GeneratedUnitInfoDTO.generated.h"

USTRUCT(BlueprintType)
struct FGeneratedUnitInfoDTO
{
	GENERATED_BODY()

	UPROPERTY()
	int32 UnitType;

	UPROPERTY()
	bool bIsUnlocked;

	UPROPERTY()
	int32 Level;
};