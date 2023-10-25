// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsTargetInRange.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UBTService_IsTargetInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	virtual FString GetStaticDescription() const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Service")
	float RangeToCheck;
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector BlackboardKeyIsInRange;
};
