// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "CppPVEMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppPVEMenu : public UCppBaseMenu
{
	GENERATED_BODY()
	
public:
	virtual void Setup() override;
	virtual void Init(TArray<UObject*> initParams) override;
	virtual void Pop() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnBack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnFindMatch;

private:
	UFUNCTION() void OnBackClicked();
	UFUNCTION() void OnFindMatchClicked();


};
