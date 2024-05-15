// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUnit.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class ABaseUnit;

UCLASS()
class AUTOHERO_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

    // Sphere component used to test collision.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USphereComponent* SphereComponent;

    // Static Mesh used to provide a visual representation of the object.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* StaticMesh;

    // Movement component for handling projectile movement.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> GEBanishedToApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	ABaseUnit* OwnerUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	ABaseUnit* TargetUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
	bool IsHomingTarget;
public:
	UFUNCTION(BlueprintCallable)
	void BanishUnit(ABaseUnit* Unit);

	void SetOwnerUnit(ABaseUnit* InOwnerUnit);
	void SetTargetUnit(ABaseUnit* InTargetUnit);
};
