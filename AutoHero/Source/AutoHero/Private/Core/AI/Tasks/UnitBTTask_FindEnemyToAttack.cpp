// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Tasks/UnitBTTask_FindEnemyToAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Actors/BaseUnit.h"
#include "Kismet/GameplayStatics.h"

UUnitBTTask_FindEnemyToAttack::UUnitBTTask_FindEnemyToAttack()
{
	NodeName = TEXT("Find Enemy To Attack");
}

EBTNodeResult::Type UUnitBTTask_FindEnemyToAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn {AIController->GetPawn()};
	// cast to BaseUnit
	const ABaseUnit* BaseUnit = Cast<ABaseUnit>(AIPawn);
	if (BaseUnit)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseUnit::StaticClass(), FoundActors);

		for (AActor* OtherActor : FoundActors)
		{
			if (OtherActor != BaseUnit)
			{
				ABaseUnit* Target = Cast<ABaseUnit>(OtherActor);
				AIController->GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, Target);
				NodeResult = EBTNodeResult::Succeeded;
				break;
			}
		}
	}
	
	// Signal the behavior tree component that the task is finished with a success or failure
	FinishLatentTask(OwnerComp, NodeResult);
	return NodeResult;
}

FString UUnitBTTask_FindEnemyToAttack::GetStaticDescription() const
{
	return FString::Printf(TEXT("Target is: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

void UUnitBTTask_FindEnemyToAttack::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}
