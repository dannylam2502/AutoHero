// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppChatBoxMessagePopup.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"

void UCppChatBoxMessagePopup::Setup()
{
	UCppBaseMenu::Setup();
}

void UCppChatBoxMessagePopup::Init()
{
	UCppBaseMenu::Init();
}

void UCppChatBoxMessagePopup::Pop()
{
	UCppBaseMenu::Pop();
}

void UCppChatBoxMessagePopup::OnExitGame()
{
	UCppBaseMenu::OnExitGame();

	textMessageTimeNode = nullptr;
	textAuthorNode = nullptr;
	textMessage = nullptr;
}

void UCppChatBoxMessagePopup::SetMessage(FSChatMessageInfo* chatInfo)
{
	textMessageTimeNode->SetText(FText::AsDateTime(chatInfo->messageTime));
	textMessage->SetText(chatInfo->message);
	textAuthorNode->SetText(FText::AsNumber(chatInfo->authorPlayerId));
}