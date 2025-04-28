#include "AI/Tasks/BTTask_MoveToRandomLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"

UBTTask_MoveToRandomLocation::UBTTask_MoveToRandomLocation()
{
	NodeName = "Move To Random Location";

	// Allow this task to be interrupted by others (like a higher priority one)
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_MoveToRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	FVector Origin = AIPawn->GetActorLocation();

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return EBTNodeResult::Failed;

	FNavLocation RandomLocation;
	bool bFound = NavSys->GetRandomReachablePointInRadius(Origin, SearchRadius, RandomLocation);
	if (!bFound) return EBTNodeResult::Failed;

	// Set the random location in the blackboard
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, RandomLocation.Location);

	return EBTNodeResult::Succeeded;
}
