// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AutoHeroGameMode.generated.h"

class AAutoHeroCharacter;

UCLASS(minimalapi)
class AAutoHeroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAutoHeroGameMode();

protected:
	virtual void BeginPlay() override;

};