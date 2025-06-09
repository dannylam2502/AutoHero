// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.h"
#include "Defines/GeneratedUnitInfoDTO.h"
#include "GameFramework/Actor.h"
#include "ClientGameEventManager.generated.h"

enum class EGamePhase : uint8;
class ABaseUnit;
class UUnitSelectionSlot;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClientUnitDropped, ABaseUnit*, BaseUnit, FVector2D, InDropPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClientUnitDragging, ABaseUnit*, BaseUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClientUnitSpawned, ABaseUnit*, BaseUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClientUnitRemovedFromField, ABaseUnit*, BaseUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClientGamePhaseChanged, EGamePhase, GamePhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClientSelectableUnitsGenerated, EActorTeam, Team, TArray<FGeneratedUnitInfoDTO>, GeneratedUnitsDTO);

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

	UFUNCTION()
	void BroadSelectableUnitsGeneratedEvent(EActorTeam Team, TArray<FGeneratedUnitInfoDTO> DTO);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when ClientUnit Dropped
	UPROPERTY()
	FOnClientUnitDropped OnClientUnitDropped;
	// Called during ClientUnit Dragging
	UPROPERTY()
	FOnClientUnitDragging OnClientUnitDragging;
	// Called when ClientUnit Spawned
	UPROPERTY()
	FOnClientUnitDragging OnClientUnitSpawned;
	// Called when ClientUnit Removed from field
	UPROPERTY()
	FOnClientUnitRemovedFromField OnClientUnitRemovedFromField;
	// Called when client game phase had changed
	UPROPERTY()
	FOnClientGamePhaseChanged OnClientGamePhaseChanged;
	UPROPERTY()
	FOnClientSelectableUnitsGenerated OnClientSelectableUnitsGenerated;
};
