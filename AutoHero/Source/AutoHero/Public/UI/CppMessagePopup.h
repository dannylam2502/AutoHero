// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "CppMessagePopup.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppMessagePopup : public UCppBaseMenu
{
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Init() override;
	virtual void Pop() override;
	virtual void OnExitGame() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UTextBlock* textMessage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnClose;

public:
	void SetTextMessage(FString content);

private:
	UFUNCTION() void OnCloseClicked();

};
