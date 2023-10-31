// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAutoHero, Log, All);

UENUM(BlueprintType)
enum class EUnitAbilityCommandID : uint8
{
	None,
	Confirm,
	Cancel,
	Punch
};

UENUM(BlueprintType)
enum class EUnitAbilityID : uint8
{
	None,
	NormalAttack,
	Cedric_OneStar_Skill
};
