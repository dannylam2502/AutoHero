// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CppSpawnCharacterManager.generated.h"

class AAutoHeroGameMode;
class AAutoHeroCharacter;
class APlayerController;

UCLASS()
class AUTOHERO_API ACppSpawnCharacterManager : public APawn
{
	GENERATED_BODY()

private:
	static ACppSpawnCharacterManager* i;
public:
	static ACppSpawnCharacterManager* I();

public:
	// Sets default values for this pawn's properties
	ACppSpawnCharacterManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	AAutoHeroGameMode* autoHeroGameMode;

	UPROPERTY(EditAnywhere) TSubclassOf<class AAutoHeroCharacter> autoHeroCharacterClass;
	AAutoHeroCharacter* autoHeroCharacter;

	UPROPERTY(EditAnywhere) TSubclassOf<class APlayerController> aPlayerControllerClass;
	APlayerController* aPlayerController;

public:
	void LoadCharacter();

};
