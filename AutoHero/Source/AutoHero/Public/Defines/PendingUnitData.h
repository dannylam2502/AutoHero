#pragma once
#include "CoreMinimal.h"
#include "Actors/BaseUnit.h"
#include "PendingUnitData.generated.h"
USTRUCT(BlueprintType)
struct FPendingUnitData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int UnitType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D GridPosition;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector UnitLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double PlacementTime;

	// Don't use this property on Server, Client only
	UPROPERTY(BlueprintReadWrite)
	ABaseUnit* BaseUnit;

	FPendingUnitData()
	{
		UnitType = 0;
		GridPosition = FVector2D::ZeroVector;
		UnitLocation = FVector::Zero();
		PlacementTime = 0.0;
		BaseUnit = nullptr;
	}
};