// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AutoHero/AutoHero.h"
#include "UnitGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UUnitGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EUnitAbilityCommandID UnitAbilityCommandID = EUnitAbilityCommandID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EUnitAbilityID UnitAbilityID = EUnitAbilityID::NormalAttack;
};
