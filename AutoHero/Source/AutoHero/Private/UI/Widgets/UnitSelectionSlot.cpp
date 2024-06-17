// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/UnitSelectionSlot.h"

#include "Components/Image.h"
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
