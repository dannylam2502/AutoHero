// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Interface/ChatSystem/CppIChatSystemInterface.h"
#include "StructPlace/CppSChannel.h"
#include "CppGameState.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API ACppGameState : public AGameStateBase, public ICppIChatSystemInterface
{
	GENERATED_BODY()

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FSChannel channelMessage;

public:
	virtual void SendChatMessage(FSChatMessageInfo message) override;
	virtual void GetChatChannelMessage(eChatSystemChannels channelType, bool& isChannelFound, FSChatMessageInfo& message) override;
	virtual void WatchChatChannel(eChatSystemChannels channelType, APlayerController* playerController) override;

private:
	void AddChannelMessage(FSChatMessageInfo message);

};
