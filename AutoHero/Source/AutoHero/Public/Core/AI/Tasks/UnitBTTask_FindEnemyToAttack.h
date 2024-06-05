// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UnitBTTask_FindEnemyToAttack.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UUnitBTTask_FindEnemyToAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UUnitBTTask_FindEnemyToAttack();
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	
};
