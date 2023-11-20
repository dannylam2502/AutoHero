// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Services/BTService_IsBeingCrowdControlled.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Actors/BaseUnit.h"

FString UBTService_IsBeingCrowdControlled::GetStaticDescription() const
{
	return "Is Being CrowdControlled?";
}

void UBTService_IsBeingCrowdControlled::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	const ABaseUnit* Unit = Cast<ABaseUnit>(AIController->GetPawn());
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (Unit)
	{
		int CCTagNum = Unit->GetAbilitySystemComponent()->GetGameplayTagCount(TagToCheck);
		if (CCTagNum > 0)
		{
			BlackboardComponent->SetValueAsBool(BlackboardKey.SelectedKeyName, true);
		}
		else
		{
			BlackboardComponent->SetValueAsBool(BlackboardKey.SelectedKeyName, false);
		}
	}
}
