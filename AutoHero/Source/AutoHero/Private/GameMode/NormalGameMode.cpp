// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NormalGameMode.h"

#include "GameEnums.h"
#include "Core/Actors/BaseUnit.h"
#include "Kismet/GameplayStatics.h"

void ANormalGameMode::Spawn()
{
	return;
	if (ActorToSpawn)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = UGameplayStatics::GetPlayerController(world, 0);
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FRotator rotator;
			FVector spawnLocation(0.0f, -270.0f, 89.0f);
			ABaseUnit* spawnedUnit = world->SpawnActor<ABaseUnit>(ActorToSpawn, spawnLocation, rotator, spawnParams);
			spawnedUnit->ETeam = EActorTeam::Red;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ANormalGameMode::Spawn");
		}
	}
}
