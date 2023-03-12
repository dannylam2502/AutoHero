// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CppBaseMenu.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppBaseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Setup();
	virtual void Init();
	virtual void Pop();

	UPROPERTY(EditAnywhere)
	int32 zOder;
};
