// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UnitBTTask_FindEnemyLocation.generated.h"

/**
 * BTTask for finding a next enemy location
 */
UCLASS()
class AUTOHERO_API UUnitBTTask_FindEnemyLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UUnitBTTask_FindEnemyLocation();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	float SearchRadius {1.0f};

	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector BBKeyTarget;
};
