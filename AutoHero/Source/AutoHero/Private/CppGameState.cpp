// Fill out your copyright notice in the Description page of Project Settings.


#include "CppGameState.h"
#include "GameFramework/PlayerController.h"
#include "AutoHero/AutoHeroPlayerController.h"

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
	if (HasAuthority())
	{
		FSChannel channelMessage;
		int index = -1;
		isChannelFound = IsMessageChannelCreated(channelType, channelMessage, index);
		if (isChannelFound)
		{
			arrayMessage = channelMessage.arrayMessageInfo;
		}
		else
		{
			arrayMessage.SetNum(0);	
		}

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
	if (HasAuthority())
	{
		eChatSystemChannels channelType = message.channelType;
		bool isChannelFound = false;
		FSChannel messageChannel;
		int index = -1;
		FindMessageChannel(channelType, isChannelFound, messageChannel, index);

		if (isChannelFound)
		{
			// Add message to the existing channel
			messageChannel.arrayMessageInfo.Add(message);
			arraychannelMessage[index] = messageChannel;
		}
		else
		{
			// Create message channel because it does not exist yet
			messageChannel.channelType = channelType;
			messageChannel.arrayMessageInfo.Add(message);
			arraychannelMessage.Add(messageChannel);
		}

		NotifyChannelUpdated(channelType);
	}
}

void ACppGameState::FindMessageChannel(eChatSystemChannels channelType, bool& isChannelFound, FSChannel& messageChannel, int& index)
{
	isChannelFound = IsMessageChannelCreated(channelType, messageChannel, index);
}

bool ACppGameState::IsMessageChannelCreated(eChatSystemChannels channelType, FSChannel& messageChannel, int& index)
{
	if (HasAuthority())
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
	}

	index = -1;
	return false;
}

void ACppGameState::NotifyChannelUpdated(eChatSystemChannels channelType)
{
	// Notify channel listeners that a new message arrived
	if (HasAuthority())
	{
		FSChannelListeners listener;
		int index = -1;
		if (IsChannelListenerCreated(channelType, listener, index))
		{
			bool isChannelFound = false;
			TArray<FSChatMessageInfo> arrayMessage;
			GetChatChannelMessages(channelType, isChannelFound, arrayMessage);

			for (int i = 0; i < listener.arrayPlayerController.Num(); i++)
			{
				APlayerController* playerController = listener.arrayPlayerController[i];
				AAutoHeroPlayerController* autoHeroPlayerController = dynamic_cast<AAutoHeroPlayerController*>(playerController);
				if (autoHeroPlayerController)
				{
					// Notify each channel listener with an updated list of messagees
					autoHeroPlayerController->OnChatChannelUpdated(channelType, arrayMessage);
				}
			}
		}
	}
}

void ACppGameState::AddChannelListener(eChatSystemChannels channelType, APlayerController* playerContronller)
{
	if (HasAuthority())
	{
		FSChannelListeners channelListener;
		int index = -1;
		if (IsChannelListenerCreated(channelType, channelListener, index))
		{
			// If channel listener struct exisits, add player to the player list
			TArray<APlayerController*> arrayNotify = channelListener.arrayPlayerController;
			int indexElement = arrayNotify.Find(playerContronller);
			if (indexElement <= -1)
			{
				arrayNotify.Add(playerContronller);
				channelListener.arrayPlayerController = arrayNotify;
				arrayChannelListener[index] = channelListener;
			}
		}
		else
		{
			// Channel listener dosen't exist, create it
			channelListener.channelType = channelType;
			channelListener.arrayPlayerController.SetNum(1);
			channelListener.arrayPlayerController[0] = playerContronller;
			arrayChannelListener.Add(channelListener);
		}
	}
}

bool ACppGameState::IsChannelListenerCreated(eChatSystemChannels channelType, FSChannelListeners& listener, int& index)
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

ICppIChatSystemInterface* ACppGameState::GetChatSystem()
{
	return this;
}