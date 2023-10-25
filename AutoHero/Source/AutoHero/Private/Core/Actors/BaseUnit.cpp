// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/BaseUnit.h"

#include "Components/WidgetComponent.h"
#include "Core/Gameplay/UnitAbilitySystemComponent.h"
#include "Core/Gameplay/UnitAttributeSet.h"
#include "Core/Gameplay/UnitGameplayAbility.h"
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
	
	Attributes = CreateDefaultSubobject<UUnitAttributeSet>("Attributes");
	
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
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ABaseUnit::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UUnitGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int>(StartupAbility.GetDefaultObject()->UnitAbilityCommandID), this));
			
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

// Called when the game starts or when spawned
void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
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
	return Attributes->GetHealth();
}

float ABaseUnit::GetMaxHealth() const
{
	return Attributes->GetMaxHealth();
}

