// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/IngameHUDWidget.h"

#include "GameEnums.h"
#include "AutoHero/AutoHeroPlayerController.h"
#include "Components/VerticalBox.h"
#include "Defines/FUnitData.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/AutoHeroPlayerState.h"
#include "Singletons/UnitDataManager.h"
#include "UI/Widgets/UnitSelectionSlot.h"
#include "Events/ClientGameEventManager.h"

UIngameHUDWidget::UIngameHUDWidget()
{
	CurrentSelectedSlot = nullptr;
}

void UIngameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BtnSubmit->OnClicked.AddDynamic(this, &UIngameHUDWidget::OnClickBtnSubmit);
	BtnTestQuickPlay->OnClicked.AddDynamic(this, &UIngameHUDWidget::OnClickBtnTest);
}

void UIngameHUDWidget::LoadListUnit()
{
	// Check context
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	AAutoHeroPlayerState* PlayerState = Controller->GetPlayerState<AAutoHeroPlayerState>();
	if (PlayerState)
	{
		TArray<int32> UnitIDs = PlayerState->CurGeneratedUnitIds;
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

	if (PlaceholderUnit)
	{
		//PlaceholderUnit->ClientPlaceOnCell();
		AClientGameEventManager::GetInstance(this->GetWorld())->OnClientUnitDropped.Broadcast(PlaceholderUnit, InDropPosition);
		PlaceholderUnit = nullptr; // reset place holder unit ptr
		CurrentSelectedSlot = nullptr; // reset too
	}
}

void UIngameHUDWidget::OnSlotDragLeaveEvent(UUnitSelectionSlot* InUnitSlot, FVector2D InPosition)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("UIngameHUDWidget::OnSlotDragOutsideEvent"));
	
}

void UIngameHUDWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// Only Spawn if there is not placeholder yet
	if (PlaceholderUnit == nullptr)
	{
		// Don't do anything if not current selected widget
		if (CurrentSelectedSlot)
		{
			FUnitData* UnitData = UUnitDataManager::Get()->GetUnitDataByID(CurrentSelectedSlot->UnitID);
			TSubclassOf<ABaseUnit> UnitTemplate = UnitData->UnitActorInstance;
			if (UnitTemplate && GetWorld())
			{
				EActorTeam CurrentTeam = EActorTeam::Blue;
				APlayerController* Controller = GetWorld()->GetFirstPlayerController();
				AAutoHeroPlayerState* PlayerState = Controller->GetPlayerState<AAutoHeroPlayerState>();
				if (PlayerState)
				{
					CurrentTeam	= PlayerState->GetTeam();
				}
				PlaceholderUnit = GetWorld()->SpawnActor<ABaseUnit>(UnitTemplate);
				if (PlaceholderUnit)
				{
					PlaceholderUnit->SetReplicates(false);
					PlaceholderUnit->SetUnitID(UnitData->UnitID);
					PlaceholderUnit->SetUnitState(EUnitState::DraggingFromWidget);
					PlaceholderUnit->ETeam = CurrentTeam;
					// If Team Red Rotate Y to face Enemy
					if (PlaceholderUnit->ETeam == EActorTeam::Red)
					{
						PlaceholderUnit->ClientRotateToFaceEnemy();
					}
					PlaceholderUnit->OnUnitRemovedFromField.AddDynamic(CurrentSelectedSlot, &UUnitSelectionSlot::OnUnitRemovedFromField);
					AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitSpawned.Broadcast(PlaceholderUnit);
				}
			}
		}
	}
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

void UIngameHUDWidget::OnClickBtnSubmit()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("OnClickBtnSubmit"));
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (AAutoHeroPlayerController* AutoHeroController = Cast<AAutoHeroPlayerController>(Controller))
	{
		// Successfully cast, now use AutoHeroController safely
		AutoHeroController->SubmitUnitsToServer();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is not of type AAutoHeroPlayerController!"));
	}

}

void UIngameHUDWidget::OnClickBtnTest()
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("CLICK BTN TEST"));
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (AAutoHeroPlayerController* AutoHeroController = Cast<AAutoHeroPlayerController>(Controller))
	{
		AutoHeroController->ServerStartQuickTest();
	}
}
