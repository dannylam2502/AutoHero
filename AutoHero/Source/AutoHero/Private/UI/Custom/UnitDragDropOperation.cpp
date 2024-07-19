// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/UnitDragDropOperation.h"

void UUnitDragDropOperation::Dragged_Implementation(const FPointerEvent& PointerEvent)
{
	OnDraggedDel.Broadcast(PointerEvent.GetScreenSpacePosition());
	Super::Dragged_Implementation(PointerEvent);
}
