// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EnumPlace/CppEChatSystemChannels.h"
#include "GameFramework/PlayerController.h"
#include "CppSChannelListeners.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppSChannelListeners : public UObject
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FSChannelListeners
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) eChatSystemChannels channelType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<APlayerController*> arrayPlayerController;

};