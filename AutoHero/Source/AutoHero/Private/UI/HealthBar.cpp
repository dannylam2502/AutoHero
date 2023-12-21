// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBar.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthBar::SetOwnerUnit(ABaseUnit* InUnit)
{
	OwnerUnit = InUnit;
}

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!OwnerUnit.IsValid())
	{
		return;
	}
	float CurrentHealth = OwnerUnit->GetCurrentHealth();
	float MaxHealth = OwnerUnit->GetMaxHealth();
	if (MaxHealth > 0)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	CurrentHealthLabel->SetText(FText::AsNumber(OwnerUnit->GetCurrentHealth(), &Opts));
	MaxHealthLabel->SetText(FText::AsNumber(OwnerUnit->GetMaxHealth(), &Opts));
}
