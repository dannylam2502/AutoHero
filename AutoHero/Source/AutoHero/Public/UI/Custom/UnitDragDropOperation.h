// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "UnitDragDropOperation.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDraggedUnit, const FVector2D&, MousePosition);

UCLASS()
class AUTOHERO_API UUnitDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "DragDrop")
	FOnDraggedUnit OnDraggedDel;

	virtual void Dragged_Implementation(const FPointerEvent& PointerEvent) override;
};
