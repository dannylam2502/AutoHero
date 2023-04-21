// Fill out your copyright notice in the Description page of Project Settings.


#include "CppGameState.h"
#include "GameFramework/PlayerController.h"
#include "AutoHero/AutoHeroPlayerController.h"

void ACppGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

#pragma region Interface.
void ACppGameState::SendChatMessage(FSChatMessageInfo message)
{
	if (HasAuthority())
	{
		AddChannelMessage(message);
	}
}

void ACppGameState::GetChatChannelMessages(eChatSystemChannels channelType, bool& isChannelFound, TArray<FSChatMessageInfo>& arrayMessage)
{
	FSChannel channelMessage;
	int index = -1;

	if (HasAuthority())
	{
		isChannelFound = isMessageChannelCreated(channelType, channelMessage, index);
		if (!isChannelFound)
		{
			arrayMessage.SetNum(0);	
		}

		arrayMessage = channelMessage.arrayMessageInfo;
	}
}

void ACppGameState::WatchChatChannel(eChatSystemChannels channelType, APlayerController* playerController)
{
	if (HasAuthority())
	{
		AddChannelListener(channelType, playerController);
	}
}
#pragma endregion

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

bool ACppGameState::isMessageChannelCreated(eChatSystemChannels channelType, FSChannel& messageChannel, int& index)
{
	for (int i = 0; i < arraychannelMessage.Num(); i++)
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
	if (HasAuthority())
	{
		FSChannelListeners listener;
		int index = -1;
		if (isChannelListenerCreated(channelType, listener, index))
		{
			bool isChannelFound = false;
			TArray<FSChatMessageInfo> arrayMessage;
			GetChatChannelMessages(channelType, isChannelFound, arrayMessage);

			for (APlayerController* playerController : listener.arrayPlayerController)
			{
				AAutoHeroPlayerController* autoHeroPlayerController = dynamic_cast<AAutoHeroPlayerController*>(playerController);
				if (autoHeroPlayerController)
				{
					autoHeroPlayerController->OnChatChannelUpdated(channelType, arrayMessage);
				}
			}
		}
	}
}

void ACppGameState::AddChannelListener(eChatSystemChannels channelType, APlayerController* playerContronller)
{
	FSChannelListeners channelListener;
	int index = -1;
	if (isChannelListenerCreated(channelType, channelListener, index))
	{
		TArray<APlayerController*> arrayNotify = channelListener.arrayPlayerController;
		int _index = arrayNotify.Find(playerContronller);
		if (_index <= -1)
		{
			arrayNotify.Add(playerContronller);
			channelListener.arrayPlayerController = arrayNotify;
			arrayChannelListener[index] = channelListener;
		}
	}
	else
	{
		channelListener.channelType = channelType;
		channelListener.arrayPlayerController.SetNum(1);
		channelListener.arrayPlayerController[0] = playerContronller;
		arrayChannelListener.Add(channelListener);
	}
}

bool ACppGameState::isChannelListenerCreated(eChatSystemChannels channelType, FSChannelListeners listener, int& index)
{
	for (int i = 0; i < arrayChannelListener.Num(); i++)
	{
		FSChannelListeners _listener = arrayChannelListener[i];
		if (_listener.channelType == channelType)
		{
			index = i;
			listener = _listener;
			return true;
		}
	}

	index = -1;
	return false;
}