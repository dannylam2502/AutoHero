// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

///**
// *
// */
UENUM(BlueprintType)
enum class eMenuType : uint8
{
    Menu UMETA(DisplayName = "Menu"),
    Popup UMETA(DisplayName = "Popup"),
    AlwayOnTop UMETA(DisplayName = "AlwayOnTop")
};