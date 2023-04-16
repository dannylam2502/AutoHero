// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "CppChatBoxPopup.generated.h"

class UButton;
class UMultiLineEditableTextBox;

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
		UMultiLineEditableTextBox* textMessageToSend;

private:
	UFUNCTION() void OnChannelGlobalButtonClicked();
	UFUNCTION() void OnChannelTradeButtonClicked();
	UFUNCTION() void OnChannelLocalButtonClicked();
	UFUNCTION() void OnbtnSendMessageButtonClicked();

};
