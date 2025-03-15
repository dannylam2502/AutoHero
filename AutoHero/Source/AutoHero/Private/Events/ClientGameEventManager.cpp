// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/ClientGameEventManager.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AClientGameEventManager::AClientGameEventManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

AClientGameEventManager* AClientGameEventManager::GetInstance(UWorld* World)
{
	static AClientGameEventManager* Instance = nullptr;
	if (!Instance)
	{
		Instance = Cast<AClientGameEventManager>(UGameplayStatics::GetActorOfClass(World, AClientGameEventManager::StaticClass()));
	}
	return Instance;
}

void AClientGameEventManager::BPBroadCastOnUnitDropped(ABaseUnit* BaseUnit, FVector2D InDropPosition)
{
	OnClientUnitDropped.Broadcast(BaseUnit, InDropPosition);
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

