#pragma once

#include "CoreMinimal.h"
#include "NetDataStructs.generated.h"

// 🔄 Stores basic information about a unit (Can be used by both client & server)
USTRUCT(BlueprintType)
struct FNetData_UnitInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 UnitID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D GridPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString UnitName;

	FNetData_UnitInfo()
	{
		UnitID = -1;
		GridPosition = FVector2D::ZeroVector;
		UnitName = "Unknown";
	}
};