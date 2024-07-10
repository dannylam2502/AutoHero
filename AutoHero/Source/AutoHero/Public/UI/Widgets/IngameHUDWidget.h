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
	
	UFUNCTION(BlueprintCallable)
	void LoadListHeroes();
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* UnitList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Slot")
	TSubclassOf<UBaseWidget> UnitSlot;

	UPROPERTY(BlueprintReadOnly)
	UUnitSelectionSlot* CurrentSelectedSlot;
	
};
