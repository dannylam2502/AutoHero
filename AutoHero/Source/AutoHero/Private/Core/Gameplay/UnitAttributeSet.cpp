// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Gameplay/UnitAttributeSet.h"

#include "Net/UnrealNetwork.h"

UUnitAttributeSet::UUnitAttributeSet()
{
}

void UUnitAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitAttributeSet, AttackDamage, COND_None, REPNOTIFY_Always);
}

void UUnitAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, Health, OldHealth);
}

void UUnitAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, Health, OldMana);
}

void UUnitAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, AttackDamage, OldAttackDamage);
}
