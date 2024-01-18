// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Gameplay/UnitAttributeSet.h"
#include "GameplayEffectExtension.h"

#include "Net/UnrealNetwork.h"

UUnitAttributeSet::UUnitAttributeSet()
{
}

void UUnitAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitAttributeSet, AttackDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
}

// float UUnitAttributeSet::GetHealth() const
// {
// 	return FMath::Max(Health.GetCurrentValue(), 0.0f);
// }
//
// void UUnitAttributeSet::SetHealth(float NewVal) const
// {
// 	// Do not accept values lower than zero.
// 	NewVal = FMath::Max(NewVal, 0.0f);
// 	NewVal = FMath::Min(NewVal, MaxHealth.GetCurrentValue());
//
// 	// Make sure we have an Ability System Component instance. This should always be the case.
// 	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
// 	if (ensure(ASC))
// 	{
// 		// Set the base value (not the current value) through the appropriate function.
// 		// This makes sure that any modifiers we have applied will still work properly.
// 		ASC->SetNumericAttributeBase(GetHealthAttribute(), NewVal);
// 	}
// }

void UUnitAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, Health, OldHealth);
}

void UUnitAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, MaxHealth, OldMaxHealth);
}

// float UUnitAttributeSet::GetMana() const
// {
// 	return FMath::Max(Mana.GetCurrentValue(), 0.0f);
// }
//
// void UUnitAttributeSet::SetMana(float NewVal) const
// {
// 	// Do not accept values lower than zero.
// 	NewVal = FMath::Max(NewVal, 0.0f);
// 	NewVal = FMath::Min(NewVal, MaxMana.GetCurrentValue());
//
// 	// Make sure we have an Ability System Component instance. This should always be the case.
// 	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
// 	if (ensure(ASC))
// 	{
// 		// Set the base value (not the current value) through the appropriate function.
// 		// This makes sure that any modifiers we have applied will still work properly.
// 		ASC->SetNumericAttributeBase(GetHealthAttribute(), NewVal);
// 	}
// }

void UUnitAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, Mana, OldMana);
}

void UUnitAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, MaxMana, OldMaxMana);
}

void UUnitAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, AttackDamage, OldAttackSpeed);
}

void UUnitAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, AttackDamage, OldAttackDamage);
}

void UUnitAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	// Check to see if this call affects our Health by using the Property Getter.
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// This Gameplay Effect is changing Health. Apply it, but restrict the value first.
		// In this case, Health's base value must be non-negative.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		// This Gameplay Effect is changing Health. Apply it, but restrict the value first.
		// In this case, Health's base value must be non-negative.
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
}
