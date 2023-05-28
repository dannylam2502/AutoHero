// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Tasks/UnitBTTask_FindEnemyLocation.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Core/Actors/BaseUnit.h"
#include "Kismet/GameplayStatics.h"

UUnitBTTask_FindEnemyLocation::UUnitBTTask_FindEnemyLocation()
{
	NodeName = TEXT("Find Enemy Location");

	// accept only vectors
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UUnitBTTask_FindEnemyLocation, BlackboardKey));
}

EBTNodeResult::Type UUnitBTTask_FindEnemyLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn {AIController->GetPawn()};
	// cast to BaseUnit
	const ABaseUnit* BaseUnit = Cast<ABaseUnit>(AIPawn);
	if (BaseUnit)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseUnit::StaticClass(), FoundActors);

		for (AActor* OtherActor : FoundActors)
		{
			if (OtherActor != BaseUnit)
			{
				FVector Origin = BaseUnit->GetActorLocation();
				FVector Other = OtherActor->GetActorLocation();
				UE_LOG(LogTemp, Log, TEXT("Origin = %s, Other = %s"), *Origin.ToString(), *Other.ToString());
				const UNavigationSystemV1* NavSystem = {UNavigationSystemV1::GetCurrent(GetWorld())};
				FNavLocation Location{};
				if (IsValid(NavSystem) && NavSystem->GetRandomReachablePointInRadius(Other, SearchRadius, Location))
				{
					AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
				}
				break;
			}
		}
	}
	
	/*FNavLocation Location{};

	// Get AI Pawn
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn {AIController->GetPawn()};

	// Get Pawn origin
	const FVector Origin {AIPawn->GetActorLocation()};

	// obtain Navigation System
	const UNavigationSystemV1* NavSystem = {UNavigationSystemV1::GetCurrent(GetWorld())};
	if (IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
	}*/

	// Signal the behavior tree component that the task is finished with a success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UUnitBTTask_FindEnemyLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

void UUnitBTTask_FindEnemyLocation::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}
