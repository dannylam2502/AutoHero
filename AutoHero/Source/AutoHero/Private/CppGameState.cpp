// Fill out your copyright notice in the Description page of Project Settings.


#include "CppGameState.h"

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

void ACppGameState::WatchChatChannel(eChatSystemChannels channelType, APlayerController* playerController)
{
	if (HasAuthority())
	{

	}
}

void ACppGameState::AddChannelMessage(FSChatMessageInfo message)
{
	if (HasAuthority())
	{
		
	}
}