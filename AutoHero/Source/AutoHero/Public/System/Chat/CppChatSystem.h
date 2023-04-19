// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumPlace/CppEChatSystemChannels.h"
#include "StructPlace/CppSChatMessageInfo.h"
#include "Interface/ChatSystem/CppIChatSystemInterface.h"
#include "CppChatSystem.generated.h"

class APlayerController;

UCLASS()
class AUTOHERO_API ACppChatSystem : public AActor, public ICppIChatSystemInterface
{
	GENERATED_BODY()

private:
	static ACppChatSystem* i;
public:
	static ACppChatSystem* I();
	
public:	
	// Sets default values for this actor's properties
	ACppChatSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SendChatMessage(FSChatMessageInfo message) override;
	virtual void GetChatChannelMessage(eChatSystemChannels channelType, bool& isChannelFound, FSChatMessageInfo& message) override;
	virtual void WatchChatChannel(eChatSystemChannels channelType, APlayerController* playerController) override;

};
