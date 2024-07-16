// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Slot")
	TSubclassOf<UBaseWidget> UnitSlotTemplate;

	UPROPERTY(BlueprintReadOnly)
	UUnitSelectionSlot* CurrentSelectedSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag")
	TSubclassOf<class APlaceholderUnit> PlaceholderUnitClass;

	UPROPERTY()
	class APlaceholderUnit* PlaceholderUnit;

	UPROPERTY(BlueprintReadOnly, Category = "UnitSlots")
	TArray<UUnitSelectionSlot*> UnitSlots;
	
	UFUNCTION(BlueprintCallable)
	void LoadListHeroes();
	
	UFUNCTION()
	void OnSlotDragDetectedEvent(UUnitSelectionSlot* InUnitSlot, FVector2D InDragPosition);

	UFUNCTION()
	void OnSlotDroppedEvent(UUnitSelectionSlot* InUnitSlot, FVector2D InDropPosition);

	UFUNCTION()
	void OnSlotDragLeaveEvent(UUnitSelectionSlot* InUnitSlot, FVector2D InPosition);

	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
