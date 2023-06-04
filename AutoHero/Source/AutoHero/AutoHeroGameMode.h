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

private:
	static AAutoHeroGameMode* i;
public:
	static AAutoHeroGameMode* I();

public:
	AAutoHeroGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	UPROPERTY(EditAnywhere) TSubclassOf<class AAutoHeroCharacter> myCharacterClass;
	AAutoHeroCharacter* myCharacter;

	UPROPERTY(EditAnywhere) TSubclassOf<class AAutoHeroPlayerController> myPlayerControllerClass;
	AAutoHeroPlayerController* myPlayerController;
};