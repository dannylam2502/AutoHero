// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Chat/CppChatSystem.h"
#include "GameFramework/PlayerController.h"

ACppChatSystem* ACppChatSystem::i;
ACppChatSystem* ACppChatSystem::I()
{
	return i;
}

// Sets default values
ACppChatSystem::ACppChatSystem()
{
	i = this;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACppChatSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACppChatSystem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

// Called every frame
void ACppChatSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACppChatSystem::SendChatMessage(FSChatMessageInfo message)
{

}

void ACppChatSystem::GetChatChannelMessages(eChatSystemChannels channelType, bool& isChannelFound, TArray<FSChatMessageInfo>& arrayMessage)
{

}

void ACppChatSystem::WatchChatChannel(eChatSystemChannels channelType, APlayerController* playerController)
{

}