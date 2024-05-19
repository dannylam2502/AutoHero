// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Gameplay/GEEC/GEEC_UnitNormalAttackDamage.h"

#include "AbilitySystemComponent.h"
#include "Core/Gameplay/UnitAttributeSet.h"

void UGEEC_UnitNormalAttackDamage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackDamage = Cast<UUnitAttributeSet>(SourceASC->GetSet<UUnitAttributeSet>())->GetAttackDamage();
	float TargetDefense = Cast<UUnitAttributeSet>(TargetASC->GetSet<UUnitAttributeSet>())->GetDefense();
	float DamageDone = SourceAttackDamage / ((TargetDefense + 100.0f) / 100.0f);
	if (DamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UUnitAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -DamageDone));
	}
	//Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
}
