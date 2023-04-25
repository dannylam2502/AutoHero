// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"

#include "EnumPlace/CppEChatSystemChannels.h"
#include "StructPlace/CppSChatMessageInfo.h"

#include "CppChatBoxPopup.generated.h"

class UButton;
class UVerticalBox;
class UMultiLineEditableTextBox;

class AAutoHeroCharacter;

class UCppChatBoxMessagePopup;

DECLARE_DELEGATE_OneParam(OnSendMessageCallback, FSChatMessageInfo);
DECLARE_DELEGATE_OneParam(OnChannelChangedCallback, eChatSystemChannels);

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppChatBoxPopup : public UCppBaseMenu
{
	GENERATED_BODY()
	
public:
	virtual void Setup() override;
	virtual void Init() override;
	virtual void Pop() override;
	virtual void OnExitGame() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnChannelGlobal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnChannelTrade;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnChannelLocal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnSendMessage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UVerticalBox* VchannelMessages;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UMultiLineEditableTextBox* textMessageToSend;

public:
	UPROPERTY(EditAnywhere) TSubclassOf<class AAutoHeroCharacter> playerClass;
	AAutoHeroCharacter* player;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		eChatSystemChannels currentChannelType;

private:
	UFUNCTION() void OnChannelGlobalButtonClicked();
	UFUNCTION() void OnChannelTradeButtonClicked();
	UFUNCTION() void OnChannelLocalButtonClicked();
	UFUNCTION() void OnbtnSendMessageButtonClicked();


public:
	void CallSendMessage(FSChatMessageInfo chatInfo);

	void ClearEnteredMessages();
	void ClearChannelMessages();
	void SetChannelMessages(eChatSystemChannels channelType, TArray<FSChatMessageInfo> arrayMessage);

private:
	FGuid MakeMessageId();

public:
	OnSendMessageCallback onSendMessageCallback;
	OnChannelChangedCallback onChannelChangedCallback;

private:
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppChatBoxMessagePopup> chatBoxMessagePopupClass;
public:
	TArray<UCppChatBoxMessagePopup*> arrayChatBoxMessagePopup;
	UCppChatBoxMessagePopup* CreateBoxMessage(FSChatMessageInfo chatInfo);

};
