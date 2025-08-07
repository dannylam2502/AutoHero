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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitRemovedFromField, ABaseUnit*, Unit);
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
	virtual void TickWhileDraggingFromWidget(float DeltaTime);
	virtual void TickWhileDraggingInField(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Which team?
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category="Team")
	EActorTeam ETeam;

	// Which ID?
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category="Unit Data")
	int UnitType;

	// The number *Star
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category="Unit Level")
	int UnitLevel;
	
	// If True, this unit is client-spawned, otherwise, it's server-spawned
	UPROPERTY(Replicated)
	bool bIsClientPlaceHolder;
	
	// Attributes
	UPROPERTY(Instanced, VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "Attribute New Set", meta = (AllowPrivateAccess = true))
	const class UUnitAttributeSet* Attributes;

	// Delegates
	FOnUnitRemovedFromField OnUnitRemovedFromField;
	
	// FUNCTIONS
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
	void SetUnitType(int InUnitType);
	int GetUnitType();
	double GetTimeSpawned();

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
	UFUNCTION(BlueprintCallable, Category = "Unit")
	void RemoveFromField();
	UFUNCTION()
	// For Red Team
	void RotateToFaceEnemy();
	void ClientRotateToFaceEnemy();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRotateToFaceEnemy();
	void ShowCrown(bool bIsShow);

	UFUNCTION()
	void UpgradeToStars(int32 NumStars);

	UFUNCTION()
	void MergeInto(ABaseUnit* TargetUnit);

	UFUNCTION()
	void SetGridPosition(FVector2D InGridPosition);

	UFUNCTION()
	FVector2D GetGridPosition();

	UFUNCTION()
	void SetPlacementTime(double InPlacementTime);

	UFUNCTION()
	double GetPlacementTime() const;

	// Replications
	UFUNCTION()
	void OnRep_StarLevel();

	int32 GetUnitInstanceID() const { return UnitInstanceID; }
	void SetUnitInstanceID(int32 InUnitInstanceID);
	
protected:
	UPROPERTY()
	class AUnitCell* CurrentCell;

	// Unit InstanceID
	UPROPERTY(Replicated)
	int32 UnitInstanceID = -1;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CrownMesh;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HealthWidgetComp;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = true))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	EUnitState CurrentState;

	UPROPERTY(BlueprintReadOnly)
	float TimeSpawned = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool bHasCrown = false;

	UPROPERTY(ReplicatedUsing = OnRep_StarLevel)
	int32 StarLevel = 1;

	UPROPERTY(Replicated)
	FVector2D GridPosition;

	UPROPERTY(Replicated)
	double PlacementTime;

	void HandleStateChange(EUnitState NewState);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GameplayEffect Event")
	void OnDamageReceived(AActor* InInstigator, AActor* InCauser, const FGameplayTagContainer& InTags, float InDamage);
};
