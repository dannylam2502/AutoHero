// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "CppMainMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppMainMenu : public UCppBaseMenu
{
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Init(TArray<UObject*> initParams) override;
	virtual void Pop() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnQuit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnSetting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnPvp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnPve;

private:
	UFUNCTION() void OnQuitClicked();
	UFUNCTION() void OnSettingClicked();
	UFUNCTION() void OnPvpClicked();
	UFUNCTION() void OnPveClicked();
};
