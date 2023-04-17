// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "CppPVPMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppPVPMenu : public UCppBaseMenu
{
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Init() override;
	virtual void Pop() override;
	virtual void OnExitGame() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnBack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnFindMatch;

private:
	UFUNCTION() void OnBackClicked();
	UFUNCTION() void OnFindMatchClicked();

};
