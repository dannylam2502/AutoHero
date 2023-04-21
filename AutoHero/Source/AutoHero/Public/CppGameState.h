// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Interface/ChatSystem/CppIChatSystemInterface.h"
#include "EnumPlace/CppEChatSystemChannels.h"
#include "StructPlace/CppSChannel.h"
#include "StructPlace/CppSChannelListeners.h"
#include "CppGameState.generated.h"

class APlayerController;

/**
 * 
 */
UCLASS()
class AUTOHERO_API ACppGameState : public AGameStateBase, public ICppIChatSystemInterface
{
	GENERATED_BODY()

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	TArray<FSChannel> arraychannelMessage;
	TArray<FSChannelListeners> arrayChannelListener;

#pragma region Interface
public:
	virtual void SendChatMessage(FSChatMessageInfo message) override;
	virtual void GetChatChannelMessages(eChatSystemChannels channelType, bool& isChannelFound, TArray<FSChatMessageInfo>& arrayMessage) override;
	virtual void WatchChatChannel(eChatSystemChannels channelType, APlayerController* playerController) override;
#pragma endregion

private:
	void AddChannelMessage(FSChatMessageInfo message);
	void FindMessageChannel(eChatSystemChannels channelType, bool& isChannelFound, FSChannel& messageChannel, int& index);
	bool isMessageChannelCreated(eChatSystemChannels channelType, FSChannel& messageChannel, int& index);
	void NotifyChannelUpdated(eChatSystemChannels channelType);
	void AddChannelListener(eChatSystemChannels channelType, APlayerController* playerContronller);
	bool isChannelListenerCreated(eChatSystemChannels channelType, FSChannelListeners listener, int&index);

};
