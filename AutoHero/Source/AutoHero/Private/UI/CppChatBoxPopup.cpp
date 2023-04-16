// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppChatBoxPopup.h"
#include "Components/Button.h"
#include "Components/MultiLineEditableTextBox.h"

void UCppChatBoxPopup::Setup()
{
	UCppBaseMenu::Setup();

	menuType = eMenuType::Popup;

	btnChannelGlobal->OnClicked.AddDynamic(this, &UCppChatBoxPopup::OnChannelGlobalButtonClicked);
	btnChannelTrade->OnClicked.AddDynamic(this, &UCppChatBoxPopup::OnChannelTradeButtonClicked);
	btnChannelLocal->OnClicked.AddDynamic(this, &UCppChatBoxPopup::OnChannelLocalButtonClicked);
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
}

void UCppChatBoxPopup::OnChannelTradeButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnChannelTradeButtonClicked!"));
}

void UCppChatBoxPopup::OnChannelLocalButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnChannelLocalButtonClicked!"));
}

void UCppChatBoxPopup::OnbtnSendMessageButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("OnbtnSendMessageButtonClicked!"));

	if (FText::TrimPrecedingAndTrailing(textMessageToSend->GetText()).IsEmptyOrWhitespace())
	{

	}
}