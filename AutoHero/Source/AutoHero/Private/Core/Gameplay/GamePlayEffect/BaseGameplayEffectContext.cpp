// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Gameplay/GamePlayEffect/BaseGameplayEffectContext.h"

FBaseGameplayEffectContext::FBaseGameplayEffectContext()
{
}

bool FBaseGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	return FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
}
