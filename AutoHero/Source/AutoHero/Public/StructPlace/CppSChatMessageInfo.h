// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumPlace/CppEChatSystemChannels.h"
#include "CppSChatMessageInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSChatMessageInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid messageId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) eChatSystemChannels channelType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FDateTime messageTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText message;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int authorPlayerId;

};
