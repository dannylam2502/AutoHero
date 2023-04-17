// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppChatBoxPopup.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/MultiLineEditableTextBox.h"

#include "AutoHero/AutoHeroCharacter.h"
#include "GameFramework/PlayerState.h"

void UCppChatBoxPopup::Setup()
{
	UCppBaseMenu::Setup();

	menuType = eMenuType::Popup;

	currentChannelType = eChatSystemChannels::Local;

	btnChannelGlobal->OnClicked.AddDynamic(this, &UCppChatBoxPopup::OnChannelGlobalButtonClicked);
	btnChannelTrade->OnClicked.AddDynamic(this, &UCppChatBoxPopup::OnChannelTradeButtonClicked);
	btnChannelLocal->OnClicked.AddDynamic(this, &UCppChatBoxPopup::OnChannelLocalButtonClicked);

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

void UCppChatBoxPopup::OnChannelGlobalButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnChannelGlobalButtonClicked!"));

	currentChannelType = eChatSystemChannels::Global;
	ClearChannelMessage();
}

void UCppChatBoxPopup::OnChannelTradeButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnChannelTradeButtonClicked!"));

	currentChannelType = eChatSystemChannels::Trade;
	ClearChannelMessage();
}

void UCppChatBoxPopup::OnChannelLocalButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnChannelLocalButtonClicked!"));

	currentChannelType = eChatSystemChannels::Local;
	ClearChannelMessage();
}

void UCppChatBoxPopup::OnbtnSendMessageButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnbtnSendMessageButtonClicked!"));

	if (FText::TrimPrecedingAndTrailing(textMessageToSend->GetText()).IsEmptyOrWhitespace())
	{
		FSChatMessageInfo* chatInfo = new FSChatMessageInfo();
		chatInfo->messageId = currentMessageId;
		chatInfo->message = textMessageToSend->GetText();
		chatInfo->channelType = currentChannelType;
		chatInfo->messageTime = FDateTime::UtcNow();
		chatInfo->authorPlayerId = player->GetPlayerState()->PlayerId;

		CallSendMessage(chatInfo);
	}
}

void UCppChatBoxPopup::CallSendMessage(FSChatMessageInfo* chatInfo)
{
	textMessageToSend->SetText(chatInfo->message);
	ClearEnteredMessage();
}

void UCppChatBoxPopup::ClearEnteredMessage()
{
	FText emtyText;
	textMessageToSend->SetText(emtyText);
}

void UCppChatBoxPopup::ClearChannelMessage()
{
	channelMessage->ClearChildren();
}

void UCppChatBoxPopup::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	btnChannelGlobal = nullptr;
	btnChannelTrade = nullptr;
	btnChannelLocal = nullptr;
	btnSendMessage = nullptr;
	channelMessage = nullptr;
	textMessageToSend = nullptr;
	playerClass = nullptr;
	player = nullptr;

}