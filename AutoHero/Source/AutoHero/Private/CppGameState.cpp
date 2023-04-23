// Fill out your copyright notice in the Description page of Project Settings.


#include "CppGameState.h"
#include "GameFramework/PlayerController.h"
#include "AutoHero/AutoHeroPlayerController.h"

//ACppGameState* ACppGameState::i;
//ACppGameState* ACppGameState::I()
//{
//	return i;
//}

void ACppGameState::BeginPlay()
{
	//i = this;

	Super::BeginPlay();
}

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
		isChannelFound = IsMessageChannelCreated(channelType, channelMessage, index);
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
	IsMessageChannelCreated(channelType, messageChannel, index);
}

bool ACppGameState::IsMessageChannelCreated(eChatSystemChannels channelType, FSChannel& messageChannel, int& index)
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
	if (IsChannelListenerCreated(channelType, channelListener, index))
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