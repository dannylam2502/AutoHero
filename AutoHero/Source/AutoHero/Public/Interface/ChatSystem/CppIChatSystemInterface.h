// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/Actor.h"
#include "EnumPlace/CppEChatSystemChannels.h"
#include "StructPlace/CppSChatMessageInfo.h"
#include "CppIChatSystemInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UCppIChatSystemInterface : public UInterface
{
	GENERATED_BODY()
};

class ICppIChatSystemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = "Chat System")
		virtual void SendChatMessage(FSChatMessageInfo message) = 0;
	UFUNCTION(Category = "Chat System")
		virtual void GetChatChannelMessages(eChatSystemChannels channelType, bool& isChannelFound, TArray<FSChatMessageInfo>& arrayMessage) = 0;
	UFUNCTION(Category = "Chat System")
		virtual void WatchChatChannel(eChatSystemChannels channelType, APlayerController* playerController) = 0;
};
