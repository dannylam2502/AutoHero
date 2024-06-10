// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstances/NormalGameInstance.h"


void UNormalGameInstance::TriggerLevelLoaded()
{
	OnLevelLoaded.Broadcast();
}

