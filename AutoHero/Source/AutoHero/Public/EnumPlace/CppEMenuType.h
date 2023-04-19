// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CppEMenuType.generated.h"

///**
// *
// */
UCLASS()
class AUTOHERO_API UCppEMenuType : public UObject
{
    GENERATED_BODY()
};

UENUM(BlueprintType)
enum class eMenuType : uint8
{
    Menu UMETA(DisplayName = "Menu"),
    Popup UMETA(DisplayName = "Popup"),
    AlwayOnTop UMETA(DisplayName = "AlwayOnTop")
};