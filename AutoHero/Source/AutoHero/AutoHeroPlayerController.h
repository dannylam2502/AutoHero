// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Defines/GeneratedUnitInfoDTO.h"
#include "Defines/PendingUnitData.h"
#include "Templates/SubclassOf.h"
#include "AutoHeroPlayerController.generated.h"

enum class EGamePhase : uint8;
class ABaseUnit;
// Forward declaration to improve compiling times
class UNiagaraSystem;

/**
 * APlayerController subclass for controlling the hero in the game.
 */
UCLASS()
class AAutoHeroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAutoHeroPlayerController();

	/** Setup the input bindings */
	virtual void SetupInputComponent() override;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_LeftMouse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_TestCameraView;

	/** Function called when the left mouse button is released */
	void OnLeftMouseReleased();

	/** Function to toggle camera symmetric test */
	void OnCameraSymmetricTest();

	// Networking Functions
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGenerateUnitList();
	
	/** Submit all units to the server */
	UFUNCTION(BlueprintCallable, Category="Units")
	void SubmitUnitsToServer();

	void RemoveLocalUnitsOnField();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartQuickTest();

	UFUNCTION()
	void UpdateSelectableUnitsUI(EActorTeam Team, TArray<FGeneratedUnitInfoDTO> SelectableUnitsDTO);

protected:
	// Will be removed when server update
	TArray<ABaseUnit*> LocalPendingUnits;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Player readiness
	UFUNCTION(BlueprintCallable)
	void SetPlayerReady();

	/** Set the player as ready on the server */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerReady();
	void ServerSetPlayerReady_Implementation();
	bool ServerSetPlayerReady_Validate();

	/** Join the game */
	UFUNCTION(BlueprintCallable)
	void JoinGame();

	// Unit generation
	void ServerGenerateUnitList_Implementation();
	bool ServerGenerateUnitList_Validate();

	/** Generate a list of units */
	void GenerateUnitList();

	UFUNCTION()
	void OnClientUnitDropped(ABaseUnit* BaseUnit, FVector2D InDropPosition);
	UFUNCTION()
	void OnClientGamePhaseChanged(EGamePhase GamePhase);

};
