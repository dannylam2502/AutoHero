// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/UnitSelectionSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Core/Actors/BaseUnit.h"
#include "Defines/FUnitData.h"
#include "Kismet/GameplayStatics.h"


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
	UDragDropOperation* DragDropOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	DragDropOperation->DefaultDragVisual = CreateWidget<UUserWidget>(GetWorld(), DragVisualClass);
	DragDropOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = DragDropOperation;

	// Spawn the unit at the mouse location
	if (UnitClass)
	{
		FVector WorldLocation, WorldDirection;
		UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0), InMouseEvent.GetScreenSpacePosition(), WorldLocation, WorldDirection);
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.0f);
		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
		{
			ABaseUnit* Unit = GetWorld()->SpawnActor<ABaseUnit>(UnitClass, HitResult.Location, FRotator::ZeroRotator);
			DragDropOperation->Payload = Unit;
		}
	}
}
