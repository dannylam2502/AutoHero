// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/AutoHeroAbilitySystemGlobals.h"
#include "Gameplay/GamePlayEffect/BaseGameplayEffectContext.h"

FGameplayEffectContext* UAutoHeroAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FBaseGameplayEffectContext();
}
