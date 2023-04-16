// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "CppRegisterMenu.generated.h"

class UButton;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppRegisterMenu : public UCppBaseMenu
{
	GENERATED_BODY()
	
public:
	virtual void Setup() override;
	virtual void Init() override;
	virtual void Pop() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnBack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnRegister;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UEditableTextBox* inputUserEmail;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UEditableTextBox* inputUserName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UEditableTextBox* inputPassword;

private:
	UFUNCTION() void OnBackClicked();
	UFUNCTION() void OnRegisterClicked();

	UFUNCTION() void OnUserEmailInput(const FText& text);
	UFUNCTION() void OnUserNameInput(const FText& text);
	UFUNCTION() void OnPasswordInput(const FText& text);

public:
	void OnRegisterSuccess();
	void OnRegisterError();

};
