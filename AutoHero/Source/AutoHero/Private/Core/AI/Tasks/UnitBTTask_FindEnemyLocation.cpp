// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Tasks/UnitBTTask_FindEnemyLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Actors/BaseUnit.h"
#include "Kismet/GameplayStatics.h"

UUnitBTTask_FindEnemyLocation::UUnitBTTask_FindEnemyLocation()
{
	NodeName = TEXT("Find Enemy Location");

	// accept only vectors
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UUnitBTTask_FindEnemyLocation, BlackboardKey));
}

EBTNodeResult::Type UUnitBTTask_FindEnemyLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn {AIController->GetPawn()};
	// cast to BaseUnit
	const ABaseUnit* BaseUnit = Cast<ABaseUnit>(AIPawn);
	const ABaseUnit* Target = Cast<ABaseUnit>(AIController->GetBlackboardComponent()->GetValueAsObject(BBKeyTarget.SelectedKeyName));
	if (BaseUnit && Target)
	{
		FVector OriginLoc = BaseUnit->GetActorLocation();
		FVector TargetLoc = Target->GetActorLocation();
		UE_LOG(LogTemp, Log, TEXT("Origin = %s, Target = %s"), *OriginLoc.ToString(), *TargetLoc.ToString());
		const UNavigationSystemV1* NavSystem = {UNavigationSystemV1::GetCurrent(GetWorld())};
		FNavLocation Location{};
		if (IsValid(NavSystem) && NavSystem->ProjectPointToNavigation(TargetLoc, Location, FVector::Zero(), nullptr, nullptr))
		{
			AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
		}
	}
	// Signal the behavior tree component that the task is finished with a success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UUnitBTTask_FindEnemyLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

void UUnitBTTask_FindEnemyLocation::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}
