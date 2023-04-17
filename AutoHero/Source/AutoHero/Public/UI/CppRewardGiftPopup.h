// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "CppRewardGiftPopup.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppRewardGiftPopup : public UCppBaseMenu
{
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Init() override;
	virtual void Pop() override;
	virtual void OnExitGame() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnClaim;

private:
	UFUNCTION() void OnClaimClicked();
	
};
