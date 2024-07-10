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

	UFUNCTION()
	void SetIngameHUD(UIngameHUDWidget* InIngameHUD);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag")
	TSubclassOf<class APlaceholderUnit> PlaceholderUnitClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag")
	TSubclassOf<UUserWidget> DragVisualClass;
	UPROPERTY()
	UDragDropOperation* CurrentDragOperation;
	UPROPERTY()
	class APlaceholderUnit* PlaceholderUnit;
	// The ref to the IngameHUD controller
	UPROPERTY()
	class UIngameHUDWidget* IngameHUD;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
