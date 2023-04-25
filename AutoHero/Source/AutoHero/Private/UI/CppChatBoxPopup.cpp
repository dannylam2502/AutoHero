// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppChatBoxPopup.h"
#include "UI/CppChatBoxMessagePopup.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/MultiLineEditableTextBox.h"

#include "AutoHero/AutoHeroCharacter.h"
#include "GameFramework/PlayerState.h"

void UCppChatBoxPopup::Setup()
{
	UCppBaseMenu::Setup();

	menuType = eMenuType::Popup;

	currentChannelType = eChatSystemChannels::Global;

	btnChannelGlobal->OnClicked.AddDynamic(this, &UCppChatBoxPopup::OnChannelGlobalButtonClicked);
	btnChannelTrade->OnClicked.AddDynamic(this, &UCppChatBoxPopup::OnChannelTradeButtonClicked);
	btnChannelLocal->OnClicked.AddDynamic(this, &UCppChatBoxPopup::OnChannelLocalButtonClicked);
	btnSendMessage->OnClicked.AddDynamic(this, &UCppChatBoxPopup::OnbtnSendMessageButtonClicked);

	player = playerClass.GetDefaultObject();
}

void UCppChatBoxPopup::Init()
{
	UCppBaseMenu::Init();
}

void UCppChatBoxPopup::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppChatBoxPopup::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	chatBoxMessagePopupClass = nullptr;
	for (UCppChatBoxMessagePopup* boxMessagePopup : arrayChatBoxMessagePopup)
	{
		if (boxMessagePopup)
		{
			boxMessagePopup->OnExitGame();
		}
	}
	arrayChatBoxMessagePopup.SetNum(0);

	btnChannelGlobal = nullptr;
	btnChannelTrade = nullptr;
	btnChannelLocal = nullptr;
	btnSendMessage = nullptr;
	VchannelMessages = nullptr;
	textMessageToSend = nullptr;
	playerClass = nullptr;
	player = nullptr;

	onChannelChangedCallback = nullptr;
	onSendMessageCallback = nullptr;
}

void UCppChatBoxPopup::OnChannelGlobalButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnChannelGlobalButtonClicked!"));

	currentChannelType = eChatSystemChannels::Global;
	ClearChannelMessages();
	if (onChannelChangedCallback.GetHandle().IsValid())
	{
		onChannelChangedCallback.Execute(currentChannelType);
	}
}

void UCppChatBoxPopup::OnChannelTradeButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnChannelTradeButtonClicked!"));

	currentChannelType = eChatSystemChannels::Trade;
	ClearChannelMessages();
	if (onChannelChangedCallback.GetHandle().IsValid())
	{
		onChannelChangedCallback.Execute(currentChannelType);
	}
}

void UCppChatBoxPopup::OnChannelLocalButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnChannelLocalButtonClicked!"));

	currentChannelType = eChatSystemChannels::Local;
	ClearChannelMessages();
	if (onChannelChangedCallback.GetHandle().IsValid())
	{
		onChannelChangedCallback.Execute(currentChannelType);
	}
}

void UCppChatBoxPopup::OnbtnSendMessageButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnbtnSendMessageButtonClicked!"));

	// Verify user entered message
	if (!FText::TrimPrecedingAndTrailing(textMessageToSend->GetText()).IsEmptyOrWhitespace())
	{
		// Make message
		FSChatMessageInfo chatInfo = FSChatMessageInfo();
		chatInfo.messageId = MakeMessageId();
		chatInfo.message = textMessageToSend->GetText();
		chatInfo.channelType = currentChannelType;
		chatInfo.messageTime = FDateTime::UtcNow();
		chatInfo.authorPlayerId = player->GetPlayerState()->PlayerId;

		// Dispatch message event
		CallSendMessage(chatInfo);
		ClearEnteredMessages();
	}
}

void UCppChatBoxPopup::CallSendMessage(FSChatMessageInfo chatInfo)
{
	textMessageToSend->SetText(chatInfo.message);
	if (onSendMessageCallback.GetHandle().IsValid())
	{
		onSendMessageCallback.Execute(chatInfo);
	}
}

void UCppChatBoxPopup::ClearEnteredMessages()
{
	FText emtyText;
	textMessageToSend->SetText(emtyText);
}

void UCppChatBoxPopup::ClearChannelMessages()
{
	VchannelMessages->ClearChildren();
}

void UCppChatBoxPopup::SetChannelMessages(eChatSystemChannels channelType, TArray<FSChatMessageInfo> arrayMessage)
{
	if (currentChannelType == channelType)
	{
		ClearChannelMessages();
		for (FSChatMessageInfo message : arrayMessage)
		{
			CreateBoxMessage(message);
		}
	}

}

FGuid UCppChatBoxPopup::MakeMessageId()
{
	return FGuid::NewGuid();
}

UCppChatBoxMessagePopup* UCppChatBoxPopup::CreateBoxMessage(FSChatMessageInfo chatInfo)
{
	UCppChatBoxMessagePopup* boxMessagePopup = dynamic_cast<UCppChatBoxMessagePopup*>(CreateWidget<UCppBaseMenu>(GetOwningPlayer(), chatBoxMessagePopupClass));
	check(boxMessagePopup);
	boxMessagePopup->Setup();
	boxMessagePopup->SetMessage(chatInfo);
	VchannelMessages->AddChildToVerticalBox(boxMessagePopup);
	arrayChatBoxMessagePopup.Add(boxMessagePopup);

	return boxMessagePopup;
}