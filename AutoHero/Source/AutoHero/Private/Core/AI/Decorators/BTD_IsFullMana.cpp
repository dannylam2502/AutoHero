// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Decorators/BTD_IsFullMana.h"

#include "AIController.h"
#include "Core/Actors/BaseUnit.h"

bool UBTD_IsFullMana::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = UBTDecorator::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	ABaseUnit* Unit = Cast<ABaseUnit>(OwnerComp.GetAIOwner()->GetPawn());
	if (Unit)
	{
		return Unit->IsFullMana();
	}
	
	return false;
}

FString UBTD_IsFullMana::GetStaticDescription() const
{
	return TEXT("BTD Check Mana Is Full");
}
