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
	float CurrentHealth = OwnerUnit->GetCurrentHealth();
	float MaxHealth = OwnerUnit->GetMaxHealth();
	if (MaxHealth > 0)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
		// todo temp
		ManaBar->SetPercent(CurrentHealth / MaxHealth);
	}

	// get current attributes
	const UUnitAttributeSet* Attributes = OwnerUnit->GetAttributes();
	
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	CurrentHealthLabel->SetText(FText::AsNumber(Attributes->GetHealth(), &Opts));
	MaxHealthLabel->SetText(FText::AsNumber(Attributes->GetMaxHealth(), &Opts));

	CurrentManaLabel->SetText(FText::AsNumber(Attributes->GetMana(), &Opts));
	MaxManaLabel->SetText(FText::AsNumber(Attributes->GetMaxMana(), &Opts));
}
