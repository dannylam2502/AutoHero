// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIHeroController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIHeroController::AAIHeroController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void AAIHeroController::BeginPlay()
{
	Super::BeginPlay();

	auto bt = BehaviorTree.Get();
	if (IsValid(bt))
	{
		RunBehaviorTree(bt);
		BehaviorTreeComponent->StartTree(*bt);
	}
}

void AAIHeroController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(Blackboard.Get()) && IsValid(BehaviorTree.Get()))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree.Get()->BlackboardAsset.Get());
	}
}
