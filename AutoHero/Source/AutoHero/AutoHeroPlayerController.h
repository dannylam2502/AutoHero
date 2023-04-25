// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "EnumPlace/CppEChatSystemChannels.h"
#include "Interface/ChatSystem/CppIChatSystemChannelListener.h"
#include "Interface/ChatSystem/CppIChatSystemInterface.h"
#include "StructPlace/CppSChatMessageInfo.h"
#include "AutoHeroPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AAutoHeroPlayerController : public APlayerController, public ICppIChatSystemChannelListener
{
	GENERATED_BODY()

public:

	AAutoHeroPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationTouchAction;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

#pragma region Interface.
public:
	virtual void OnChatChannelUpdated(eChatSystemChannels channelType, TArray<FSChatMessageInfo> arrayMessage) override;
#pragma endregion

private:
	void BootstrapAll();
	void BootstrapAuthority();
	void BootstrapClient();

#pragma region Chat Cliend.
private:
	UFUNCTION() void OnChatSendMessage(FSChatMessageInfo message);
	UFUNCTION() void OnChatChannelChanged(eChatSystemChannels channelType);
#pragma endregion

#pragma region Sever.
public:
	UFUNCTION(server, unreliable)
		void SendChatMessageToServer(FSChatMessageInfo message);

	UFUNCTION(server, unreliable)
		void SendWatchChannelToServer(eChatSystemChannels channelType);

	UFUNCTION(server, unreliable)
		void PushChannelMessagesToCliend(eChatSystemChannels channelType, const TArray<FSChatMessageInfo>& arrayMessage);

#pragma endregion

private:
	ICppIChatSystemInterface* iChatSystem;

};


