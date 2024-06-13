// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FUnitData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FUnitData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Data")
	int32 UnitID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Data")
	FString UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Data")
	UTexture2D* UnitIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Data")
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Data")
	TSubclassOf<UAnimInstance> AnimInstance;

	// Add other properties as needed
};
