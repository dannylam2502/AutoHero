// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.h"
#include "Components/Button.h"
#include "Defines/GeneratedUnitInfoDTO.h"
#include "UI/Widgets/BaseWidget.h"
#include "IngameHUDWidget.generated.h"

class UUnitSelectionSlot;
class UBaseWidgetBlueprint;
class UVerticalBox;
/**
 * 
 */

UCLASS()
class AUTOHERO_API UIngameHUDWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	UIngameHUDWidget();

	UPROPERTY(meta=(BindWidget))
	UVerticalBox* UnitList;

	UPROPERTY(meta=(BindWidget))
	UButton* BtnSubmit;

	UPROPERTY(meta=(BindWidget))
	UButton* BtnTestQuickPlay;

	UPROPERTY(meta=(BindWidget))
	UButton* BtnTestScript;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Slot")
	TSubclassOf<UBaseWidget> UnitSlotTemplate;

	UPROPERTY(BlueprintReadOnly)
	UUnitSelectionSlot* CurrentSelectedSlot;

	UPROPERTY()
	class ABaseUnit* PlaceholderUnit;

	UPROPERTY(BlueprintReadOnly, Category = "UnitSlots")
	TArray<UUnitSelectionSlot*> UnitSlots;
	
	UFUNCTION(BlueprintCallable)
	void LoadListUnit();
	
	UFUNCTION()
	void OnSlotDragDetectedEvent(UUnitSelectionSlot* InUnitSlot, FVector2D InDragPosition);

	UFUNCTION()
	void OnSlotDroppedEvent(UUnitSelectionSlot* InUnitSlot, FVector2D InDropPosition);

	UFUNCTION()
	void OnSlotDragLeaveEvent(UUnitSelectionSlot* InUnitSlot, FVector2D InPosition);

	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeConstruct() override;

	// Button Submit
	UFUNCTION()
	void OnClickBtnSubmit();

	UFUNCTION()
	void OnClickBtnTest();

	UFUNCTION()
	void OnClickBtnTestScript();

	EActorTeam Team;

	UFUNCTION()
	void OnClientSelectableUnitsGenerated(EActorTeam InTeam, TArray<FGeneratedUnitInfoDTO> DTO);
};
