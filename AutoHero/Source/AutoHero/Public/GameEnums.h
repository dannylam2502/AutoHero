// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EActorTeam : uint8
{
	Blue UMETA(DisplayName = "Blue Team"),
	Red UMETA(DisplayName = "Red Team")
};
