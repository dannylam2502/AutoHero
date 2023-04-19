// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EnumPlace/CppEChatSystemChannels.h"
#include "StructPlace/CppSChatMessageInfo.h"
#include "CppSChannel.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppSChannel : public UObject
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FSChannel
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) eChatSystemChannels channelType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FSChatMessageInfo messageInfo;
};
