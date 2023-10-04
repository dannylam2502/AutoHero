// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Gameplay/UnitAbilitySystemComponent.h"

uint8 UUnitAbilitySystemComponent::GetGameplayTaskDefaultPriority() const
{
	return 0;
}

void UUnitAbilitySystemComponent::OnGameplayTaskInitialized(UGameplayTask& Task)
{
	return;
}

UGameplayTasksComponent* UUnitAbilitySystemComponent::GetGameplayTasksComponent(const UGameplayTask& Task) const
{
	return nullptr;
}

AActor* UUnitAbilitySystemComponent::GetGameplayTaskOwner(const UGameplayTask* Task) const
{
	return nullptr;
}

void UUnitAbilitySystemComponent::GrabDebugSnapshot(FVisualLogEntry* Snapshot) const
{
	
}

void UUnitAbilitySystemComponent::OnGameplayTaskActivated(UGameplayTask& Task)
{
}

void UUnitAbilitySystemComponent::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
}
