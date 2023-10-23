// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Services/BTService_IsTargetInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Actors/BaseUnit.h"

FString UBTService_IsTargetInRange::GetStaticDescription() const
{
	return "BTService_IsTargetInRange";
}

void UBTService_IsTargetInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	const ABaseUnit* Unit = Cast<ABaseUnit>(AIController->GetPawn());
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	ABaseUnit* OtherUnit = Cast<ABaseUnit>(BlackboardComponent->GetValueAsObject(BlackboardKey.SelectedKeyName));
	if (Unit && OtherUnit)
	{
		float Distance = Unit->GetDistanceTo(OtherUnit);
		if (Distance <= RangeToCheck)
		{
			BlackboardComponent->SetValueAsBool(BlackboardKeyIsInRange.SelectedKeyName, true);
		}
		else
		{
			BlackboardComponent->SetValueAsBool(BlackboardKeyIsInRange.SelectedKeyName, false);
		}
	}
}
