// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/IngameHUDWidget.h"

#include "AutoHero/AutoHeroPlayerController.h"
#include "Components/VerticalBox.h"
#include "Defines/FUnitData.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/AutoHeroPlayerState.h"
#include "Singletons/UnitDataManager.h"


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
				UUserWidget* UnitSlotWidget = CreateWidget<UUserWidget>(this, UnitSlot);
				if (UnitSlotWidget)
				{
					UnitList->AddChildToVerticalBox(UnitSlotWidget);
				}
			}
		}
	}
}
