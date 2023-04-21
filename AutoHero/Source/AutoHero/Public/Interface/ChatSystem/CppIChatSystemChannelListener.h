// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnumPlace/CppEChatSystemChannels.h"
#include "StructPlace/CppSChatMessageInfo.h"
#include "CppIChatSystemChannelListener.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UCppIChatSystemChannelListener : public UInterface
{
	GENERATED_BODY()
};

class ICppIChatSystemChannelListener
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = "Chat System")
		virtual void OnChatChannelUpdated(eChatSystemChannels channelType, TArray<FSChatMessageInfo> arrayMessage) = 0;

};
