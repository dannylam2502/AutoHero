// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Services/BTService_IsUnitDead.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Actors/BaseUnit.h"

FString UBTService_IsUnitDead::GetStaticDescription() const
{
	return TEXT("BTService Is Unit Dead?");
}

void UBTService_IsUnitDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	const ABaseUnit* Unit = Cast<ABaseUnit>(AIController->GetPawn());
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (Unit->GetCurrentHealth() > 0)
	{
		BlackboardComponent->SetValueAsBool(BlackboardKey.SelectedKeyName, false);
	}
	else
	{
		BlackboardComponent->SetValueAsBool(BlackboardKey.SelectedKeyName, true);
	}
}