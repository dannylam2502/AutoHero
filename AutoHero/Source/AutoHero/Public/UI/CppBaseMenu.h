// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumPlace/CppEMenuType.h"
#include "Blueprint/UserWidget.h"
#include "CppBaseMenu.generated.h"

DECLARE_DELEGATE(Action);

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppBaseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		eMenuType menuType;

public:
	virtual void Setup();
	virtual void Init();
	virtual void Pop();
	virtual void OnExitGame();

public:
	Action callback;

};
