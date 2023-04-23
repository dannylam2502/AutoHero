// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "StructPlace/CppSChatMessageInfo.h"
#include "CppChatBoxMessagePopup.generated.h"

class UTextBlock;
class UMultiLineEditableTextBox;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppChatBoxMessagePopup : public UCppBaseMenu
{
	GENERATED_BODY()
	
public:
	virtual void Setup() override;
	virtual void Init() override;
	virtual void Pop() override;
	virtual void OnExitGame() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UTextBlock* textMessageTimeNode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UTextBlock* textAuthorNode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UMultiLineEditableTextBox* textMessage;

public:
	void SetMessage(FSChatMessageInfo chatInfo);

};
