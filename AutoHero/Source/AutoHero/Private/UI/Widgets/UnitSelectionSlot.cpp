// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/UnitSelectionSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Core/Actors/PlaceholderUnit.h"
#include "Defines/FUnitData.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widgets/IngameHUDWidget.h"


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
	if (IngameHUD && IngameHUD->CurrentSelectedSlot == nullptr)
	{
		CurrentDragOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
		CurrentDragOperation->DefaultDragVisual = CreateWidget<UUserWidget>(GetWorld(), DragVisualClass);
		CurrentDragOperation->Pivot = EDragPivot::MouseDown;
	
		OutOperation = CurrentDragOperation;
		IngameHUD->CurrentSelectedSlot = this;
	}
}

bool UUnitSelectionSlot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

FReply UUnitSelectionSlot::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UUnitSelectionSlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// Don't do anything if not current selected widget
	if (IngameHUD && IngameHUD->CurrentSelectedSlot == this)
	{
		if (PlaceholderUnitClass && GetWorld())
		{
			PlaceholderUnit = GetWorld()->SpawnActor<APlaceholderUnit>(PlaceholderUnitClass);
			if (PlaceholderUnit)
			{
				//InOperation->Payload = PlaceholderUnit;

				// Change the size or visibility of the DefaultDragVisual
				UUserWidget* DragVisual = Cast<UUserWidget>(InOperation->DefaultDragVisual);
				if (DragVisual)
				{
					DragVisual->SetVisibility(ESlateVisibility::Hidden);
				}

				SetVisibility(ESlateVisibility::Hidden);

				// UMainUIWidget* MainUI = Cast<UMainUIWidget>(GetOwningPlayer()->GetHUD()->GetUserWidgetObject());
				// if (MainUI)
				// {
				// 	MainUI->CurrentPlaceholderUnit = PlaceholderUnit;
				// }
			}
		}
	}
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

void UUnitSelectionSlot::SetIngameHUD(UIngameHUDWidget* InIngameHUD)
{
	this->IngameHUD = InIngameHUD;
}

void UUnitSelectionSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
