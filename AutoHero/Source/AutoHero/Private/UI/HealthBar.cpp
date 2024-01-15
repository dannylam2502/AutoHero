// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Core/Gameplay/UnitAttributeSet.h"

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
	// get current attributes
	const UUnitAttributeSet* Attributes = OwnerUnit->GetAttributes();
	
	const float CurrentHealth = Attributes->GetHealth();
	const float MaxHealth = Attributes->GetMaxHealth();
	const float CurrentMana = Attributes->GetMana();
	const float MaxMana = Attributes->GetMaxMana();
	if (MaxHealth > 0)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
	if (MaxMana > 0)
	{
		ManaBar->SetPercent(CurrentMana / MaxMana);
	}
	
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	CurrentHealthLabel->SetText(FText::AsNumber(CurrentHealth, &Opts));
	MaxHealthLabel->SetText(FText::AsNumber(MaxHealth, &Opts));

	CurrentManaLabel->SetText(FText::AsNumber(CurrentMana, &Opts));
	MaxManaLabel->SetText(FText::AsNumber(MaxMana, &Opts));
}
