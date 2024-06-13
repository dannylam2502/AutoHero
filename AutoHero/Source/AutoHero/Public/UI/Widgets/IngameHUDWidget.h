// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseWidget.h"
#include "IngameHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UIngameHUDWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	void LoadListHeroes();
	
	
};
