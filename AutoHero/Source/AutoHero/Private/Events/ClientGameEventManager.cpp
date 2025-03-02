// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/ClientGameEventManager.h"


// Sets default values
AClientGameEventManager::AClientGameEventManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AClientGameEventManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClientGameEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

