// Fill out your copyright notice in the Description page of Project Settings.


#include "CppGameState.h"
#include "GameFramework/PlayerController.h"

void ACppGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

void ACppGameState::SendChatMessage(FSChatMessageInfo message)
{
	if (HasAuthority())
	{
		AddChannelMessage(message);
	}
}

void ACppGameState::GetChatChannelMessage(eChatSystemChannels channelType, bool& isChannelFound, FSChatMessageInfo& message)
{

}

#pragma region Interface.
void ACppGameState::WatchChatChannel(eChatSystemChannels channelType, APlayerController* playerController)
{
	if (HasAuthority())
	{
		AddChannelListener(channelType, playerController);
	}
}

void ACppGameState::AddChannelMessage(FSChatMessageInfo message)
{
	bool isChannelFound = false;
	FSChannel messageChannel;
	int index = -1;
	FindMessageChannel(message.channelType, isChannelFound, messageChannel, index);

	if (isChannelFound)
	{
		messageChannel.arrayMessageInfo.Add(message);
		arraychannelMessage.Add(messageChannel);
	}
	else
	{
		messageChannel.channelType = message.channelType;
		messageChannel.arrayMessageInfo.Add(message);
		arraychannelMessage.Add(messageChannel);
	}

	NotifyChannelUpdated(message.channelType);
}

void ACppGameState::FindMessageChannel(eChatSystemChannels channelType, bool& isChannelFound, FSChannel& messageChannel, int& index)
{
	isMessageChannelCreated(channelType, messageChannel, index);
}
#pragma endregion

bool ACppGameState::isMessageChannelCreated(eChatSystemChannels channelType, FSChannel& messageChannel, int& index)
{
	for (int i = 0; i < arraychannelMessage.Max(); i++)
	{
		FSChannel channel = arraychannelMessage[i];
		if (channel.channelType == channelType)
		{
			index = i;
			messageChannel = channel;
			return true;
		}
	}

	index = -1;
	return false;
}

void ACppGameState::NotifyChannelUpdated(eChatSystemChannels channelType)
{

}

void ACppGameState::AddChannelListener(eChatSystemChannels channelType, APlayerController* playerContronller)
{

}

bool ACppGameState::isChannelListenerCreated(eChatSystemChannels channelType, int& index, eChatSystemChannels listener)
{
	return false;
}