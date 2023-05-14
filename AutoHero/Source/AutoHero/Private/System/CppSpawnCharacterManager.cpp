// Fill out your copyright notice in the Description page of Project Settings.


#include "System/CppSpawnCharacterManager.h"
#include "AutoHero/AutoHeroGameMode.h"
#include "AutoHero/AutoHeroCharacter.h"
#include "AutoHero/AutoHeroPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ACppSpawnCharacterManager* ACppSpawnCharacterManager::i;
ACppSpawnCharacterManager* ACppSpawnCharacterManager::I()
{
	return i;
}

// Sets default values
ACppSpawnCharacterManager::ACppSpawnCharacterManager()
{
	i = this;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACppSpawnCharacterManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACppSpawnCharacterManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACppSpawnCharacterManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACppSpawnCharacterManager::LoadCharacter()
{
	
}

