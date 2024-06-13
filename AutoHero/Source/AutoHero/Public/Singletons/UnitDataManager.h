// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UnitDataManager.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UUnitDataManager : public UObject
{
	GENERATED_BODY()
	
public:
	static UUnitDataManager* Get();

	void Initialize();
	//FUnit 
	
	
};
