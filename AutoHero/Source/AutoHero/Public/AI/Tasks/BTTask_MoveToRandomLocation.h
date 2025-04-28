#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToRandomLocation.generated.h"

UCLASS()
class AUTOHERO_API UBTTask_MoveToRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToRandomLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	/** Search radius from the controlled pawn's location */
	UPROPERTY(EditAnywhere, Category = "AI")
	float SearchRadius = 1000.0f;

	/** Blackboard key to store the random location */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetLocationKey;
};
