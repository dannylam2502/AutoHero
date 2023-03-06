// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CppMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BlindWidget)) class UTextBlock* textNumber;
	UPROPERTY(meta = (BlindWidget)) class UButton* btnGenerate;
	
	void NativeConstruct() override;

	UFUNCTION() void OnPlayClicked();
	void GenerateRandom();
};
