// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoHeroPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AutoHeroCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "CppGameState.h"
#include "UI/CppUIManager.h"
#include "UI/CppChatBoxPopup.h"

AAutoHeroPlayerController::AAutoHeroPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AAutoHeroPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	BootstrapAll();
	BootstrapAuthority();
	BootstrapClient();
}

void AAutoHeroPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void AAutoHeroPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AAutoHeroPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AAutoHeroPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AAutoHeroPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AAutoHeroPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AAutoHeroPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AAutoHeroPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AAutoHeroPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AAutoHeroPlayerController::OnTouchReleased);
	}
}

void AAutoHeroPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AAutoHeroPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AAutoHeroPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AAutoHeroPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AAutoHeroPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

#pragma region Interface.
void AAutoHeroPlayerController::OnChatChannelUpdated(eChatSystemChannels channelType, TArray<FSChatMessageInfo> arrayMessage)
{
	// Push channel messages to client.
	PushChannelMessageToCliend(channelType, arrayMessage);
}
#pragma endregion

void AAutoHeroPlayerController::BootstrapAll()
{
	// Executes on authority and local player controllers.
	ACppGameState* gameState = dynamic_cast<ACppGameState*>(UGameplayStatics::GetGameState(GetWorld()));
	if (gameState)
	{
		iChatSystem = gameState->GetChatSystem();
	}

}

void AAutoHeroPlayerController::BootstrapAuthority()
{
	// Executes on authority machine.
	if (HasAuthority())
	{
		iChatSystem->WatchChatChannel(eChatSystemChannels::Global, this);
		//BootstrapClient();
	}
}

void AAutoHeroPlayerController::BootstrapClient()
{
	if (IsLocalPlayerController())
	{
		// Executes on machines that have a local player controller.
		UCppChatBoxPopup* chatBoxPopup = ACppUIManager::I()->chatBoxPopup;
		ACppUIManager::I()->Push(chatBoxPopup);
		chatBoxPopup->onChannelChangedCallback.BindUObject(this, &AAutoHeroPlayerController::OnChatChannelChanged);
		chatBoxPopup->onSendMessageCallback.BindUObject(this, &AAutoHeroPlayerController::OnChatSendMessage);
	}
}

#pragma region Chat Cliend.
void AAutoHeroPlayerController::OnChatSendMessage(FSChatMessageInfo message)
{
	// Send chat message.
	SendChatMessageToServer(message);
}

void AAutoHeroPlayerController::OnChatChannelChanged(eChatSystemChannels channelType)
{
	// Watch chat channel for updates.
	SendWatchChannelToServer(channelType);
}

#pragma endregion

#pragma region Sever.
void AAutoHeroPlayerController::SendChatMessageToServer_Implementation(FSChatMessageInfo message)
{
	iChatSystem->SendChatMessage(message);
}

void AAutoHeroPlayerController::SendWatchChannelToServer_Implementation(eChatSystemChannels channelType)
{
	// Register for channel updates.
	iChatSystem->WatchChatChannel(channelType, this);

	// Get existing messages for this channel we are now watching.
	bool isChannelFound = false;
	TArray<FSChatMessageInfo> arrayMessage;
	iChatSystem->GetChatChannelMessages(channelType, isChannelFound, arrayMessage);
	PushChannelMessageToCliend(channelType, arrayMessage);
}

void AAutoHeroPlayerController::PushChannelMessageToCliend_Implementation(eChatSystemChannels channelType, const TArray<FSChatMessageInfo>& arrayMessage)
{
	ACppUIManager::I()->chatBoxPopup->SetChannelMessages(channelType, arrayMessage);
}

#pragma endregion
