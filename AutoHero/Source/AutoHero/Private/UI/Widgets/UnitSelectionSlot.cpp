// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/UnitSelectionSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Defines/FUnitData.h"


void UUnitSelectionSlot::LoadData(FUnitData* Data)
{
	this->SetUnitName(FText::FromString(Data->UnitName));
	this->SetIcon(Data->UnitIcon); 
}

void UUnitSelectionSlot::SetUnitName(FText NewUnitName)
{
	if (this->UnitName)
	{
		this->UnitName->SetText(NewUnitName);
	}
}

void UUnitSelectionSlot::SetIcon(UTexture2D* NewIcon)
{
	if (UnitIcon)
	{
		UnitIcon->SetBrushFromTexture(NewIcon);
	}
}

FReply UUnitSelectionSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bDraggingEnable)
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UUnitSelectionSlot::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InGestureEvent, this, EKeys::TouchKeys[1]).NativeReply;
}

void UUnitSelectionSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (!bDraggingEnable)
	{
		return;
	}
	FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
	OnSlotDragDetectedDel.Broadcast(this, MousePosition);
	
	CurrentDragOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	CurrentDragOperation->DefaultDragVisual = CreateWidget<UUserWidget>(GetWorld(), DragVisualClass);
	CurrentDragOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = CurrentDragOperation;
}

void UUnitSelectionSlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!bDraggingEnable)
	{
		return;
	}
	FVector2D MousePosition = InDragDropEvent.GetScreenSpacePosition();
	OnSlotDragLeaveDel.Broadcast(this, MousePosition);
	// Change the size or visibility of the DefaultDragVisual
	UUserWidget* DragVisual = Cast<UUserWidget>(InOperation->DefaultDragVisual);
	if (DragVisual)
	{
		DragVisual->SetVisibility(ESlateVisibility::Hidden);
	}

	SetVisibility(ESlateVisibility::Hidden);
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

void UUnitSelectionSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!bDraggingEnable)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NativeOnDragCancelled"));
	FVector2D MousePosition = InDragDropEvent.GetScreenSpacePosition();
	OnSlotDroppedDel.Broadcast(this, MousePosition);
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

void UUnitSelectionSlot::SetDraggingEnable(bool bEnable)
{
	bDraggingEnable = bEnable;
}

void UUnitSelectionSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
