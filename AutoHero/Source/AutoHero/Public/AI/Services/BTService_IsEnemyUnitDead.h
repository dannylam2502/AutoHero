// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_IsEnemyUnitDead.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UBTService_IsEnemyUnitDead : public UBTService
{
	GENERATED_BODY()
public:
	virtual FString GetStaticDescription() const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector BlackboardKeyEnemyUnit;
};
