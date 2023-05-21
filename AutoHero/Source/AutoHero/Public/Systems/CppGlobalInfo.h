// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CppGlobalInfo.generated.h"

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppGlobalInfo : public UObject
{
	GENERATED_BODY()

private:
	static UCppGlobalInfo* i;
public:
	static UCppGlobalInfo* I();

public:
	static const FName nameLevelMain;
	static const FName nameLevelGameplay;

public:
	UCppGlobalInfo();
	~UCppGlobalInfo();

};