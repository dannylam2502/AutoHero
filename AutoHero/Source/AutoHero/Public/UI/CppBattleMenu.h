// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "CppBattleMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppBattleMenu : public UCppBaseMenu
{
	GENERATED_BODY()
	
public:
	virtual void Setup() override;
	virtual void Init(TArray<UObject*> initParams) override;
	virtual void Pop() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* btnEndGame;

private:
	UFUNCTION() void OnEndGameClicked();

};
