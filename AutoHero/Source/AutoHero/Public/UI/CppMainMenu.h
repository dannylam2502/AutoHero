// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CppMainMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnSetting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnPvp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnPve;

private:
	UFUNCTION() void OnSettingClicked();
	UFUNCTION() void OnPvpClicked();
	UFUNCTION() void OnPveClicked();

protected:
	virtual void NativeConstruct() override;
};
