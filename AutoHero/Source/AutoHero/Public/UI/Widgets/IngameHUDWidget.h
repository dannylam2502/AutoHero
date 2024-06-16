// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseWidget.h"
#include "IngameHUDWidget.generated.h"

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
	UFUNCTION(BlueprintCallable)
	void LoadListHeroes();
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* UnitList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Slot")
	TSubclassOf<UBaseWidget> UnitSlot;
};
