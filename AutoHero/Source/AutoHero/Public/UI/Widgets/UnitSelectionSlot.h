// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseWidget.h"
#include "UnitSelectionSlot.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotDropped, UUnitSelectionSlot*, UnitSlot,  FVector2D, DropPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotDragDetected, UUnitSelectionSlot*, UnitSlot,  FVector2D, DropPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotDragLeave, UUnitSelectionSlot*, UnitSlot,  FVector2D, DropPosition);

UCLASS()
class AUTOHERO_API UUnitSelectionSlot : public UBaseWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UnitName;

	UPROPERTY(meta = (BindWidget))
    class UImage* UnitIcon;

	FOnSlotDropped OnSlotDroppedDel;
	FOnSlotDragDetected OnSlotDragDetectedDel;
	FOnSlotDragLeave OnSlotDragLeaveDel;

	bool bDraggingEnable;
	// Functions
	
	void LoadData(struct FUnitData* Data);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetUnitName(FText NewUnitName);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetIcon(UTexture2D* NewIcon);

	void SetDraggingEnable(bool bEnable);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag")
	TSubclassOf<UUserWidget> DragVisualClass;
	UPROPERTY()
	UDragDropOperation* CurrentDragOperation;
	// The ref to the IngameHUD controller
	UPROPERTY()
	class UIngameHUDWidget* IngameHUD;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
