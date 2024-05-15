// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AutoHeroAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UAutoHeroAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
	
	
};
