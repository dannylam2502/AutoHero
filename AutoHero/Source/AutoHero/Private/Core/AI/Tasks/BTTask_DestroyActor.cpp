// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Tasks/BTTask_DestroyActor.h"

#include "AIController.h"

EBTNodeResult::Type UBTTask_DestroyActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPawn {AIController->GetPawn()};
	if (IsValid(AIPawn))
	{
		AIPawn->K2_DestroyActor();
	}
	return EBTNodeResult::Succeeded;
}
