// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/BaseUnit.h"

#include "Components/WidgetComponent.h"
#include "Core/Actors/BaseProjectile.h"
#include "Core/Gameplay/UnitAbilitySystemComponent.h"
#include "Core/Gameplay/UnitAttributeSet.h"
#include "Core/Gameplay/UnitGameplayAbility.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UI/HealthBar.h"

#define DETECTION_RADIUS 10000.0f

// Sets default values
ABaseUnit::ABaseUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    if (RootComponent == nullptr)
    {
	    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    }

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	AbilitySystemComponent = CreateDefaultSubobject<UUnitAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	NormalAttackAbility = nullptr;
	
	// Setup detection sphere
	/*DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(DETECTION_RADIUS);*/
}

UAbilitySystemComponent* ABaseUnit::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseUnit::InitializeAttributes()
{
	// if (AbilitySystemComponent && DefaultAttributeEffect)
	// {
	// 	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	// 	EffectContext.AddSourceObject(this);
	// 	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);
	// 	if (SpecHandle.IsValid())
	// 	{
	// 		FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	// 	}
	// }
}

void ABaseUnit::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UUnitGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			UUnitGameplayAbility* DefaultObject = StartupAbility.GetDefaultObject();
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int>(DefaultObject->UnitAbilityCommandID), this));
			// we will cache the normal attack ability for optimization purpose, since we will need to ref this quite a lot
			if (DefaultObject->UnitAbilityID == EUnitAbilityID::NormalAttack)
			{
				NormalAttackAbility = DefaultObject;
			}
		}
	}
}

void ABaseUnit::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server ASystem init
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	GiveAbilities();
}

void ABaseUnit::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Client call
	// need to init attributes and actor info, but only server gives abilities
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	BindInput();
}

void ABaseUnit::BindInput()
{
	// set up input, may not need when using AI Controller
	if (AbilitySystemComponent && InputComponent)
	{
		FTopLevelAssetPath InputEnumPath = FTopLevelAssetPath(TEXT("/Script/AutoHero"), TEXT("EUnitAbilityCommandID"));
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", InputEnumPath,
			static_cast<int32>(EUnitAbilityCommandID::Confirm), static_cast<int32>(EUnitAbilityCommandID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

bool ABaseUnit::IsDead() const
{
	return GetCurrentHealth() <= 0;
}

UUnitGameplayAbility* ABaseUnit::GetNormalAttackAbility() const
{
	return NormalAttackAbility;
}

const UUnitAttributeSet* ABaseUnit::GetAttributes()
{
	return Attributes;
}

ABaseProjectile* ABaseUnit::SpawnProjectile(UObject* WorldContextObject, TSubclassOf<ABaseProjectile> BPProjectile, FVector Location, FRotator Rotation,  float Speed,
		float Gravity, bool IsHomingTarget, ABaseUnit* InOwnerUnit, ABaseUnit* InTargetUnit)
{
	if (!BPProjectile) // Check if the Actor class is valid
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor class is not valid."));
		return nullptr;
	}
	// Ensure the WorldContextObject is valid and retrieve the World from it
	if (!WorldContextObject) return nullptr;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return nullptr;

	// Define spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn the actor
	ABaseProjectile* SpawnedActor = World->SpawnActor<ABaseProjectile>(BPProjectile, Location, Rotation, SpawnParams);
	if (SpawnedActor)
	{
		// Set the custom property
		SpawnedActor->SetOwnerUnit(InOwnerUnit);
		SpawnedActor->SetTargetUnit(InTargetUnit);
		SpawnedActor->ProjectileMovementComponent->InitialSpeed = Speed;
		SpawnedActor->ProjectileMovementComponent->MaxSpeed = Speed;
		SpawnedActor->ProjectileMovementComponent->ProjectileGravityScale = Speed;
		SpawnedActor->IsHomingTarget = IsHomingTarget;
	}

	return SpawnedActor;
}

// Called when the game starts or when spawned
void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	//Attributes = CreateDefaultSubobject<UUnitAttributeSet>("AttributesNew");
	if (IsValid(AbilitySystemComponent))
	{
		Attributes = AbilitySystemComponent->GetSet<UUnitAttributeSet>();
	}
	
	if (Attributes == NULL || Attributes == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SOMETHING WENT WRONG"))
	}
	
	UHealthBar* HealthBar = Cast<UHealthBar>(HealthWidgetComp->GetUserWidgetObject());
	HealthBar->SetOwnerUnit(this);
}

// Called every frame
void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// TODO Questionable code
	BindInput();
}

float ABaseUnit::GetCurrentHealth() const
{
	if (Attributes == nullptr)
	{
		return 0;
	}
	return Attributes->GetHealth();
}

float ABaseUnit::GetMaxHealth() const
{
	if (Attributes == nullptr)
	{
		return 0;
	}
	return Attributes->GetMaxHealth();
}

bool ABaseUnit::IsFullMana() const
{
	if (Attributes == nullptr)
	{
		return false;
	}
	if (Attributes->GetMaxMana() == 0)
	{
		return false;
	}
	if (Attributes->GetMana() >= Attributes->GetMaxMana())
	{
		return true;
	}
	return false;
}

