// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "CppLoginMenu.generated.h"

class UButton;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppLoginMenu : public UCppBaseMenu
{
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Init() override;
	virtual void Pop() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnQuit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UEditableTextBox* inputUserName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UEditableTextBox* inputPassword;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnLogin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnSignUp;

private:
	UFUNCTION() void OnQuitClicked();
	UFUNCTION() void OnLoginClicked();
	UFUNCTION() void OnRegisterClicked();

	UFUNCTION() void OnUserNameInput(const FText& text);
	UFUNCTION() void OnPasswordInput(const FText& text);

public:
	void OnLoginSuccess();
	void OnLoginError();

};
