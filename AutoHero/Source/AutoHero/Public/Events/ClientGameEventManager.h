// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClientGameEventManager.generated.h"

class ABaseUnit;
class UUnitSelectionSlot;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClientUnitDropped, ABaseUnit*, BaseUnit, FVector2D, InDropPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClientUnitDragging, ABaseUnit*, BaseUnit);


UCLASS()
class AUTOHERO_API AClientGameEventManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClientGameEventManager();

	static AClientGameEventManager* GetInstance(UWorld* World);

	UFUNCTION(BlueprintCallable)
	void BPBroadCastOnUnitDropped(ABaseUnit* InBaseUnit, FVector2D InDropPosition);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	FOnClientUnitDropped OnClientUnitDropped;
	UPROPERTY()
	FOnClientUnitDragging OnClientUnitDragging;
	
};
