// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Services/BTService_IsEnemyUnitDead.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Actors/BaseUnit.h"

FString UBTService_IsEnemyUnitDead::GetStaticDescription() const
{
	return TEXT("BTService Is Target Enemy Unit Dead?");
}

void UBTService_IsEnemyUnitDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	const ABaseUnit* OwnerUnit = Cast<ABaseUnit>(AIController->GetPawn());
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	const ABaseUnit* TargetUnit = Cast<ABaseUnit>(BlackboardComponent->GetValueAsObject(BlackboardKeyEnemyUnit.SelectedKeyName));
	if (OwnerUnit && TargetUnit && TargetUnit->IsDead())
	{
		BlackboardComponent->SetValueAsObject(BlackboardKeyEnemyUnit.SelectedKeyName, nullptr);
	}
}
