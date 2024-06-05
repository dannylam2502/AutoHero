// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/BaseProjectile.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/Actors/BaseUnit.h"
#include "Core/Gameplay/UnitAbilitySystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABaseProjectile::ABaseProjectile() : OwnerUnit(nullptr), TargetUnit(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;

    //Definition for the SphereComponent that will serve as the Root component for the projectile and its collision.
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    RootComponent = SphereComponent;

    //Definition for the Mesh that will serve as your visual representation.
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    StaticMesh->SetupAttachment(SphereComponent);
	
    //Definition for the Projectile Movement Component.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);

}

void ABaseProjectile::BanishUnit(ABaseUnit* Unit)
{
	UAbilitySystemComponent* OwnerComp = OwnerUnit->GetAbilitySystemComponent();
	UAbilitySystemComponent* TargetComp = Unit->GetAbilitySystemComponent();

	FGameplayEffectContextHandle NewContext = OwnerComp->MakeEffectContext();
	NewContext.AddInstigator(this, this);
	// When applying the effect
	OwnerComp->ApplyGameplayEffectToTarget(GEBanishedToApply.GetDefaultObject(), TargetComp, 1, NewContext);
}

void ABaseProjectile::SetOwnerUnit(ABaseUnit* InOwnerUnit)
{
	this->OwnerUnit = InOwnerUnit;
}

void ABaseProjectile::SetTargetUnit(ABaseUnit* InTargetUnit)
{
	this->TargetUnit = InTargetUnit;
}
