// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NormalGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelLoaded);
/**
 * 
 */
UCLASS()
class AUTOHERO_API UNormalGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLevelLoaded OnLevelLoaded;

    // Function to trigger the delegate
    void TriggerLevelLoaded();
	
	
};
