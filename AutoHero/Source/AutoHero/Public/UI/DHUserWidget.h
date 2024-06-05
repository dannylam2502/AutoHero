// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DHUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UDHUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Doing setup in the C++ constructor is not as
	// useful as using NativeConstruct.
	virtual void NativeConstruct() override;
};
