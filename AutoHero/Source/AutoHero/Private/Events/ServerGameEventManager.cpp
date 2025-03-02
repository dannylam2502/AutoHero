// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/ServerGameEventManager.h"


// Sets default values
AServerGameEventManager::AServerGameEventManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AServerGameEventManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AServerGameEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

