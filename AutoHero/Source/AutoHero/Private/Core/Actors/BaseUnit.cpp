// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/BaseUnit.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/WidgetComponent.h"
#include "Core/Actors/BaseProjectile.h"
#include "Core/Actors/UnitCell.h"
#include "Core/Actors/UnitGrid.h"
#include "Core/Gameplay/UnitAbilitySystemComponent.h"
#include "Core/Gameplay/UnitAttributeSet.h"
#include "Core/Gameplay/UnitGameplayAbility.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/HealthBar.h"

#define DETECTION_RADIUS 10000.0f

class AUnitGrid;
// Sets default values
ABaseUnit::ABaseUnit()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

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

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;

	// Set up Default State as Waiting For Placement
	CurrentState = EUnitState::Default;
	CurrentCell = nullptr;

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
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(StartupAbility, 1, static_cast<int>(DefaultObject->UnitAbilityCommandID), this));
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
		                                       static_cast<int32>(EUnitAbilityCommandID::Confirm),
		                                       static_cast<int32>(EUnitAbilityCommandID::Cancel));
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

ABaseProjectile* ABaseUnit::SpawnProjectile(UObject* WorldContextObject, TSubclassOf<ABaseProjectile> BPProjectile,
                                            FVector Location, FRotator Rotation, float Speed,
                                            float Gravity, bool IsHomingTarget, ABaseUnit* InOwnerUnit,
                                            ABaseUnit* InTargetUnit)
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
		SpawnedActor->ProjectileMovementComponent->ProjectileGravityScale = Gravity;
		SpawnedActor->ProjectileMovementComponent->bIsHomingProjectile = IsHomingTarget;
		if (IsHomingTarget)
		{
			SpawnedActor->ProjectileMovementComponent->HomingTargetComponent = InTargetUnit->GetRootComponent();
		}
	}

	return SpawnedActor;
}

void ABaseUnit::SetCurrentCell(AUnitCell* InUnitCell)
{
	CurrentCell = InUnitCell;
}

AUnitCell* ABaseUnit::GetCurrentCell()
{
	return CurrentCell;
}

void ABaseUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseUnit, ETeam);
	DOREPLIFETIME(ABaseUnit, UnitLevel);
	DOREPLIFETIME(ABaseUnit, Attributes);
	DOREPLIFETIME(ABaseUnit, AbilitySystemComponent);
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

	if (Attributes == nullptr || Attributes == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SOMETHING WENT WRONG ABOUT ATTRIBUTES"))
	}

	if (HealthWidgetComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("HealthWidgetComp is null"));
	}
	else
	{
		UUserWidget* UserWidget = HealthWidgetComp->GetUserWidgetObject();
		if (UserWidget == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("HealthWidgetComp->GetUserWidgetObject() returned null"));
		}
		else
		{
			UHealthBar* HealthBar = Cast<UHealthBar>(UserWidget);
			if (HealthBar == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("HealthWidgetComp->GetUserWidgetObject() returned null"));
			}
			else
			{
				HealthBar->SetOwnerUnit(this);
			}
		}
	}

	if (Attributes)
	{
		Attributes->OnDamageReceived.AddDynamic(this, &ABaseUnit::OnDamageReceived);
	}

	UnitGrid = Cast<AUnitGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AUnitGrid::StaticClass()));
	OnUnitRemovedFromField.AddDynamic(UnitGrid, &AUnitGrid::OnUnitRemovedFromField);
}

void ABaseUnit::HandleStateChange(EUnitState NewState)
{
	// Implement logic to handle state changes
	switch (NewState)
	{
	case EUnitState::DraggingInField:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("StateChange Dragging in Field"));
		if (CurrentCell)
		{
			CurrentCell->SelectCell(true);
		}
		break;
	case EUnitState::WaitingForPlacement:
		// Handle logic when entering waiting state
		break;
	case EUnitState::WaitingForBattle:
		// Handle logic when entering waiting state
		break;
	case EUnitState::InBattle:
		// Handle logic when entering in battle state
		break;
	case EUnitState::Dead:
		// Handle logic when entering dead state
		break;
	default:
		break;
	}

	// Optionally handle exit logic for the previous state
	switch (CurrentState)
	{
	case EUnitState::WaitingForPlacement:
		// Handle logic when exiting waiting state
		break;
	case EUnitState::WaitingForBattle:
		// Handle logic when exiting waiting state
		break;
	case EUnitState::InBattle:
		// Handle logic when exiting in battle state
		break;
	case EUnitState::Dead:
		// Handle logic when exiting dead state
		break;
	default:
		break;
	}
}

void ABaseUnit::RemoveFromField()
{
	OnUnitRemovedFromField.Broadcast(this);
}

//
// void ABaseUnit::OnRep_ReplicatedMovement()
// {
// 	SetActorRotation(ReplicatedMovement);
// }
//
// void ABaseUnit::MulticastSetReplicatedMovement_Implementation(FRotator NewRotation)
// {
// 	if (!HasAuthority())
// 	{
// 		SetActorRotation(NewRotation);
// 	}
// }
//
// void ABaseUnit::ServerSetReplicatedMovement_Implementation(FRotator NewRotation)
// {
// 	ReplicatedMovement = NewRotation;
// 	MulticastSetReplicatedMovement(NewRotation);
// }
//
// bool ABaseUnit::ServerSetReplicatedMovement_Validate(FRotator NewRotation)
// {
// 	return true;
// }

// Called every frame
void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case EUnitState::DraggingFromWidget:
		// Handle dragging state behavior
		TickWhileDraggingFromWidget(DeltaTime);
		break;
	case EUnitState::DraggingInField:
		// Handle dragging state behavior
		TickWhileDraggingInField(DeltaTime);
		break;
	case EUnitState::WaitingForPlacement:
		// Handle waiting state behavior
		TickWaitingForPlacement(DeltaTime);
		break;
	case EUnitState::WaitingForBattle:
		break;
	case EUnitState::InBattle:
		// Handle in battle state behavior
		break;
	case EUnitState::Dead:
		// Handle dead state behavior
		break;
	default:
		break;
	}
}

void ABaseUnit::TickWaitingForPlacement(float DeltaTime)
{
}

void ABaseUnit::TickWhileDraggingFromWidget(float DeltaTime)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		FHitResult HitResult;
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("LineTraceSingleByChannel"));
		FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		FVector WorldLocation, WorldDirection;
		PlayerController->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);
		FVector End = WorldLocation + (WorldDirection * 10000.0f);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, End, ECC_Visibility))
		{
			SetActorLocation(HitResult.Location + GetOffsetWhenDragging());
		}

		// Highlight the nearest cell in the grid manager
		if (UnitGrid)
		{
			UnitGrid->HighlightNearestCell(GetActorLocation());
		}
	}
}

void ABaseUnit::TickWhileDraggingInField(float DeltaTime)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		FHitResult HitResult;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));
		if (PlayerController->GetHitResultUnderCursorForObjects(ObjectTypes, true, HitResult))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("GetHitResultUnderCursorForObjects"));
			// Apply the offset to the hit location
			FVector AdjustedLocation = HitResult.Location + GetOffsetWhenDragging();
			SetActorLocation(AdjustedLocation);
		}

		// Highlight the nearest cell in the grid manager
		if (UnitGrid)
		{
			UnitGrid->HighlightNearestCell(GetActorLocation());
		}
	}
}

// Called to bind functionality to input
void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// TODO Questionable code
	BindInput();
}

FVector ABaseUnit::GetOffsetWhenDragging() const
{
	return FVector(0.0f, 0.0f, 100.0f);
}

FVector ABaseUnit::GetOffsetWhenPlace()
{
	return FVector(0.0f, 0.0f, 56.5f);
}

void ABaseUnit::FinalizePlacement()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Finalize Placement"));
	if (UnitGrid)
	{
		// Find Old Cell and Vacate it
		AUnitCell* OldCell = this->GetCurrentCell();
		if (OldCell)
		{
			UnitGrid->VacateCell(OldCell);
			OldCell->SelectCell(false);
		}
		//FVector SnappedPosition = UnitGrid->GetNearestCellLocation(GetActorLocation());
		AUnitCell* NearestCell = UnitGrid->GetNearestCell();
		if (NearestCell)
		{
			FVector SnappedPosition = NearestCell->GetCellCenterLocation();
			SetActorLocation(SnappedPosition + GetOffsetWhenPlace());
			SetUnitState(EUnitState::WaitingForPlacement);
			// Set Nearest Cell occupied
			UnitGrid->OccupyCell(NearestCell, this);
			this->SetCurrentCell(NearestCell);
		}
	}
}

void ABaseUnit::SetUnitState(EUnitState NewState)
{
	if (CurrentState != NewState)
	{
		HandleStateChange(NewState);
		CurrentState = NewState;
	}
}

EUnitState ABaseUnit::GetUnitState() const
{
	return CurrentState;
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
