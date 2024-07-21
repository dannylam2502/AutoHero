// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines/UnitState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "BaseUnit.generated.h"

class ABaseProjectile;
class UUnitGameplayAbility;
class USphereComponent;
enum class EActorTeam : uint8;
UCLASS()
class AUTOHERO_API ABaseUnit : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseUnit();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void TickWaitingForPlacement(float DeltaTime);
	virtual void TickWhileDragging(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Which team?
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category="Team")
	EActorTeam ETeam;

	// The number *Star
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category="Unit Level")
	int UnitLevel;
	// Attributes
	UPROPERTY(Instanced, VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "Attribute New Set", meta = (AllowPrivateAccess = true))
	const class UUnitAttributeSet* Attributes;

	UFUNCTION(BlueprintCallable, Category = "Unit")
	void SetUnitState(EUnitState NewState);

	UFUNCTION(BlueprintCallable, Category = "Unit")
	EUnitState GetUnitState() const;
	
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	bool IsFullMana() const;
	UFUNCTION(BlueprintCallable)
	FVector GetOffsetWhenDragging() const;
	UFUNCTION(BlueprintCallable)
	FVector GetOffsetWhenPlace();

	UFUNCTION(BlueprintCallable)
	virtual void FinalizePlacement();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ability")
	TArray<TSubclassOf<UUnitGameplayAbility>> DefaultAbilities;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	void BindInput();

	bool IsDead() const;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ability")
	UUnitGameplayAbility* NormalAttackAbility;
	UUnitGameplayAbility* GetNormalAttackAbility() const;

	const class UUnitAttributeSet* GetAttributes();

	UFUNCTION(BlueprintCallable)
	ABaseProjectile* SpawnProjectile(UObject* WorldContextObject, TSubclassOf<ABaseProjectile> BPProjectile, FVector Location, FRotator Rotation, float Speed,
		float Gravity, bool IsHomingTarget ,ABaseUnit* InOwnerUnit = nullptr, ABaseUnit* InTargetUnit = nullptr);

	UFUNCTION(BlueprintCallable)
	void SetCurrentCell(class AUnitCell* InUnitCell);
	UFUNCTION(BlueprintCallable)
	class AUnitCell* GetCurrentCell();
protected:
	class AUnitCell* CurrentCell;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HealthWidgetComp;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = true))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	EUnitState CurrentState;

	void HandleStateChange(EUnitState NewState);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GameplayEffect Event")
	void OnDamageReceived(AActor* InInstigator, AActor* InCauser, const FGameplayTagContainer& InTags, float InDamage);
};
