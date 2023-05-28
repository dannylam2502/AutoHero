// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EActorTeam : uint8
{
	TEAM_PLAYER UMETA(DisplayName = "Player Team"),
	TEAM_ENEMY UMETA(DisplayName = "Enemy Team")
};
