// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceholderUnit.generated.h"

UCLASS()
class AUTOHERO_API APlaceholderUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceholderUnit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* UnitMesh;

	UFUNCTION(BlueprintCallable, Category = "Unit")
	void SetUnitPosition(FVector NewPosition);

protected:
	virtual void Tick(float DeltaTime) override;
	FVector TargetPosition;
	
	
};
