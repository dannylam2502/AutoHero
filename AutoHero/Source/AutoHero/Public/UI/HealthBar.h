// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Actors/BaseUnit.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AUTOHERO_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwnerUnit(ABaseUnit* InUnit);

protected:
	TWeakObjectPtr<ABaseUnit> OwnerUnit;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Health
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CurrentHealthLabel;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* MaxHealthLabel;

	// Mana
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* ManaBar;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CurrentManaLabel;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* MaxManaLabel;
};
