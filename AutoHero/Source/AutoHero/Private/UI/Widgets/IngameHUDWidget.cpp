// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/IngameHUDWidget.h"

#include "AutoHero/AutoHeroPlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Core/Actors/PlaceholderUnit.h"
#include "Defines/FUnitData.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/AutoHeroPlayerState.h"
#include "Singletons/UnitDataManager.h"
#include "UI/Widgets/UnitSelectionSlot.h"


UIngameHUDWidget::UIngameHUDWidget()
{
	CurrentSelectedSlot = nullptr;
}

void UIngameHUDWidget::LoadListHeroes()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	AAutoHeroPlayerState* PlayerState = Controller->GetPlayerState<AAutoHeroPlayerState>();
	if (PlayerState)
	{
		TArray<int32> UnitIDs = PlayerState->CurrentUnitIDs;
		for (auto UnitID : UnitIDs)
		{
			FUnitData* UnitData = UUnitDataManager::Get()->GetUnitDataByID(UnitID);
			if (UnitData)
			{
				UUnitSelectionSlot* UnitSelectionSlot = CreateWidget<UUnitSelectionSlot>(this, UnitSlotTemplate);
				if (UnitSelectionSlot)
				{
					UnitList->AddChildToVerticalBox(UnitSelectionSlot);
					UnitSelectionSlot->LoadData(UnitData);
					UnitSelectionSlot->SetDraggingEnable(true);
					UnitSelectionSlot->OnSlotDragDetectedDel.AddDynamic(this, &UIngameHUDWidget::OnSlotDragDetectedEvent);
					UnitSelectionSlot->OnSlotDroppedDel.AddDynamic(this, &UIngameHUDWidget::OnSlotDroppedEvent);
					UnitSelectionSlot->OnSlotDragLeaveDel.AddDynamic(this, &UIngameHUDWidget::UIngameHUDWidget::OnSlotDragLeaveEvent);
					// Cache the Slot for performance
					UnitSlots.Add(UnitSelectionSlot);
				}
			}
		}
	}
}

void UIngameHUDWidget::OnSlotDragDetectedEvent(UUnitSelectionSlot* InUnitSlot, FVector2D InDragPosition)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("UIngameHUDWidget::OnSlotDraggedEvent"));
	CurrentSelectedSlot = InUnitSlot;
	for (auto UnitSlot : UnitSlots)
	{
		if (UnitSlot != InUnitSlot)
		{
			UnitSlot->SetDraggingEnable(false);
		}
	}
}

void UIngameHUDWidget::OnSlotDroppedEvent(UUnitSelectionSlot* InUnitSlot, FVector2D InDropPosition)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("UIngameHUDWidget::OnSlotDroppedEvent"));
	for (auto UnitSlot : UnitSlots)
	{
		UnitSlot->SetDraggingEnable(true);
	}
}

void UIngameHUDWidget::OnSlotDragLeaveEvent(UUnitSelectionSlot* InUnitSlot, FVector2D InPosition)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("UIngameHUDWidget::OnSlotDragOutsideEvent"));
	
}

void UIngameHUDWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// Don't do anything if not current selected widget
	if (CurrentSelectedSlot)
	{
		if (PlaceholderUnitClass && GetWorld())
		{
			PlaceholderUnit = GetWorld()->SpawnActor<APlaceholderUnit>(PlaceholderUnitClass);
			if (PlaceholderUnit)
			{
			
			}
		}
	}
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}
