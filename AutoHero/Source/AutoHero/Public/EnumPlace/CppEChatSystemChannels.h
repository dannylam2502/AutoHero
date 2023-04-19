// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CppEChatSystemChannels.generated.h"

/**
 *
 */
UCLASS()
class AUTOHERO_API UCppEChatSystemChannels : public UObject
{
    GENERATED_BODY()
};

UENUM(BlueprintType)
enum class eChatSystemChannels : uint8
{
    Global UMETA(DisplayName = "Global"),
    Trade UMETA(DisplayName = "Trade"),
    Local UMETA(DisplayName = "Local")
};