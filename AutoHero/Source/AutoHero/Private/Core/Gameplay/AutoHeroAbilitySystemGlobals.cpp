// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Gameplay/AutoHeroAbilitySystemGlobals.h"
#include "Core/Gameplay/GamePlayEffect/BaseGameplayEffectContext.h"

FGameplayEffectContext* UAutoHeroAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FBaseGameplayEffectContext();
}
