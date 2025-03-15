#pragma once

USTRUCT(BlueprintType)
struct FPendingUnitData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int UnitID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D GridPosition;

	FPendingUnitData()
	{
		UnitID = 0;
		GridPosition = FVector2D::ZeroVector;
	}
};