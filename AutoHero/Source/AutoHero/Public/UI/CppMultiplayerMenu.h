// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "CppMultiplayerMenu.generated.h"

class UButton;
class UEditableTextBox;
class UCheckBox;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppMultiplayerMenu : public UCppBaseMenu
{
	GENERATED_BODY()
	
public:
	virtual void Setup() override;
	virtual void Init() override;
	virtual void Pop() override;
	virtual void OnExitGame() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* createSession;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UEditableTextBox* inputMaxPlayer;
	int maxPlayerValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UCheckBox* checkBoxLAN;
	bool isHostLAN;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* findSession;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* joinSession;

private:
	UFUNCTION() void OnCreateSessionClicked();
	UFUNCTION() void OnFindSessionClicked();
	UFUNCTION() void OnJoinSessionClicked();

	UFUNCTION() void OnMaxPlayerInput(const FText& text);

	UFUNCTION() void OnCheckBoxLAN(bool bIsChecked);

};
