// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "BaseGameplayEffectContext.generated.h"

/**
 * 
 */
USTRUCT()
struct AUTOHERO_API FBaseGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_USTRUCT_BODY()
public:
	FBaseGameplayEffectContext();
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FBaseGameplayEffectContext::StaticStruct();
	}

	virtual FGameplayEffectContext* Duplicate() const override
	{
		FBaseGameplayEffectContext* NewContext = new FBaseGameplayEffectContext();
		*NewContext = *this;
		NewContext->AddActors(Actors);
		if (GetHitResult())
		{
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
};
