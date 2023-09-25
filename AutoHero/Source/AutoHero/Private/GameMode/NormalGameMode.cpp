// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NormalGameMode.h"

#include "GameEnums.h"
#include "Core/Actors/BaseUnit.h"
#include "Core/Actors/BaseUnit.h"
#include "Core/Actors/BaseUnit.h"
#include "Kismet/GameplayStatics.h"

void ANormalGameMode::Spawn()
{
	if (ActorToSpawn)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = UGameplayStatics::GetPlayerController(world, 0);
			FRotator rotator;
			FVector spawnLocation(200.0f, 400.0f, 0.0f);
			ABaseUnit* spawnedUnit = world->SpawnActor<ABaseUnit>(ActorToSpawn, spawnLocation, rotator, spawnParams);
			spawnedUnit->ETeam = EActorTeam::Red;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ANormalGameMode::Spawn");
		}
	}
}
