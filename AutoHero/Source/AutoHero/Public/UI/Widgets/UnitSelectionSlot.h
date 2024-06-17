// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseWidget.h"
#include "UnitSelectionSlot.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UUnitSelectionSlot : public UBaseWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UnitName;

	UPROPERTY(meta = (BindWidget))
    class UImage* UnitIcon;

	void LoadData(struct FUnitData* Data);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetUnitName(FText NewUnitName);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetIcon(UTexture2D* NewIcon);
};
